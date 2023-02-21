// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResidualShadowActor.generated.h"

class UPoseableMeshComponent;
class USkeletalMeshComponent;

/**
 * 闪避残影留下的actor.
 * 使用了UPoseableMeshComponent这种组件
 */
UCLASS()
class SIMPLECOMBAT_API AResidualShadowActor : public AActor
{
	GENERATED_BODY()

protected:
	// 允许蓝图保存与驱动骨骼动画的组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ResidualShadow", meta = (AllowPrivateAccess = "true"))
		UPoseableMeshComponent* ResidualShadowComponent;

public:
	// Sets default values for this actor's properties
	AResidualShadowActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 刷新闪避残影(需要指定1个骨骼)
	void UpdateResidualShadow(USkeletalMeshComponent* InComponentToCopy);
};
