#pragma once

#include "CoreMinimal.h"
#include "../../MMOARPGGameplayAbility.h"
#include "GameplayAbility_Skill.generated.h"

/**
 * GA:基类.
 * 具有明确逻辑的技能都由此类派生出去.
 */
UCLASS()
class MMOARPG_API UGameplayAbility_Skill : public UMMOARPGGameplayAbility
{
	GENERATED_BODY()
public:

	/** 重写来自UMMOARPGGameplayAbility的虚方法. */
	virtual void OnCompleted() override;
	virtual void OnBlendOut() override;
	virtual void OnInterrupted() override;
	virtual void OnCancelled() override;

};
