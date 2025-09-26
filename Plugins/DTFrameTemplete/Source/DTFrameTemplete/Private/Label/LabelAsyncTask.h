#pragma once
#include "CoreMinimal.h"
#include "Async/AsyncWork.h"

//struct FLabelData
//{
//	FString LabelUUID;
//	FOutputLabelData Output;
//	FInputLabelData Input;
//};
//struct FOutputLabelData
//{
//	FVector2D Postion;
//	bool bIsLeaf;
//	int32 GruopCount;//聚合后的数量
//};
//struct FInputLabelData
//{
//	FString LabelType;
//	FVector Location;
//	void* Datas;
//};
class ULabelBase;
struct FLabelData
{
	//Input
	FString LabelType;
	FVector Location;
	void* Datas;
	//Output
	FVector2D Postion;
	bool bIsLeaf;
	int32 GruopCount;//聚合后的数量
	TSubclassOf<ULabelBase> LabelBaseClass;
};

class FLabelAsyncsTask : public FNonAbandonableTask
{

friend class FAsyncTask<FLabelAsyncsTask>;
friend class ULabelPanel;
private:
	FLabelAsyncsTask();
	void InitTask(APlayerController* Contoller, UWorld* InWorld);
	void DoWork();
	void AddLabelData(const FString& LabelType,const FVector& InLoc,void* InData);
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FLabelAsyncsTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<UWorld> MyWorld;
	TArray<FLabelData*> LabelDatas;
};