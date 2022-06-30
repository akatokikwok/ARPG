#include "MMOARPGAICharacter.h"
#include "../MMOARPGGameState.h"

void AMMOARPGAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()) {
		if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
			// 让GS读DTRow里的 属性集
			if (FCharacterAttributeTable* InDTR_Attribute = InGameState->GetCharacterAttributeTable(GetID())) {
				// Lambda--把一组GTag转成FName型
				auto ToGamePlayNameTags = [&](const TArray<FGameplayTag>& InTags, TArray<FName>& OutNames) ->void {
					for (auto& Tmp : InTags) {
						OutNames.Add(Tmp.GetTagName());// !!!这里不可以用tostring(), 要用专门的API拿取FName型; 以此确保GTag的Index是标准正确的
					}
				};

				// I. 先使用DATATABLE里的数据源 更新角色属性.
				UpdateAttribute(InDTR_Attribute);

				TArray<FName> ComboAttack_names;
				ToGamePlayNameTags(InDTR_Attribute->ComboAttackTags, ComboAttack_names);

				// II. 更新技能
				if (GetWorld()->IsServer()) {
					// 2.0 在服务器上强制给小怪一个默认的控制器.
					if (!GetController()) {
						APawn::SpawnDefaultController();
					}

					// 2.1 把DT里的GTag转FName型.
					TArray<FName> Skill_names;
					TArray<FName> Limb_names;
					ToGamePlayNameTags(InDTR_Attribute->SkillTags, Skill_names);
					ToGamePlayNameTags(InDTR_Attribute->LimbsTags, Limb_names);

					// 2.2 为这些技能名组 注册数据.
					RegisterGameplayAbility(ComboAttack_names, EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK);
					RegisterGameplayAbility(Skill_names, EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
					RegisterGameplayAbility(Limb_names, EMMOARPGGameplayAbilityType::GAMEPLAYABILITY_LIMBS);
				}
				// III. 非广播版, 注册连招黑盒(服务器客户端都执行)
				// 可以在AI量很大的时候节省带宽.
				AMMOARPGCharacterBase::RegisterComboAttack(ComboAttack_names);
			}
		}
	}
}