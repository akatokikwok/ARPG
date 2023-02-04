// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleRuntimeGamePrintingType.h"
#include "SimpleRuntimeGamePrintingBPLibrary.generated.h"
class UWorld;
class UUI_GamePrintf;

/* 
* GamePrint空间的BP Library
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
class SIMPLERUNTIMEGAMEPRINTING_API USimpleRuntimeGamePrintingBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	//
	static bool GamePrint(
		UWorld* InWorld,
		TSubclassOf<UUI_GamePrintf> GamePrintfClass,
		const FText& InSlot,
		float InDieDelayTime = 7.0f);

	// GamePrint, 需要一个绘制空间文本
	static bool GamePrint(
		UWorld* InWorld,
		TSubclassOf<UUI_GamePrintf> GamePrintfClass,
		const SimpleRuntimeGamePrinting::FPrintSlot& InSlot,
		float InDieDelayTime = 7.0f);

	//
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GamePrintText", Category = "SRGP", WorldContext = WorldContextObject))
		static bool K2_GamePrintText(
		UObject* WorldContextObject,
		TSubclassOf<UUI_GamePrintf> GamePrintfClass,
		const FText& InText,
		float InDieDelayTime = 7.0f);

	//
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GamePrint", Category = "SRGP", WorldContext = WorldContextObject))
		static bool K2_GamePrint(
		UObject* WorldContextObject,
		TSubclassOf<UUI_GamePrintf> GamePrintfClass,
		const TArray<FSimplePrintSlot>& InSlot,
		float InDieDelayTime = 7.0f);
};
