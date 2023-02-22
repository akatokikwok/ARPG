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
	// 回调,用于注册FOnInitializeCameraShake这个事件
	void InitializeCameraShake(UCameraShakeBase* InCameraShakeBase);

protected:
	// 自定义模式勾选
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock")
		bool bCustomShockClass;

	/** 相机震荡的根模式, (例如)是正弦波振荡，柏林噪声，或FBX动画 */
	UPROPERTY(EditAnywhere, Instanced, Category = "Screen Shock", meta = (EditCondition = "!bCustomShockClass"))
		TObjectPtr<UCameraShakePattern> RootShakePattern;

	// 自定义模式勾选后会启用的 相机震动的tclass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock", meta = (EditCondition = "bCustomShockClass"))
		TSubclassOf<UCameraShakeBase> ShakeClass;

	// 自定义模式勾选后会启用的 震动振幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock", meta = (EditCondition = "bCustomShockClass"))
		float Scale;

	// 自定义模式勾选后会启用的 震动应用在的3D模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock", meta = (EditCondition = "bCustomShockClass"))
		ECameraShakePlaySpace PlaySpace;
	
	// 自定义模式勾选后会启用的 用户自定义的相机震动旋转幅度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Shock", meta = (EditCondition = "bCustomShockClass"))
		FRotator UserPlaySpaceRot;
};
