// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ComputerGraphics.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "ComputerGraphics"))
class SIMPLECOMBAT_API UAnimNotifyState_ComputerGraphics : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_ComputerGraphics();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

public:
	// 预设距离人的偏移(偏移多远到场景相机)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComputerGraphics")
		FVector CameraViewOffsetValue;

	// 用这段偏移值偏移到相机
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComputerGraphics")
		FVector RelativePos;

	// 是否启用控制器旋转权
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComputerGraphics")
		bool bControllerRotation;

	// 近裁剪面
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = CameraSettings)
		float OrthoNearClipPlane;

	// 预设偏移旋转分量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComputerGraphics", meta = (EditCondition = "bControllerRotation"))
		FRotator PreRotation;

	// 是否启用相机复位,复位至人身上
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComputerGraphics")
		bool bCameraHoming;


	// 是否开启相机动画平滑过渡至跳转场景相机
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComputerGraphics")
		bool bPlayCameraAnim;

	// FOV
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = CameraSettings, meta = (UIMin = "5.0", UIMax = "170", ClampMin = "0.001", ClampMax = "360.0", Units = deg))
		float FieldOfView;

	// 正交投影宽
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = CameraSettings)
		float OrthoWidth;

	// 远裁剪面
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = CameraSettings)
		float OrthoFarClipPlane;

	// AspectRatio
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = CameraSettings, meta = (ClampMin = "0.1", ClampMax = "100.0", EditCondition = "bConstrainAspectRatio"))
		float AspectRatio;

	// 限制ratio开关
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = CameraOptions)
		uint8 bConstrainAspectRatio : 1;

	// LOD开关
	UPROPERTY(Interp, EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = CameraOptions)
		uint8 bUseFieldOfViewForLOD : 1;

	// 投影成像模式
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = CameraSettings)
		TEnumAsByte<ECameraProjectionMode::Type> ProjectionMode;

	/** Indicates if PostProcessSettings should be used when using this Camera to view through. */
	UPROPERTY(Interp, EditAnywhere, BlueprintReadWrite, Category = PostProcess, meta = (UIMin = "0.0", UIMax = "1.0"))
		float PostProcessBlendWeight;

	// 后处理设定集
	UPROPERTY(Interp, BlueprintReadWrite, Category = PostProcess, meta = (ShowOnlyInnerProperties))
		struct FPostProcessSettings MyPostProcessSettings;
};
