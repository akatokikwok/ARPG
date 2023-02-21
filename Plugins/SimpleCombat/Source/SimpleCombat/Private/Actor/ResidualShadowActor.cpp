// Fill out your copyright notice in the Description page of Project Settings.
#include "Actor/ResidualShadowActor.h"
#include "Components/PoseableMeshComponent.h"

// Sets default values
AResidualShadowActor::AResidualShadowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ResidualShadowComponent = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("ResidualShadowComponent"));
	RootComponent = ResidualShadowComponent;

}

// Called when the game starts or when spawned
void AResidualShadowActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResidualShadowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AResidualShadowActor::UpdateResidualShadow(USkeletalMeshComponent* InComponentToCopy)
{
	// 使用PoseableMeshComp这种类型的comp来拷贝保存当前骨骼的姿势与动画.
	// 可以临时性的保存骨骼mesh的一个动作
	ResidualShadowComponent->CopyPoseFromSkeletalComponent(InComponentToCopy);
}