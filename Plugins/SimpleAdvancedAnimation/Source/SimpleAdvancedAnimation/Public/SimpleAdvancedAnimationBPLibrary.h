// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleAdvancedAnimationType.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SimpleAdvancedAnimationBPLibrary.generated.h"

/* 
* SIMPLEADVANCEDANIMATION_API �걩¶�������ⲿ.
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
*
*/
UCLASS()
class SIMPLEADVANCEDANIMATION_API USimpleAdvancedAnimationBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "SimpleAdvancedAnimation sample test testing"), Category = "SimpleAdvancedAnimationTesting")
	static float SimpleAdvancedAnimationSampleFunction(float Param);

public:
	/** �ֶ����� IK����֮ƥ���Hanle. */
	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
		static int32 CreateFootIK(ACharacter* InCharacter, const TArray<FName>& InBoneNames, EDrawDebugTrace::Type InFootTraceDrawDebugType, float TraceDistance = 50.f, float InterpSpeed = 18.f, float InTraceStart = 50.f);

	/** �ҵ�ָ��Handle �� IK �� ָ�� ���� �� IK�������ƫ��ֵ. */
	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
		static float FindOffset(const int32 InKey, const FName& InKeyName);

	/** �ҵ�ָ��Handle �� IK �� ָ�� ���� �� IK����. */
	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
		static FFootIKInfo FindFootIKInfo(const int32 InKey, const FName& InKeyName);
	
	/** Ӧ�Զ���ĸ������.(�ҳ���С��ƫ��ֵ) */
	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
		static float GetButtZOffset(const TArray<float>& InOffsets);

	/** ���ٸ߼������ܹ��㵥��. */
	UFUNCTION(BlueprintCallable, Category = "SimpleAdvancedAnimation|FootIK")
		static void Destroy();
};
