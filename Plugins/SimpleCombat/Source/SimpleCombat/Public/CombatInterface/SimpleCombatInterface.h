// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class USimpleCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLECOMBAT_API ISimpleCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 执行信号处事件.
	UFUNCTION(BlueprintCallable, Category = "Combat")
		virtual void AnimSignal(int32 InSignal) {};
};
