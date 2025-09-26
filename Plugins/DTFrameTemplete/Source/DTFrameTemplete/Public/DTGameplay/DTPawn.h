// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraAnchorPoint.h"
#include "DTControlInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include <Components/StaticMeshComponent.h>

#include "DTPawn.generated.h"

UENUM(BlueprintType)
enum class EAutoRotationMode :uint8
{
	//顺时针
	Clockwise,
	//逆时针
	Anticlockwise,
	//随机方向
	RandomDir,
	//UnKown
	Max
};

UENUM(BlueprintType)
enum class EOperationMode :uint8
{
	MouseLeftRotation,
	MouseRightRotation,
};




class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class DTFRAMETEMPLETE_API ADTPawn : public APawn/*,public IDTControlInterface*/
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADTPawn();

public:
	//Setting
	//无人操作时多少秒后开启自动旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float RecAuRotTime;
	//自动旋转灵敏度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AuRotSensitivity;
	//平移灵敏度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 10.f), Category = "Setting")
	float MoveSensitivity;
	//旋转灵敏度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 10.f), Category = "Setting")
	float RotationSensitivity;
	//缩放灵敏度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 10.f), Category = "Setting")
	float ScaleSensitivity;
	//触屏平移灵敏度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 10.f), Category = "Setting|Touch")
	float TouchMoveSensitivity;
	//触屏旋转灵敏度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 10.f), Category = "Setting|Touch")
	float TouchRotationSensitivity;
	//触屏缩放灵敏度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.01f, ClampMax = 10.f), Category = "Setting|Touch")
	float TouchScaleSensitivity;
	//自动旋转的模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	EAutoRotationMode AutoRotationMode;
	//操作模式，左手旋转模式还是右手旋转模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	EOperationMode OperationMode;
	//相机的唯一标识Tag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FName CameraTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> CameraSettingUI;
	UPROPERTY(Transient)
	float CameraSpeed;//相机切换时的速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECameraMode CameraMode;//切换模式
	UPROPERTY(Transient)
	uint8 IsAutoRotation : 1;//是否拥有自动旋转属性
	UPROPERTY(Transient)
	int32 MaxYaw;//最大范围
	UPROPERTY(Transient)
	int32 MinYaw;//最小范围
	UPROPERTY(Transient)
	float DeltaAuRotTime;//自动旋转累加时间，累加到RecAuRotTime秒后就启动自动旋转
	UPROPERTY(Transient)
	uint8 bIsStartUpAutoRot : 1;//是否启动自动旋转
	UPROPERTY(Transient)
	float DefaultPitch;//相机默认的俯仰角
	UPROPERTY(Transient)
	uint8 bIsUseInstant : 1;//是否为瞬切镜头
	UPROPERTY(Transient)
	uint8 bIsInstantComplete : 1;//是否为瞬切镜头
	UPROPERTY(Transient)
	float ChangeTime;//切换相机时所需要的时间
	UPROPERTY(Transient)
	float TargetArmLength;//默认的相机臂长（单位cm）
	UPROPERTY(Transient)
	float MinDistance;//最小臂长（单位cm）
	UPROPERTY(Transient)
	float MaxDistance;//最大臂长（单位cm）
	UPROPERTY(Transient)
	FVector2D PitchRange;//俯仰角的范围 
	UPROPERTY(Transient)
	uint8 bIsDTLeftMouseButton : 1;
	UPROPERTY(Transient)
	uint8 bIsDTRightMouseButton : 1;
	UPROPERTY(Transient)
	FTransform TargetTransform;//最终目标的变换
	UPROPERTY(Transient)
	FTransform StartTransform;//开始时的变换状态
	UPROPERTY(Transient)
	uint8 bStartChangeCamera : 1;
	UPROPERTY(Transient)
	uint8 EnableInstant : 1;
	UPROPERTY(Transient)
	float DeltaSeconds;
	UPROPERTY(Transient)
	TEnumAsByte<ETouchIndex::Type> CurTouchIndex;
	UPROPERTY(Transient)
	TArray<TEnumAsByte<ETouchIndex::Type>> TouchArr;
	UPROPERTY(Transient)
	FVector  TouchLoc;
	UPROPERTY(Transient)
	FVector  TouchMoveLoc;
	UPROPERTY(Transient)
	bool  IsTouchActive;
	UPROPERTY(Transient)
	float CurrentTouchDelta;
	UPROPERTY(Transient)
	float PreTouchDelta;
	UPROPERTY(Transient)
	bool bTouchLeftPressed;
	UPROPERTY(Transient)
	bool bTouchRightPressed;
	UPROPERTY(Transient)
	uint8 bIsOpenCameraTool : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FCameraLimit CameraLimit;
	UPROPERTY(Transient)
	FBox2D MoveRangBox;
	UPROPERTY(Transient)
	bool NextFrameActive;
	UPROPERTY(Transient)
	bool bTmepIsUseInstant;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* MyCamera;

private:
	FVector2D CurrenMouseOffsetValue;
	FVector2D PreMouseLoc;
	FVector ArchBridgeStart;
	FVector ArchBridgeTarget;
	bool bIsArchBridge;
	float ArchBridgeAnimTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION()
	void ChangeCamera(FName InTag, FCameraSetting CameraSetting, FCameraLimit InLimit, FCameraChangeComplete CompleteCallBack = FCameraChangeComplete());
	UFUNCTION(BlueprintCallable)
	void CameraFocus(FVector InLoc, FRotator Rot, float AnimTime = 1.0f, float Distance = 100, bool bIsInstant = false);
	UFUNCTION(BlueprintCallable)
	void CameraFocusArchBridge(FVector InStart, FVector InTarget, float AnimTime = 1.0f, float degree = 80.f, float DisScaleVale = 1.2f);
	UFUNCTION(BlueprintPure)
	FCameraSetting GetCurrentCameAttribute();
	UFUNCTION(BlueprintCallable)
	void SetDefaultDistance(float InDis);
public:
	//更新Pawn的位置跟旋转
	void UpdatePawnTransform(float DeltaTime);
	//更新臂长
	void UpdateTargetArmLength(float DeltaTime);
	//根据轴向移动Pawn
	void MoveCameraInput(FVector MoveAxis, float InScale);
	//获取每一帧的偏移量
	FVector2D LerpSrceenPos(float sensitivity = 0.04f);
	//获取点击的屏幕位置，可能是键盘输入，可能是触手
	FVector2D GetSrceenPostion();
	//更新位置
	void UpdateCameraLocation();
	//更新旋转
	void UpdateCameraRotation();
	//是否停止自动旋转
	void StopAutoRotation(bool bIsStop);
	void AutoRotationFun(float DeltaTime);
	//限制相机俯仰角
	void LimitCameraPitch();
	//限制相机偏航角
	bool LimitCameraYaw(float Val);
	//限制相机偏航角
	bool LimitCameraMove(FVector2D MoveLoc);
	//开启运行模式下定相机的UI面板
	void OpenCameraTool();
	//快速定义相机的观看视角
	void RapidDefinitionCameraLookAt();
	void CallInstant();
protected:
	float GetMoveSensitivity()const;
	float GetRotationSensitivity()const;
	float GetScaleSensitivity()const;
private:
	UFUNCTION()
	void DTWheelUp();
	//滚轮向下事件
	UFUNCTION()
	void DTWheelDown();
	//Touch_Down
	void DTInputTouchDown(const ETouchIndex::Type FingerIndex, const FVector Location);
	//Touch_Up
	void DTInputTouchUp(const ETouchIndex::Type FingerIndex, const FVector Location);
	//Touch_Move
	void DTInputTouchMove(const ETouchIndex::Type FingerIndex, const FVector Location);

private:
	TSharedPtr<class SWindow> CameraSettingWindow;
	FCameraChangeComplete CameraChangeComplete;

public:

	//鼠标press的位子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D	PressedPos;
	//鼠标release的位子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D	ReleasedPos;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MouseLeftClicked();



public:
	//input system
	
	//映射文件
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "EnhancedInput")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	//action文件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput|Basic")
	TObjectPtr<UInputAction> IA_LeftMouse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput|Basic")
	TObjectPtr<UInputAction> IA_RightMouse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput|Basic")
	TObjectPtr<UInputAction> IA_WheelUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput|Basic")
	TObjectPtr<UInputAction> IA_WheelDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnhancedInput|Basic")
	TObjectPtr<UInputAction> IA_SaveCameraPoint;

	UFUNCTION()
	void DTRightMouseStart(const FInputActionValue& Value);
	UFUNCTION()
	void DTRightMouseCompolete(const FInputActionValue& Value);
	UFUNCTION()
	void DTLeftMouseStart(const FInputActionValue& Value);
	UFUNCTION()
	void DTLeftMouseCompolete(const FInputActionValue& Value);
	//滚轮向上事件
	UFUNCTION()
	void DTWheelUpStart(const FInputActionValue& Value);
	//滚轮向下事件
	UFUNCTION()
	void DTWheelDownStart(const FInputActionValue& Value);
};
