// Fill out your copyright notice in the Description page of Project Settings.


#include "DTGameplay/CameraAnchorPoint.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

// Sets default values
ACameraAnchorPoint::ACameraAnchorPoint()
	: Super()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	if (SpringArm)
	{
		SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SpringArm->bDoCollisionTest = CameraSetting.bCameraCollsionTest;
		SpringArm->TargetArmLength = CameraSetting.DefaultDistance*100;
	}
	MyCamera = CreateDefaultSubobject<UCameraComponent>("MyCamera");
	if (MyCamera)
	{
		MyCamera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
		MyCamera->SetFieldOfView(CameraSetting.CameraFOV);
	}
	FRotator Rot = GetActorRotation();
	SetActorRotation(FRotator(CameraSetting.DefaultPitch, Rot.Yaw, Rot.Yaw));
	CameraType = ECameraType::Common;
}

void ACameraAnchorPoint::InitCameraSetting(FCameraSetting Setting)
{
	CameraSetting = Setting;
	if (SpringArm)
	{
		SpringArm->TargetArmLength = CameraSetting.DefaultDistance * 100;
		SpringArm->bDoCollisionTest = CameraSetting.bCameraCollsionTest;
	}
	if (MyCamera)
		MyCamera->SetFieldOfView(CameraSetting.CameraFOV);
	FRotator Rot = CameraSetting.TargetTransform.GetRotation().Rotator();
	SetActorRotation(FRotator(CameraSetting.DefaultPitch, Rot.Yaw, Rot.Roll));
	SetActorLocation(CameraSetting.TargetTransform.GetLocation());
}

void ACameraAnchorPoint::Init()
{
	FRotator Rot = GetActorRotation();
	CameraSetting.DefaultPitch = FMath::Clamp(CameraSetting.DefaultPitch, CameraSetting.PitchRange.X, CameraSetting.PitchRange.Y);
	SetActorRotation(FRotator(CameraSetting.DefaultPitch, Rot.Yaw, Rot.Roll));
}

void ACameraAnchorPoint::SetCameraSpeed(float InSpeed/*=1.f*/)
{
	CameraSetting.CameraSpeed = InSpeed;
}

void ACameraAnchorPoint::SetCameraMode(ECameraMode InMode)
{
	CameraSetting.CameraMode = InMode;
}

void ACameraAnchorPoint::SetDefaultPitch(float InPitch)
{
	CameraSetting.DefaultPitch = FMath::Clamp(InPitch,CameraSetting.PitchRange.X,CameraSetting.PitchRange.Y);
}

void ACameraAnchorPoint::SetEnableInstant(bool InEnable)
{
	CameraSetting.bIsUseInstant = InEnable;
}

void ACameraAnchorPoint::SetChangeTime(float InTime)
{
	CameraSetting.ChangeTime = InTime;
}

void ACameraAnchorPoint::SetDefaultDistance(float Dis, bool bIsForce/*=false*/)
{
	if (bIsForce)
	{
		CameraSetting.DefaultDistance = Dis;
		if (Dis > CameraSetting.MaxDistance)
		{
			CameraSetting.MaxDistance = Dis;
			//CameraSetting.MinDistance = Dis;
		}
		if (Dis < CameraSetting.MinDistance)
		{
			//CameraSetting.MaxDistance = Dis;
			CameraSetting.MinDistance = Dis;
		}
	}
	else
	{
		CameraSetting.DefaultDistance = FMath::Clamp(Dis, CameraSetting.MinDistance, CameraSetting.MaxDistance);
	}
}

void ACameraAnchorPoint::SetMinDistance(float Dis)
{
	CameraSetting.MinDistance = Dis;
	CameraSetting.DefaultDistance = FMath::Clamp(Dis, CameraSetting.MinDistance, CameraSetting.MaxDistance);
}

void ACameraAnchorPoint::SetMaxDistance(float Dis)
{
	CameraSetting.MaxDistance = Dis;
	CameraSetting.DefaultDistance = FMath::Clamp(Dis, CameraSetting.MinDistance, CameraSetting.MaxDistance);
}

void ACameraAnchorPoint::SetPitchRange(const FVector2D& InRange)
{
	CameraSetting.PitchRange = InRange;
}

void ACameraAnchorPoint::SetEnableAutoRotationValue(bool InEnable)
{
	CameraSetting.IsAutoRotation = InEnable;
}

void ACameraAnchorPoint::SetCameraFOV(float InFov)
{
	CameraSetting.CameraFOV = InFov;
}

void ACameraAnchorPoint::SetEnableCameraCollsionTest(bool InEnable)
{
	CameraSetting.bCameraCollsionTest = InEnable;
}

void ACameraAnchorPoint::SetTargetTransform(const FTransform& InTransform)
{
	CameraSetting.TargetTransform = InTransform;
}

void ACameraAnchorPoint::SetTargetLocation(const FVector& InLoc)
{
	CameraSetting.TargetTransform.SetLocation(InLoc);
}

void ACameraAnchorPoint::SetTargetRotation(const FRotator& InRot)
{
	CameraSetting.TargetTransform.SetRotation(InRot.Quaternion());
	CameraSetting.DefaultPitch = InRot.Pitch;
}

void ACameraAnchorPoint::OpenCameraYawRangeLimit(bool bOpen, float InAngle)
{
	CameraLimit.bIsOpenYawRange = bOpen;
	CameraLimit.YawRange = InAngle;
}

void ACameraAnchorPoint::OpenCameraMoveLimit(bool bOpen, float InRadius)
{
	CameraLimit.bIsOpenMoveLimit = bOpen;
	CameraLimit.MoveRadius = InRadius;
}

void ACameraAnchorPoint::DebugRefreshRange()
{
	if (CameraLimit.bIsOpenYawRange)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(GetWorld(), EGetWorldErrorMode::LogAndReturnNull))
		{
			FVector DebugForward = SpringArm->GetForwardVector();
			DebugForward = DebugForward.RotateAngleAxis(CameraSetting.DefaultPitch, SpringArm->GetRightVector());
			::DrawDebugCone(World, GetActorLocation(),
				FVector(-DebugForward),
				SpringArm->TargetArmLength * 0.4f,
				FMath::DegreesToRadians(CameraLimit.YawRange / 2.f),
				FMath::DegreesToRadians(1.f), 5000, FLinearColor::Green.ToFColor(true),
				false, 1, SDPG_MAX, 20);
		}
	}
	if (CameraLimit.bIsOpenMoveLimit)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(GetWorld(), EGetWorldErrorMode::LogAndReturnNull))
		{
			::DrawDebugSolidPlane(World, FPlane(GetActorLocation(), FVector(0, 0, 1)), GetActorLocation(), FVector2D(CameraLimit.MoveRadius * 100), FLinearColor::Green.ToFColor(true), false, 1, SDPG_MAX);
		}
	}

}

// Called when the game starts or when spawned
void ACameraAnchorPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACameraAnchorPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ACameraAnchorPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) 
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetName() == TEXT("DefaultDistance"))
	{
		CameraSetting.DefaultDistance = FMath::Clamp(CameraSetting.DefaultDistance, CameraSetting.MinDistance, CameraSetting.MaxDistance);

		CameraSetting.MinDistance = FMath::Clamp(CameraSetting.MinDistance, 0.f, CameraSetting.DefaultDistance);
		CameraSetting.MaxDistance = FMath::Clamp(CameraSetting.MaxDistance, CameraSetting.DefaultDistance, CameraSetting.MaxDistance);
		if (SpringArm)
		{
			SpringArm->TargetArmLength = CameraSetting.DefaultDistance * 100;
		}
	}
	else if (PropertyChangedEvent.Property->GetName() == TEXT("MinDistance")|| PropertyChangedEvent.Property->GetName() == TEXT("MaxDistance"))
	{
		CameraSetting.DefaultDistance = FMath::Clamp(CameraSetting.DefaultDistance, CameraSetting.MinDistance, CameraSetting.MaxDistance);
	}
	else if (PropertyChangedEvent.Property->GetName() == TEXT("CameraFOV"))
	{
		CameraSetting.CameraFOV= FMath::Clamp(CameraSetting.CameraFOV, 5, 170);
		if(MyCamera)
		MyCamera->SetFieldOfView(CameraSetting.CameraFOV);
	}
	else if (PropertyChangedEvent.Property->GetName() == TEXT("bCameraCollsionTest"))
	{
		if(SpringArm)
			SpringArm->bDoCollisionTest = CameraSetting.bCameraCollsionTest;
	}
	else if (PropertyChangedEvent.Property->GetName() == TEXT("DefaultPitch")||
		PropertyChangedEvent.GetPropertyName() == TEXT("Pitch"))
	{
		FRotator Rot = GetActorRotation();
		CameraSetting.DefaultPitch = FMath::Clamp(CameraSetting.DefaultPitch, CameraSetting.PitchRange.X, CameraSetting.PitchRange.Y);
		SetActorRotation(FRotator(CameraSetting.DefaultPitch, Rot.Yaw, Rot.Roll));
		CameraSetting.TargetTransform.SetRotation(GetActorRotation().Quaternion());
	}
	else if (PropertyChangedEvent.GetPropertyName() == TEXT("X")||
		PropertyChangedEvent.GetPropertyName() == TEXT("Y")|| 
		PropertyChangedEvent.GetPropertyName() == TEXT("Z"))
	{
		CameraSetting.TargetTransform.SetLocation(GetActorLocation());
	}
	else if (PropertyChangedEvent.GetPropertyName() == TEXT("Yaw")||
		PropertyChangedEvent.GetPropertyName() == TEXT("Roll"))
	{
		CameraSetting.TargetTransform.SetRotation(GetActorRotation().Quaternion());

	}
	else if (PropertyChangedEvent.Property->GetName() == TEXT("ActorLabel"))
	{
		Init();
	}
	else if (PropertyChangedEvent.Property->GetName() == TEXT("RelativeLocation")||
		PropertyChangedEvent.Property->GetName() == TEXT("WorldLocation"))
	{
		CameraSetting.TargetTransform.SetLocation(GetActorLocation());
	}
}

void ACameraAnchorPoint::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	if (bFinished)
	{
		CameraSetting.TargetTransform.SetLocation(GetActorLocation());
		CameraSetting.TargetTransform.SetRotation(GetActorRotation().Quaternion());
	}
}
#endif