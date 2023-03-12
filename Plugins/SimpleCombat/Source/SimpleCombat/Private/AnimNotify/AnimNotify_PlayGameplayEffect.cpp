// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotify/AnimNotify_PlayGameplayEffect.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Script/CustomReleasingBuffTarget.h"

UAnimNotify_PlayGameplayEffect::UAnimNotify_PlayGameplayEffect()
	: bTarget(false)// 默认应用给自己
{

}

FString UAnimNotify_PlayGameplayEffect::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_PlayGameplayEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* InOnwer = Cast<AActor>(MeshComp->GetOuter())) {
		if (InOnwer->GetWorld() && InOnwer->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {// 仅承认在服务端
			if (ISimpleComboInterface* InSimpleCombatInterface = Cast<ISimpleComboInterface>(MeshComp->GetOuter())) {// 人物主体必须继承了战斗UINTERFACE
				
				/* I. 依据业务情形来加工一组欲施加buff的作用对象. */
				TArray<AActor*> FindTargets;
				// 策划人员是否在蓝图里配置了脚本来获取定制目标
				if (CustomReleasingBuffTarget) {
					if (UCustomReleasingBuffTarget* InTarget = Cast<UCustomReleasingBuffTarget>(CustomReleasingBuffTarget->GetDefaultObject())) {
						InTarget->GetCustomReleasingBuffTarget(InOnwer, FindTargets);
					}
				}
				// 获取他人
				else if (bTarget) {
					FindTargets.Add(InSimpleCombatInterface->GetTarget());
				}
				// 获取自身
				else {
					FindTargets.Add(InOnwer);
				}

				// II. 扫描所有目标对象, 并对其应用buff
				for (auto& AnyTargetActor : FindTargets) {
					check(AnyTargetActor != nullptr);
					if (IAbilitySystemInterface* InAbilitySystemInterface = Cast<IAbilitySystemInterface>(AnyTargetActor)) {// 这个人必须继承ASC接口
						if (UAbilitySystemComponent* InAbilitySystemComponent = InAbilitySystemInterface->GetAbilitySystemComponent()) {// 获取对象身上的ASC
							/* 用ASC创建出上下文并借此关注具体pawn */
							FGameplayEffectContextHandle EffectContext = InAbilitySystemComponent->MakeEffectContext();
							EffectContext.AddSourceObject(AnyTargetActor);
							/* 使用上下文来创建出1个GEHandle. */
							FGameplayEffectSpecHandle EffectSpecHandle = InAbilitySystemComponent->MakeOutgoingSpec(EffectClass, InSimpleCombatInterface->GetCharacterLevel(), EffectContext);
							// 最终应用GE到 1个pawn对象身上.
							if (EffectSpecHandle.IsValid()) {
								InAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), InAbilitySystemComponent);
							}
						}
					}
				}
			}
		}
	}
}