#include "LabelAsyncTask.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "LabelBase.h"


FLabelAsyncsTask::FLabelAsyncsTask()
{

}

void FLabelAsyncsTask::InitTask(APlayerController* Contoller, UWorld* InWorld)
{
	PlayerController = Contoller;
	MyWorld = InWorld;
}

void FLabelAsyncsTask::DoWork()
{
	while (true)
	{
		for (auto& LabelItem : LabelDatas)
		{
			bool success = PlayerController->ProjectWorldLocationToScreen(LabelItem->Location, LabelItem->Postion);
			if (success)
			{
				const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(MyWorld.Get());
				const float RelativeScale = FMath::Pow(ViewportScale, -1);
				LabelItem->Postion = LabelItem->Postion * RelativeScale;
			}
			//聚合逻辑
			//..
			//输出有效UI
			//..
		}
	}
}

void FLabelAsyncsTask::AddLabelData(const FString& LabelType, const FVector& InLoc, void* InData)
{
	FLabelData* InputData=new FLabelData;
	InputData->LabelType=LabelType;
	InputData->Location = InLoc;
	InputData->Datas = InData;
	LabelDatas.Add(MoveTemp(InputData));
}

