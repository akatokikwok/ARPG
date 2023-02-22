// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotify/AnimNotify_ScreenShock.h"
#include "GameFramework/Character.h"
#include "Camera/CameraShakeBase.h"
#include "Object/Camera/ComboPlayerCameraManager.h"
#include "Object/Camera/ComboCameraShake.h"
#include "PerlinNoiseCameraShakePattern.h"

UAnimNotify_ScreenShock::UAnimNotify_ScreenShock(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, bCustomShockClass(false)// 不勾选自定义模式
	, Scale(1.f)// 振幅倍率
	, PlaySpace(ECameraShakePlaySpace::CameraLocal)// 播放坐标空间
	, UserPlaySpaceRot(FRotator::ZeroRotator)
{
	RootShakePattern = static_cast<UPerlinNoiseCameraShakePattern*>(ObjectInitializer.CreateDefaultSubobject(
		this,
		TEXT("RootShakePattern"),
		UPerlinNoiseCameraShakePattern::StaticClass(),
		nullptr,	// No class to create by default
		false,		// Not required
		false		// Not transient
		));
}

FString UAnimNotify_ScreenShock::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_ScreenShock::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
		if (InCharacter->GetWorld() && !InCharacter->GetWorld()->IsNetMode(ENetMode::NM_DedicatedServer)) {// 仅在客户端
			if (APlayerController* InPlayerController = Cast<APlayerController>(InCharacter->GetController())) {
				if (InPlayerController->PlayerCameraManager != nullptr) {// 拿到相机manager
					if (!bCustomShockClass) {/* 未启用自定义震动模式 */
						
						// 确保设定了震荡根模式(例如)是正弦波振荡，柏林噪声，或FBX动画
						if (!RootShakePattern) {
							return;
						}
						// 
						if (AComboPlayerCameraManager* ComboCamManager = Cast<AComboPlayerCameraManager>(InPlayerController->PlayerCameraManager)) {
							if (UCameraShakeBase* InShakeBase = ComboCamManager->PlayCameraShake(
								UComboCameraShake::StaticClass(),
								FOnInitializeCameraShake::CreateUObject(this, &UAnimNotify_ScreenShock::InitializeCameraShake),
								Scale,
								PlaySpace,
								UserPlaySpaceRot)) {

							}
						}
					}
					else {/* 启用了自定义震动模式. */
						if (UCameraShakeBase* InShakeBase =
							InPlayerController->PlayerCameraManager->StartCameraShake(ShakeClass, Scale, PlaySpace, UserPlaySpaceRot)) {// 这种情况则 播放普通通用的相机震动

						}
					}
				}
			}
		}
	}
}

void UAnimNotify_ScreenShock::InitializeCameraShake(UCameraShakeBase* InCameraShakeBase)
{
	if (InCameraShakeBase) {
		InCameraShakeBase->SetRootShakePattern(RootShakePattern);
	}
}