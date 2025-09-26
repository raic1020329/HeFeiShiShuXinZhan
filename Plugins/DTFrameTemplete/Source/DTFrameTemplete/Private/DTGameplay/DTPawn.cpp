// Fill out your copyright notice in the Description page of Project Settings.

#include "DTGameplay/DTPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "DTGameplay/DTGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "../../Public/DTGameplay/DTCommonFunLibrary.h"
#include "GameFramework/InputSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
//#include "../../../../../../Source/CalculatedPolygon/Public/CalculatedPolygonBPLibrary.h"

// Sets default values

#define UE_LOGTimeAndFunction \
{	\
	UE_LOG(LogTemp, Warning, TEXT("[%s][%s][%d]"), *(FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"))), *(FString(__FUNCTION__)),__LINE__);	\
}	\

//#define LOGTimeAndFunction [%s][%s][%d]
//#define LOGTimeAndFunctionParame *(FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"))), *(FString(__FUNCTION__)),__LINE__

#define UE_LOGTimeAndFunctionToScreen \
{	\
	if(GEngine)	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("[%s][%s][%d]"), *(FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"))), *(FString(__FUNCTION__)), __LINE__));	\
}	\

ADTPawn::ADTPawn()
	: RecAuRotTime(60.f)
	, AuRotSensitivity(1.f)
	, MoveSensitivity(0.2f)
	, RotationSensitivity(0.20f)
	, ScaleSensitivity(2.5f)
	, TouchMoveSensitivity(0.3f)
	, TouchRotationSensitivity(0.20f)
	, TouchScaleSensitivity(2.5f)
	, AutoRotationMode(EAutoRotationMode::RandomDir)
	, OperationMode(EOperationMode::MouseRightRotation)
	, bStartChangeCamera(false)
	, bIsOpenCameraTool(false)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleRootCom");
	RootComponent = CapsuleCollision;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	if (SpringArm)
	{
		SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SpringArm->bEnableCameraLag = true;
		SpringArm->bEnableCameraRotationLag = true;
		SpringArm->bInheritRoll = false;//不允许出现横滚角的旋转
	}
	MyCamera = CreateDefaultSubobject<UCameraComponent>("MyCamera");
	if (MyCamera)
	{
		MyCamera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	}
	AutoPossessPlayer = EAutoReceiveInput::Type::Player0;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void ADTPawn::BeginPlay()
{
	Super::BeginPlay();
	//Input Syetem
	if (APlayerController* playerController = CastChecked<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			EnhancedInputSubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void ADTPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePawnTransform(DeltaTime);//更新相机切换过程
	UpdateTargetArmLength(DeltaTime);//更新臂长
	UpdateCameraLocation();//更新移动相机
	UpdateCameraRotation();//更新旋转相机
	AutoRotationFun(DeltaTime);//自动旋转
	LimitCameraPitch();//限制相机Pitch
	//LimitCameraYaw();//限制相机Yaw
}

void ADTPawn::UpdatePawnTransform(float DeltaTime)
{
	if (bStartChangeCamera)
	{
		if (bIsUseInstant)//瞬切
		{
			CallInstant();
		}
		else//过程切换
		{
			SpringArm->bEnableCameraLag = true;
			SpringArm->bEnableCameraRotationLag = true;
			DeltaSeconds += DeltaTime / ChangeTime;
			if (DeltaSeconds > 1.0)
			{
				DeltaSeconds = 1.0f;
				bStartChangeCamera = false;
				SetActorLocation(TargetTransform.GetLocation(), true);
				SetActorRotation(TargetTransform.GetRotation().Rotator());
				if(GetController())
					GetController()->SetControlRotation(TargetTransform.GetRotation().Rotator());
				if (bIsArchBridge)
				{
					float Distance = FVector::Distance(ArchBridgeStart, ArchBridgeTarget);
					bIsArchBridge = false;
					FRotator Rot = FRotationMatrix::MakeFromX(ArchBridgeTarget - ArchBridgeStart).Rotator();
					CameraFocus(ArchBridgeTarget, Rot, ArchBridgeAnimTime, Distance / 100.f, false);
				}
				CameraChangeComplete.ExecuteIfBound();
				//新修改的 移动完相机之后把碰撞检测重新赋值
			}
			else
			{
				FTransform TempTransform = UKismetMathLibrary::TLerp(StartTransform,
					TargetTransform, DeltaSeconds, ELerpInterpolationMode::QuatInterp);

				SetActorLocation(TempTransform.GetLocation(), true);
				SetActorRotation(TempTransform.GetRotation().Rotator());
				if(GetController())
					GetController()->SetControlRotation(TempTransform.GetRotation().Rotator());
			}
		}

	}
	else
	{
		if (EnableInstant && bIsInstantComplete)//下一帧将弹簧臂设置为平滑过度的状态
		{
			EnableInstant = false;
			SpringArm->bEnableCameraLag = true;
			SpringArm->bEnableCameraRotationLag = true;
			SpringArm->bUseCameraLagSubstepping = true;
			SpringArm->CameraRotationLagSpeed = 10;
			bIsInstantComplete = false;
			CameraChangeComplete.ExecuteIfBound();
		}
		if (NextFrameActive)
		{
			bIsUseInstant = bTmepIsUseInstant;
		}
	}
}

void ADTPawn::UpdateTargetArmLength(float DeltaTime)
{
	float Detla = FMath::Abs(TargetArmLength - SpringArm->TargetArmLength);
	if (Detla>1)
	{
		SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, 3.f);
	}
}

void ADTPawn::MoveCameraInput(FVector MoveAxis, float InScale)
{
	if (CameraMode == ECameraMode::RST)
	{
		//根据臂长移动的权重值不同
		const float MoveWeightValue = FMath::Clamp(SpringArm->TargetArmLength / 100.f, 10.f, SpringArm->TargetArmLength / 100.f);
		FVector Loc = GetActorLocation() + MoveAxis * InScale * MoveWeightValue;
		if (!LimitCameraMove(FVector2D(Loc.X, Loc.Y)))
		{
			SetActorLocation(FVector(Loc.X, Loc.Y, GetActorLocation().Z), true);
		}
	}
}

FVector2D ADTPawn::LerpSrceenPos(float sensitivity)
{
	FVector2D Loc = GetSrceenPostion();
	CurrenMouseOffsetValue = Loc - PreMouseLoc;
	PreMouseLoc = Loc;

	return CurrenMouseOffsetValue * CameraSpeed * sensitivity;//0.04f可以理解成灵敏度
}

FVector2D ADTPawn::GetSrceenPostion()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		FVector2D Location;

		if (PlayerController->GetMousePosition(Location.X, Location.Y))
		{
			float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
			return Location * FMath::Pow(ViewportScale, -1);
		}
		bool bIsCurrentlyPressed = false;
		PlayerController->GetInputTouchState(CurTouchIndex, Location.X, Location.Y, bIsCurrentlyPressed);
		if (bIsCurrentlyPressed)
		{
			float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
			return Location * FMath::Pow(ViewportScale, -1);
		}
	}
	return FVector2D();
}

void ADTPawn::UpdateCameraLocation()
{
	if (OperationMode == EOperationMode::MouseRightRotation)
	{
		if (bIsDTLeftMouseButton && !bStartChangeCamera)
		{
			if (CameraMode == ECameraMode::RST)//相机模式为带有平移属性时的模式，才能启动平移相机
			{
				FVector2D LocOffset = LerpSrceenPos(GetMoveSensitivity() / 5.f);

				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);
				const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				MoveCameraInput(Direction, LocOffset.Y);//向前移动
				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				MoveCameraInput(RightDirection, -LocOffset.X);//左右移动
			}
		}
	}
	else if (OperationMode == EOperationMode::MouseLeftRotation)
	{
		if (bIsDTRightMouseButton && !bStartChangeCamera)
		{
			if (CameraMode == ECameraMode::RST)//相机模式为带有平移属性时的模式，才能启动平移相机
			{
				FVector2D LocOffset = LerpSrceenPos(GetMoveSensitivity() / 5.f);
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);
				const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				MoveCameraInput(Direction, LocOffset.Y);//向前移动
				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
				MoveCameraInput(RightDirection, -LocOffset.X);//左右移动
			}
		}
	}
}

void ADTPawn::UpdateCameraRotation()
{
	if (!bStartChangeCamera)
	{
		if (OperationMode == EOperationMode::MouseRightRotation)
		{
			if (bIsDTRightMouseButton)
			{
				if (CameraMode == ECameraMode::RST || CameraMode == ECameraMode::OnlyRS)//相机模式为带有旋转属性时的模式，才能启动旋转相机
				{
					FVector2D LocOffset = LerpSrceenPos(GetRotationSensitivity() / 5.f);
					if (LimitCameraYaw(LocOffset.X))
						AddControllerYawInput(LocOffset.X);
					AddControllerPitchInput(LocOffset.Y);
					FRotator Rot = GetController()->GetControlRotation();
					Rot.Clamp();
					StopAutoRotation(0);
				}
			}
			else
			{
				if (CameraMode == ECameraMode::OnlyRS && bIsDTLeftMouseButton)
				{
					FVector2D LocOffset = LerpSrceenPos(GetRotationSensitivity() / 5.f);
					if (LimitCameraYaw(LocOffset.X))
						AddControllerYawInput(LocOffset.X);
					AddControllerPitchInput(LocOffset.Y);
					StopAutoRotation(0);
				}
			}
		}
		else if (OperationMode == EOperationMode::MouseLeftRotation)
		{
			if (bIsDTLeftMouseButton)
			{
				if (CameraMode == ECameraMode::RST || CameraMode == ECameraMode::OnlyRS)//相机模式为带有旋转属性时的模式，才能启动旋转相机
				{
					FVector2D LocOffset = LerpSrceenPos(GetRotationSensitivity() / 5.f);
					if (LimitCameraYaw(LocOffset.X))
						AddControllerYawInput(LocOffset.X);
					AddControllerPitchInput(LocOffset.Y);
					StopAutoRotation(0);
				}
			}
			else
			{
				if (CameraMode == ECameraMode::OnlyRS && bIsDTRightMouseButton)
				{
					FVector2D LocOffset = LerpSrceenPos(GetRotationSensitivity() / 5.f);
					if (LimitCameraYaw(LocOffset.X))
						AddControllerYawInput(LocOffset.X);
					AddControllerPitchInput(LocOffset.Y);
					StopAutoRotation(0);
				}
			}
		}
	}
}

void ADTPawn::StopAutoRotation(bool bIsStop)
{
	bIsStartUpAutoRot = bIsStop;
	DeltaAuRotTime = 0.f;
}

void ADTPawn::AutoRotationFun(float DeltaTime)
{
	if (IsAutoRotation && !CameraLimit.bIsOpenYawRange)
	{
		if (!bIsStartUpAutoRot)
		{
			DeltaAuRotTime += DeltaTime;
			if (DeltaAuRotTime >= RecAuRotTime)
			{
				StopAutoRotation(1);
			}
		}
		else
		{
			if (CameraMode == ECameraMode::RST || CameraMode == ECameraMode::OnlyRS)
			{
				static float dir = 1.f;
				static float RandomTime = FMath::RandRange(RecAuRotTime / 2.f, RecAuRotTime * 5);
				static float CumulativeDuration = 0.f;//累计时长
				float Val = DeltaTime * FMath::RandRange(0.2f, 2.0f);
				switch (AutoRotationMode)
				{
				case EAutoRotationMode::Anticlockwise:
					Val = -Val;
					break;
				case EAutoRotationMode::RandomDir:
					AutoRotationMode = EAutoRotationMode::Max;
				case EAutoRotationMode::Max:
					CumulativeDuration += DeltaTime;
					if (CumulativeDuration >= RandomTime)
					{
						CumulativeDuration = 0.f;
						RandomTime = FMath::RandRange(RecAuRotTime / 2.f, RecAuRotTime * 5);
						dir = FMath::RandRange(-1, 1) > 0 ? 1 : -1;
						AutoRotationMode = EAutoRotationMode::RandomDir;
					}
					break;
				}
				Val = Val * dir * AuRotSensitivity;
				AddControllerYawInput(Val);
			}
		}
	}
}

void ADTPawn::LimitCameraPitch()
{
	if (!bStartChangeCamera)
	{
		if (GetController()) {
			FRotator Rot = GetController()->GetControlRotation();
			float TempPitch = Rot.Pitch;
			if (TempPitch >= 270)
			{
				TempPitch -= 360.f;
			}

			Rot.Pitch = FMath::Clamp(TempPitch, PitchRange.X, PitchRange.Y);
			if (TempPitch< PitchRange.X || TempPitch>PitchRange.Y)
			{
				SetActorRotation(Rot);
				GetController()->SetControlRotation(Rot);
			}
		}
	}
}

bool ADTPawn::LimitCameraYaw(float Val)
{
	if (!bStartChangeCamera)
	{
		if (CameraLimit.bIsOpenYawRange)
		{
			FRotator Rot = GetController()->GetControlRotation();
			Rot.Clamp();
			float TempYaw = Rot.Yaw;
			if (Val != 0)
			{
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					TempYaw += Val * UInputSettings::GetInputSettings()->bEnableLegacyInputScales;
				}
			}
			else
			{
				return false;
			}
			if (MinYaw > MaxYaw)
			{
				if (TempYaw <= MinYaw && TempYaw >= MaxYaw)
				{
					return false;
				}
				else
					return true;
			}
			else
			{
				if (TempYaw > MinYaw && TempYaw < MaxYaw)
				{
					return true;
				}
				else
					return false;
			}
		}
		return true;
	}
	return true;
}

bool ADTPawn::LimitCameraMove(FVector2D MoveLoc)
{
	if (CameraLimit.bIsOpenMoveLimit)
	{
		return !MoveRangBox.IsInside(MoveLoc);
	}
	return false;
}

void ADTPawn::OpenCameraTool()
{
	if (UUserWidget* UI = FindObject<UUserWidget>(GetWorld(), TEXT("CameraSettingUI")))
	{
		return;
	}
	if (CameraSettingUI)
	{
		UUserWidget* UISetting = CreateWidget<UUserWidget>(GetWorld(), CameraSettingUI, TEXT("CameraSettingUI"));
		if (UISetting)
		{
			UISetting->AddToViewport(100001);
		}
	}
}

void ADTPawn::RapidDefinitionCameraLookAt()
{
	if (const UWorld* World = GetWorld())
	{
		if (UDTGameInstance* const GameInstance = Cast<UDTGameInstance>(World->GetGameInstance()))
		{
			GameInstance->SaveCameraSaveGame(CameraTag, GetCurrentCameAttribute(), CameraLimit, ECameraType::Common);
#if WITH_EDITOR
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("CameraTag=%s Set Success!"), *CameraTag.ToString()));
#endif
		}
	}
}

void ADTPawn::CallInstant()
{

	EnableInstant = true;
	StopAutoRotation(0);
	SetActorLocation(TargetTransform.GetLocation(), true);
	SetActorRotation(TargetTransform.GetRotation().Rotator());
	GetController()->SetControlRotation(TargetTransform.GetRotation().Rotator());
	SpringArm->TargetArmLength = TargetArmLength;
	bIsInstantComplete = false;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUseCameraLagSubstepping = false;
	SpringArm->CameraRotationLagSpeed = 0;
	bIsInstantComplete = true;
	bStartChangeCamera = false;

}

float ADTPawn::GetMoveSensitivity() const
{
	if (IsTouchActive)
	{
		return TouchMoveSensitivity;
	}
	else
	{
		return MoveSensitivity;
	}

}

float ADTPawn::GetRotationSensitivity() const
{
	if (IsTouchActive)
	{
		return TouchRotationSensitivity;
	}
	else
	{
		return RotationSensitivity;
	}

}

float ADTPawn::GetScaleSensitivity() const
{
	if (IsTouchActive)
	{
		return TouchScaleSensitivity;
	}
	else
	{
		return ScaleSensitivity;
	}

}

// Called to bind functionality to input
void ADTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Input System
	UEnhancedInputComponent* playInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (playInputComponent)
	{
		playInputComponent->BindAction(IA_RightMouse, ETriggerEvent::Started, this, &ADTPawn::DTRightMouseStart);
		playInputComponent->BindAction(IA_RightMouse, ETriggerEvent::Completed, this, &ADTPawn::DTRightMouseCompolete);
		playInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Started, this, &ADTPawn::DTLeftMouseStart);
		playInputComponent->BindAction(IA_LeftMouse, ETriggerEvent::Completed, this, &ADTPawn::DTLeftMouseCompolete);
		playInputComponent->BindAction(IA_WheelUp, ETriggerEvent::Started, this, &ADTPawn::DTWheelUpStart);
		playInputComponent->BindAction(IA_WheelDown, ETriggerEvent::Started, this, &ADTPawn::DTWheelDownStart);
		playInputComponent->BindAction(IA_SaveCameraPoint, ETriggerEvent::Started, this, &ADTPawn::RapidDefinitionCameraLookAt);
	}
}

void ADTPawn::ChangeCamera(FName InTag, FCameraSetting CameraSetting, FCameraLimit InLimit, FCameraChangeComplete CompleteCallBack)
{
	bStartChangeCamera = false;
	bIsUseInstant = false;
	CameraLimit = InLimit;
	FVector Loc = CameraSetting.TargetTransform.GetLocation();
	CameraChangeComplete = CompleteCallBack;
	//如果开启移动限制
	if (CameraLimit.bIsOpenMoveLimit)
	{
		MoveRangBox = FBox2D(FVector2D(Loc.X - CameraLimit.MoveRadius * 100, Loc.Y - CameraLimit.MoveRadius * 100),
			FVector2D(Loc.X + CameraLimit.MoveRadius * 100, Loc.Y + CameraLimit.MoveRadius * 100));
	}
	CameraTag = InTag;

	DeltaSeconds = 0.f;
	CameraSpeed = CameraSetting.CameraSpeed;
	CameraMode = CameraSetting.CameraMode;
	IsAutoRotation = CameraSetting.IsAutoRotation;
	DefaultPitch = CameraSetting.DefaultPitch;
	bIsUseInstant = CameraSetting.bIsUseInstant;
	if (!bIsUseInstant)
	{
		NextFrameActive = false;
	}
	else
	{
		NextFrameActive = true;
		bTmepIsUseInstant = CameraSetting.bIsUseInstant;
	}
	ChangeTime = FMath::Clamp(CameraSetting.ChangeTime, 0.01f, CameraSetting.ChangeTime);
	MinDistance = CameraSetting.MinDistance;
	MaxDistance = CameraSetting.MaxDistance;
	TargetArmLength = FMath::Clamp(CameraSetting.DefaultDistance * 100, MinDistance * 100, MaxDistance * 100);
	PitchRange = CameraSetting.PitchRange;
	TargetTransform = CameraSetting.TargetTransform;
	//如果开启Yaw限制
	if (CameraLimit.bIsOpenYawRange)
	{
		float TempYaw = TargetTransform.GetRotation().Rotator().Yaw;
		if (TempYaw < 0)
		{
			TempYaw = TempYaw + 360;
		}
		MaxYaw = TempYaw + CameraLimit.YawRange / 2.f;
		MinYaw = TempYaw - CameraLimit.YawRange / 2.f;

		MaxYaw = MaxYaw % 360;
		if (MinYaw < 0)
		{
			MinYaw = 360 + MinYaw;
		}
	}
	if (FMath::Abs(MyCamera->FieldOfView - CameraSetting.CameraFOV) > 0.9)
	{
		MyCamera->SetFieldOfView(CameraSetting.CameraFOV);
	}
	//SpringArm->bDoCollisionTest =CameraSetting.bCameraCollsionTest;

	SpringArm->bDoCollisionTest = false;
	StartTransform.SetLocation(GetActorLocation());
	StartTransform.SetRotation(GetController()->GetControlRotation().Quaternion());
	bStartChangeCamera = true;
}

void ADTPawn::CameraFocus(FVector InLoc, FRotator Rot, float AnimTime/* = 1.0f*/, float Distance/* = 100*/, bool bIsInstant/* = false*/)
{
	TargetTransform.SetLocation(InLoc);
	TargetTransform.SetRotation(Rot.Quaternion());
	bStartChangeCamera = false;
	DeltaSeconds = 0.f;
	DefaultPitch = Rot.Pitch;
	bIsUseInstant = AnimTime == 0.f ? true : bIsInstant;
	ChangeTime = AnimTime;
	TargetArmLength = Distance * 100.f;
	//如果开启Yaw限制
	if (CameraLimit.bIsOpenYawRange)
	{
		float TempYaw = TargetTransform.GetRotation().Rotator().Yaw;
		if (TempYaw < 0)
		{
			TempYaw = TempYaw + 360;
		}
		MaxYaw = TempYaw + CameraLimit.YawRange / 2.f;
		MinYaw = TempYaw - CameraLimit.YawRange / 2.f;

		MaxYaw = MaxYaw % 360;
		if (MinYaw < 0)
		{
			MinYaw = 360 + MinYaw;
		}
	}

	StartTransform.SetLocation(GetActorLocation());
	StartTransform.SetRotation(GetController()->GetControlRotation().Quaternion());
	bStartChangeCamera = true;
}

void ADTPawn::CameraFocusArchBridge(FVector InStart, FVector InTarget, float AnimTime /*= 1.0f*/, float degree/* = 80.f*/, float DisScaleVale/* = 1.2f*/)
{
	ArchBridgeTarget = InTarget;
	ArchBridgeStart = InStart;
	bIsArchBridge = true;
	ArchBridgeAnimTime = AnimTime;
	FRotator Rot = GetController()->GetControlRotation();
	Rot.Pitch = Rot.Pitch > -degree ? -degree : Rot.Pitch;
	float Distance = FVector::Distance(InStart, InTarget) * DisScaleVale;
	CameraFocus(InTarget, Rot, AnimTime, Distance / 100.f, false);
}

FCameraSetting ADTPawn::GetCurrentCameAttribute()
{
	FCameraSetting OutsSetting;
	FRotator Rot = GetController()->GetControlRotation();
	FTransform Transform;
	Transform.SetLocation(GetActorLocation());
	Transform.SetRotation(Rot.Quaternion());
	OutsSetting.bCameraCollsionTest = SpringArm->bDoCollisionTest;
	OutsSetting.bIsUseInstant = bIsUseInstant;
	OutsSetting.CameraFOV = MyCamera->FieldOfView;
	OutsSetting.CameraMode = CameraMode;
	OutsSetting.CameraSpeed = CameraSpeed;
	OutsSetting.ChangeTime = ChangeTime;
	OutsSetting.DefaultDistance = SpringArm->TargetArmLength / 100.f;
	OutsSetting.DefaultPitch = Rot.Pitch;
	if (OutsSetting.DefaultPitch > 180)
	{
		OutsSetting.DefaultPitch -= 360;
	}
	OutsSetting.IsAutoRotation = IsAutoRotation;
	OutsSetting.MaxDistance = MaxDistance;
	OutsSetting.MinDistance = MinDistance;
	OutsSetting.PitchRange = PitchRange;
	OutsSetting.TargetTransform = Transform;
	return OutsSetting;
}

void ADTPawn::SetDefaultDistance(float InDis)
{
	SpringArm->TargetArmLength = InDis * 100;
	MaxDistance = SpringArm->TargetArmLength > MaxDistance ? SpringArm->TargetArmLength : MaxDistance;
	MinDistance = SpringArm->TargetArmLength < MinDistance ? SpringArm->TargetArmLength : MinDistance;
}

void ADTPawn::DTWheelUp()
{
	StopAutoRotation(0);
	if (CameraMode != ECameraMode::NotRTS)
	{
		if (!bStartChangeCamera)
		{
			float Length = SpringArm->TargetArmLength;
			if (SpringArm->TargetArmLength < 50.f)
				Length = 50.f;
			TargetArmLength = TargetArmLength - Length * GetScaleSensitivity() / 10.f;
			TargetArmLength = FMath::Clamp(TargetArmLength, MinDistance * 100, MaxDistance * 100);
			DeltaAuRotTime = 0.f;
		}
	}
}

void ADTPawn::DTWheelDown()
{
	StopAutoRotation(0);
	if (CameraMode != ECameraMode::NotRTS)
	{
		if (!bStartChangeCamera)
		{
			float Length = SpringArm->TargetArmLength;
			if (Length < 50.f)
			{
				Length = 50.f;
			}
			TargetArmLength = TargetArmLength + Length * GetScaleSensitivity() / 10.f;
			TargetArmLength = FMath::Clamp(TargetArmLength, MinDistance * 100, MaxDistance * 100);
			DeltaAuRotTime = 0.f;
		}
	}
}

void ADTPawn::DTInputTouchDown(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	CurTouchIndex = FingerIndex;
	TouchLoc = Location;
	TouchArr.Add(FingerIndex);
	IsTouchActive = true;
	if (TouchArr.Num() > 2)
	{
		bTouchLeftPressed = true;
		StopAutoRotation(0);
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			bIsDTLeftMouseButton = true;
			PreMouseLoc = GetSrceenPostion();
			if (CameraMode == ECameraMode::RST)
				PlayerController->CurrentMouseCursor = EMouseCursor::Type::GrabHandClosed;
		}
	}
	else if (TouchArr.Num() == 1)
	{
		bTouchRightPressed = true;
		StopAutoRotation(0);
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			bIsDTRightMouseButton = true;
			PreMouseLoc = GetSrceenPostion();
			PlayerController->CurrentMouseCursor = EMouseCursor::Type::Default;
		}
	}

}

void ADTPawn::DTInputTouchUp(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	TouchArr.Empty();
	if (bTouchLeftPressed)
	{
		bTouchLeftPressed = false;
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->CurrentMouseCursor = EMouseCursor::Type::Default;
		}
		bIsDTLeftMouseButton = false;
	}
	if (bTouchRightPressed)
	{
		bTouchRightPressed = false;
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			PlayerController->CurrentMouseCursor = EMouseCursor::Type::Default;
		}
		bIsDTRightMouseButton = false;
	}
}


void ADTPawn::DTInputTouchMove(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	TouchMoveLoc = Location;
	if (TouchArr.Num() == 2)
	{
		CurrentTouchDelta = FVector::Distance(TouchMoveLoc, TouchLoc);
		if (CurrentTouchDelta > PreTouchDelta)
		{
			DTWheelUp();
		}
		else
		{
			DTWheelDown();
		}
		PreTouchDelta = CurrentTouchDelta;
	}
	TouchLoc = TouchMoveLoc;
}

void ADTPawn::DTRightMouseStart(const FInputActionValue& Value)
{
	StopAutoRotation(0);
	IsTouchActive = false;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		bIsDTRightMouseButton = true;
		PreMouseLoc = GetSrceenPostion();
		PlayerController->CurrentMouseCursor = EMouseCursor::Type::Default;
		PlayerController->GetMousePosition(PressedPos.X, PressedPos.Y);
	}

}

void ADTPawn::DTRightMouseCompolete(const FInputActionValue& Value)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->CurrentMouseCursor = EMouseCursor::Type::Default;
		if (PlayerController->GetMousePosition(ReleasedPos.X, ReleasedPos.Y))
		{
			if (PressedPos.Equals(ReleasedPos, 0.001f))
			{
			}
		}
	}
	bIsDTRightMouseButton = false;
}

void ADTPawn::DTLeftMouseStart(const FInputActionValue& Value)
{
	StopAutoRotation(0);
	IsTouchActive = false;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		bIsDTLeftMouseButton = true;
		PreMouseLoc = GetSrceenPostion();
		if (CameraMode == ECameraMode::RST)
			PlayerController->CurrentMouseCursor = EMouseCursor::Type::GrabHandClosed;
		PlayerController->GetMousePosition(PressedPos.X, PressedPos.Y);
	}
}

void ADTPawn::DTLeftMouseCompolete(const FInputActionValue& Value)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->CurrentMouseCursor = EMouseCursor::Type::Default;
		if (PlayerController->GetMousePosition(ReleasedPos.X, ReleasedPos.Y))
		{
			if (PressedPos.Equals(ReleasedPos, 0.001f))
			{
				MouseLeftClicked();		
				AActor* Temp = UDTCommonFunLibrary::GetActorByMouse(this);
				if (Temp->IsValidLowLevel())
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Temp->GetName());
			}
		}
	}
	bIsDTLeftMouseButton = false;
}

void ADTPawn::DTWheelUpStart(const FInputActionValue& Value)
{
	DTWheelUp();
}

void ADTPawn::DTWheelDownStart(const FInputActionValue& Value)
{
	DTWheelDown();
}
