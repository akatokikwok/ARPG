// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Actor/ResidualShadowActor.h"
#include "SimpleCombatBPLibrary.generated.h"
class UUI_ComboCount;

/*
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class SIMPLECOMBAT_API USimpleCombatBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	// 使用计数单例, 构建出连打计数UI并同步执行UI表现
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CombatText_ComboPlay", Category = "Combat", WorldContext = WorldContextObject))
		static void ComboPlay(UObject* WorldContextObject, TSubclassOf<UUI_ComboCount> InClass);

	// 销毁连打计数单例; 目的是为了防止第二次打开插件崩溃
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CombatText_Destroy", Category = "Combat"))
		static void ComboTextDestroy();
	
	/** 负责产生闪避残影的BP接口 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ResidualShadow", Category = "Combat", WorldContext = WorldContextObject))
		static AResidualShadowActor* SpawnResidualShadow(UObject* WorldContextObject, TSubclassOf<AResidualShadowActor>ResidualShadowActorClass, USkeletalMeshComponent* InComponentToCopy, float InZOffset, const FVector& Location, const FRotator& InRot, float InLiftTime = 1.f);
};
