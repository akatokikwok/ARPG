#include "Core/SimpleAdvancedAnimSystem.h"
FSimpleAdvancedAnimSystem* FSimpleAdvancedAnimSystem::AdvancedAnimSystem = nullptr;

FSimpleAdvancedAnimSystem* FSimpleAdvancedAnimSystem::Get()
{
	if (!AdvancedAnimSystem) {
		AdvancedAnimSystem = new FSimpleAdvancedAnimSystem();
	}

	return AdvancedAnimSystem;
}

void FSimpleAdvancedAnimSystem::Destroy()
{
	if (AdvancedAnimSystem) {
		delete AdvancedAnimSystem;
		AdvancedAnimSystem = nullptr;
	}
}

TStatId FSimpleAdvancedAnimSystem::GetStatId() const
{
	return TStatId();
}

FSAAHandle FSimpleAdvancedAnimSystem::CreateFootIK(ACharacter* InCharacter, const TArray<FName>& InBoneNames, float InTraceDistance /*= 50.f*/, float InInterpSpeed /*= 18.f*/, float InTraceStart /*= 50.f*/)
{
	FSAAHandle Handle = FMath::RandRange(0, 999999);
	if (!FootIKs.Contains(Handle)) {
		// 构造1个并填值.
		FSimpleFootIK& InFootIK = FootIKs.Add(Handle, FSimpleFootIK());
		InFootIK.Init(InCharacter, InBoneNames, InTraceDistance, InInterpSpeed, InTraceStart);

		return Handle;
	}
	// 循环递归创数据直至查询表内不再有"重复"元素.
	return CreateFootIK(InCharacter, InBoneNames, InTraceDistance, InInterpSpeed);
}

FSimpleFootIK* FSimpleAdvancedAnimSystem::FindFootIK(const FSAAHandle InKey)
{
	return FootIKs.Find(InKey);
}

float FSimpleAdvancedAnimSystem::FindOffset(const FSAAHandle InKey, const FName& InKeyName)
{
	if (FFootIKInfo* InInfo = FindFootIKInfo(InKey, InKeyName)) {
		return InInfo->Offset;
	}

	return 0.0f;
}

FFootIKInfo* FSimpleAdvancedAnimSystem::FindFootIKInfo(const FSAAHandle InKey, const FName& InKeyName)
{
	if (FSimpleFootIK* InFootIK = FindFootIK(InKey)) {
		return InFootIK->FindFootIKInfo(InKeyName);
	}

	return nullptr;
}

void FSimpleAdvancedAnimSystem::Tick(float DeltaTime)
{
	// Tick 表内所有FootIK.
 	for (auto& FootIK_per : FootIKs) {
 		FootIK_per.Value.Tick(DeltaTime);
 	}

 	// 从FootIK查询表内移除那些 人物已经失效了的行.
 	TArray<FSAAHandle> RemoveHandle;
 	for (auto& FootIk_Itr : FootIKs) {
 		if (FootIk_Itr.Value.IsPendingKill()) {
 			RemoveHandle.Add(FootIk_Itr.Key);
 		}
 	}
 	for (auto& Tmp : RemoveHandle) {
 		FootIKs.Remove(Tmp);
 	}
}
