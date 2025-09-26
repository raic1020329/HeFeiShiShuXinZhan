#pragma once
#include "DrawDebugHelpers.h"


#define UE_LOGTimeAndFunction \
{	\
	UE_LOG(LogTemp, Warning, TEXT("[%s][%s][%d]"), *(FDateTime::Now().ToString(TEXT("%Y-%m-%d-%H-%M-%S"))), *(FString(__FUNCTION__)),__LINE__);	\
}	

#define UE_LOGTimeAndFunctionToScreen \
{	\
	if(GEngine)	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("[%s][%s][%d]"), *(FDateTime::Now().ToString(TEXT("%Y-%m-%d-%H-%M-%S"))), *(FString(__FUNCTION__)), __LINE__));	\
}	

#define UE_LOGONSCREEN(int_Key,Float_TimeToDisplay,FColor_DisplayColor,FString_DebugMessage, ...)	\
{	\
	if(GEngine)	\
	{	\
		FString Message = FString::Printf(TEXT(FString_DebugMessage),##__VA_ARGS__);	\
		GEngine->AddOnScreenDebugMessage(int_Key, Float_TimeToDisplay, FColor_DisplayColor, Message);	\
	}	\
}

#define DRAW_SPHERE(FVector_Center)	\
{	\
	UWorld* World = GetWorld();	\
	if(World)	\
	{	\
		DrawDebugSphere(World, FVector_Center, 25.f, 24, FColor::Cyan, true);	\
	}	\
}
#define DRAW_SPHERE_SingleFrame(FVector_Center)	\
{	\
	UWorld* World = GetWorld();	\
	if(World)	\
	{	\
		DrawDebugSphere(World, FVector_Center, 25.f, 24, FColor::Cyan, false, -1.f);	\
	}	\
}

#define DRAW_LINE(FVector_Start,FVector_End)	\
{	\
	UWorld* World = GetWorld();	\
	if(World)	\
	{	\
		DrawDebugLine(World, FVector_Start, FVector_End, FColor::Cyan, true, -1.f, 0, 1.f);	\
	}	\
}
#define DRAW_LINE_SingleFrame(FVector_Start,FVector_End)	\
{	\
	UWorld* World = GetWorld();	\
	if(World)	\
	{	\
		DrawDebugLine(World, FVector_Start, FVector_End, FColor::Cyan, false, -1.f, 0, 1.f);	\
	}	\
}

#define DRAW_POINT(FVector_Position)	\
{	\
	UWorld* World = GetWorld();	\
	if(World)	\
	{	\
		DrawDebugPoint(World, FVector_Position, 15.f, FColor::Cyan, true);	\
	}	\
}
#define DRAW_POINT_SingleFrame(FVector_Position)	\
{	\
	UWorld* World = GetWorld();	\
	if(World)	\
	{	\
		DrawDebugPoint(World, FVector_Position, 15.f, FColor::Cyan, false, -1.f);	\
	}	\
}

#define DRAW_VECTOR(FVector_Start,FVector_End)	\
{	\
	UWorld* World = GetWorld();	\
	if(World)	\
	{	\
		DrawDebugLine(World, FVector_Start, FVector_End, FColor::Cyan, true, -1.f, 0, 1.f);	\
		DrawDebugPoint(World, FVector_End, 15.f, FColor::Cyan, true);	\
	}	\
}
#define DRAW_VECTOR_SingleFrame(FVector_Start,FVector_End)	\
{	\
	UWorld* World = GetWorld();	\
	if(World)	\
	{	\
		DrawDebugLine(World, FVector_Start, FVector_End, FColor::Cyan, false, -1.f, 0, 1.f);	\
		DrawDebugPoint(World, FVector_End, 15.f, FColor::Cyan, false, -1.f);	\
	}	\
}