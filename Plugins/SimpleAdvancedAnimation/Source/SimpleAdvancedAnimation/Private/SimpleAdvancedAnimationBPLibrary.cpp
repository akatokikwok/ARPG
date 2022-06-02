// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleAdvancedAnimationBPLibrary.h"
#include "SimpleAdvancedAnimation.h"
#include "Core/SimpleAdvancedAnimSystem.h"

USimpleAdvancedAnimationBPLibrary::USimpleAdvancedAnimationBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float USimpleAdvancedAnimationBPLibrary::SimpleAdvancedAnimationSampleFunction(float Param)
{
	return -1;
}

int32 USimpleAdvancedAnimationBPLibrary::CreateFootIK(ACharacter* InCharacter, const TArray<FName>& InBoneNames, EDrawDebugTrace::Type InFootTraceDrawDebugType, float TraceDistance, float InterpSpeed, float InTraceStart)
{
	return FSimpleAdvancedAnimSystem::Get()->CreateFootIK(InCharacter, InBoneNames, InFootTraceDrawDebugType, TraceDistance, InterpSpeed, InTraceStart);
}

float USimpleAdvancedAnimationBPLibrary::FindOffset(const int32 InKey, const FName& InKeyName)
{
	return FSimpleAdvancedAnimSystem::Get()->FindOffset(InKey, InKeyName);
}

FFootIKInfo USimpleAdvancedAnimationBPLibrary::FindFootIKInfo(const int32 InKey, const FName& InKeyName)
{
	if (FFootIKInfo* InFootIKInfo = FSimpleAdvancedAnimSystem::Get()->FindFootIKInfo(InKey, InKeyName)) {
		return *InFootIKInfo;
	}

	return FFootIKInfo();
}

float USimpleAdvancedAnimationBPLibrary::GetButtZOffset(const TArray<float>& InOffsets)
{
	float TmpValue = 0.f;
	for (auto& Tmp : InOffsets) {
		TmpValue = FMath::Min(TmpValue, Tmp);
	}

	return TmpValue < 0.f ? TmpValue : 0.f;
}

void USimpleAdvancedAnimationBPLibrary::Destroy()
{
	FSimpleAdvancedAnimSystem::Destroy();
}
