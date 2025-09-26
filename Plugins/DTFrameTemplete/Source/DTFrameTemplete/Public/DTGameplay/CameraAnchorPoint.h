// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "CameraAnchorPoint.generated.h"


DECLARE_DYNAMIC_DELEGATE(FCameraChangeComplete);

UENUM(BlueprintType)
enum class ECameraMode :uint8
{
	//平移旋转缩放
	RST,
	//只有旋转跟缩放
	OnlyRS,
	//只有缩放
	OnlyS,
	//禁止平移旋转缩放
	NotRTS
};
UENUM(BlueprintType)
enum class ECameraType :uint8
{
	//新增
	New,
	//已经存在
	Common,
	//标记为删除
	Delete
};
USTRUCT(BlueprintType)
struct DTFRAMETEMPLETE_API FCameraLimit
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "是否开启偏航角范围"),Category = "CameraLimit")
	uint8 bIsOpenYawRange : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "偏航角的扇形范围",ClampMin = 0.f, ClampMax = 180.f), Category = "CameraLimit")
	float YawRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "是否开启移动限制"), Category = "CameraLimit")
	uint8 bIsOpenMoveLimit : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip ="可移动的半径(单位m)" ), Category = "CameraLimit")
	float MoveRadius;

	FCameraLimit()
		: bIsOpenYawRange(false)
		, YawRange(120.f)
		, bIsOpenMoveLimit(false)
		, MoveRadius(200)
	{

	}
};

USTRUCT(BlueprintType)
struct DTFRAMETEMPLETE_API FCameraSetting
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Tooltip="相机切换时的速度"), Category = "Setting")
	float CameraSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "切换模式"), Category = "Setting")
	ECameraMode CameraMode; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "相机默认的俯仰角"), Category = "Setting")
	float DefaultPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "是否为瞬切镜头"), Category = "Setting")
	uint8 bIsUseInstant : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "切换相机时所需要的时间"), Category = "Setting")
	float ChangeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "默认的相机臂长（单位m）"), Category = "Setting")
	float DefaultDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "最小臂长（单位m）"), Category = "Setting")
	float MinDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "最大臂长（单位m）"), Category = "Setting")
	float MaxDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "俯仰角的范围"), Category = "Setting")
	FVector2D PitchRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "是否拥有自动旋转属性"), Category = "Setting")
	uint8 IsAutoRotation : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 5.f, ClampMax = 170.f,Tooltip = "相机的FOV"), Category = "Setting")
	int32 CameraFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "此相机是否拥有遮挡物碰撞检测"), Category = "Setting")
	uint8 bCameraCollsionTest : 1;
	UPROPERTY(BlueprintReadWrite, meta = (Tooltip = "最终目标的变换"), Category = "Setting")
	FTransform TargetTransform;
public:
	FCameraSetting()
		: CameraSpeed(1.f)
		, CameraMode(ECameraMode::RST)
		, DefaultPitch(-25.f)
		, bIsUseInstant(false)
		, ChangeTime(1.3f)
		, DefaultDistance(800.0f)
		, MinDistance(100.f)
		, MaxDistance(2000.f)
		, PitchRange(-70.f, 0.f)
		, IsAutoRotation(true)
		, CameraFOV(90.f)
		, bCameraCollsionTest(false)
	{

	}
};

class USpringArmComponent;
class UCameraComponent;

UCLASS(BlueprintType)
class DTFRAMETEMPLETE_API ACameraAnchorPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraAnchorPoint();

public:
	//Setting
	//相机的唯一标识Tag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	FName CameraTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,SaveGame, Category = "Setting")
	FCameraSetting CameraSetting;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "CameraLimit")
	FCameraLimit CameraLimit;
	UPROPERTY(BlueprintReadWrite, meta = (Tooltip = "相机的状态"), Category = "Setting")
	ECameraType CameraType;
public:
	UFUNCTION(BlueprintCallable, Category = "CameraAnchorPoint")
	void InitCameraSetting(FCameraSetting Setting);
	UFUNCTION(BlueprintCallable, Category = "CameraAnchorPoint")
	void Init();

public:
	//Setting Function
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetCameraSpeed(float InSpeed=1.f);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetCameraMode(ECameraMode InMode);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetDefaultPitch(float InPitch);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetEnableInstant(bool InEnable);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetChangeTime(float InTime);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetDefaultDistance(float Dis, bool bIsForce = false);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetMinDistance(float Dis);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetMaxDistance(float Dis);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetPitchRange(const FVector2D& InRange);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetEnableAutoRotationValue(bool InEnable);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetCameraFOV(float InFov);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetEnableCameraCollsionTest(bool InEnable);

	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetTargetTransform(const FTransform& InTransform);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetTargetLocation(const FVector& InLoc);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void SetTargetRotation(const FRotator& InRot);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void OpenCameraYawRangeLimit(bool bOpen, float InAngle);
	UFUNCTION(BlueprintCallable, Category = "CameraSetting")
	void OpenCameraMoveLimit(bool bOpen, float InRadius);

public:

	UFUNCTION(CallInEditor, Category = "CameraLimit")
	void DebugRefreshRange();


public:
	UPROPERTY()
	USpringArmComponent* SpringArm;
	UPROPERTY()
	UCameraComponent* MyCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif
};
