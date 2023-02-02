// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../../../../MMOARPGGameType.h"
#include "MMOARPGAnimInstanceBase.generated.h"

/**
 * MMOARPG项目的动画势力的 最基类.
 */
UCLASS()
class MMOARPG_API UMMOARPGAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMMOARPGAnimInstanceBase();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltaseconds) override;


	// 找到指定骨骼名 的IK里的偏移值.
	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetFootIKOffset(const FName& InBoneName);

	// 初始化动画实例,构建一些数据,例如IK.
	// 仅当启用IK时, 允许为一系列骨骼构建 IK数据.
	virtual void InitAnimInstance(ACharacter* InCharacter);

public:
	// 是否死亡.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bDeath;
	// 播死亡后仰天暴毙姿势的哪一个 Animseq.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		int32 DieIndex;
	// 人的速率.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		float Speed;
	// 是否滞空.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bInAir;
	// 何种行为状态.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		ECharacterActionState ActionState;
	
	// 是否开启FootIK
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
		bool bFootIK;
	// FOOTIK-ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
		int32 FookIKID;
	// 左脚IK偏移.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
		float LeftOffset;
	// 右脚IK偏移.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
		float RightOffset;
	// 臀部IK 偏移.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
		float ButtZOffset;
	// 左脚IK 骨骼名.
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
		FName LeftBoneName;
	// 右脚IK 骨骼名.
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
		FName RightBoneName;
	// 适用于多足情况.
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
		TArray<FName> BoneNames;

	// 是否被挑飞
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bPickFly;

private:
	// 脚部 射线追踪IK时候的 射线绘制选项.
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
		TEnumAsByte<EDrawDebugTrace::Type> mFootDrawLineTraceType;
		

};
