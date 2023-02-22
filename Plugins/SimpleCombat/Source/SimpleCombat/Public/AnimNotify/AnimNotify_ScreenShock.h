// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Camera/CameraTypes.h"
#include "AnimNotify_ScreenShock.generated.h"

class UCameraShakeBase;
class UCameraShakePattern;
/**
 *
 */
UCLASS(meta = (DisplayName = "ScreenShock"))
class SIMPLECOMBAT_API UAnimNotify_ScreenShock : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_ScreenShock(const FObjectInitializer& ObjectInitializer);
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	void InitializeCameraShake(UCameraShakeBase* InCameraShakeBase);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock")
		bool bCustomShockClass;

	/** The root pattern for this camera shake */
	UPROPERTY(EditAnywhere, Instanced, Category = "Screen Shock", meta = (EditCondition = "!bCustomShockClass"))
		TObjectPtr<UCameraShakePattern> RootShakePattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock", meta = (EditCondition = "bCustomShockClass"))
		TSubclassOf<UCameraShakeBase> ShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock", meta = (EditCondition = "bCustomShockClass"))
		float Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock", meta = (EditCondition = "bCustomShockClass"))
		ECameraShakePlaySpace PlaySpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock", meta = (EditCondition = "bCustomShockClass"))
		FRotator UserPlaySpaceRot;
};
