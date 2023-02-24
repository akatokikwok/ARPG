// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomReleasingBuffTarget.generated.h"

/**
 * 脚本/算法: 专门由策划负责限制条件的获取一些目标(比如卡距离)
 * 专门负责读取获取 自定义的目标Actor或对象.
 */
UCLASS(BlueprintType, Blueprintable)
class SIMPLECOMBAT_API UCustomReleasingBuffTarget : public UObject
{
	GENERATED_BODY()

public:
	// 获取欲应用buff的自定义对象/目标, 并保存它们
	virtual void GetCustomReleasingBuffTarget(AActor* InOwner, TArray<AActor*>& OutTarget);

	// 获取欲应用buff的自定义对象/目标
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "CustomReleasing", meta = (DisplayName = "GetCustomReleasingBuffTarget"))
		void K2_GetCustomReleasingBuffTarget(AActor* InOwner, TArray<AActor*>& OutTarget);
	void K2_GetCustomReleasingBuffTarget_Implementation(AActor* InOwner, TArray<AActor*>& OutTarget) {};

	// 获取1个对象所在的world
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CustomReleasing")
		UWorld* GetWorld_BuffTarget(AActor* InOwner);

	// 获取某种类型的所有Actor, 有可能会获取失败
	UFUNCTION(BlueprintCallable, Category = "CustomReleasing")
		bool GetAllActorsOfClass(AActor* InOwner, TSubclassOf<AActor> ActorClass, TArray<AActor*>& InArray);
};
