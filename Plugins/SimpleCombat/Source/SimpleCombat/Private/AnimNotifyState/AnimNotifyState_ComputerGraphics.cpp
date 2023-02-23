// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotifyState/AnimNotifyState_ComputerGraphics.h"
#include "SimpleComboType.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

#if WITH_EDITOR
#include "AnimationEditorPreviewActor.h"
#endif

UAnimNotifyState_ComputerGraphics::UAnimNotifyState_ComputerGraphics()
	: bControllerRotation(false)
	, bCameraHoming(true)
	, bPlayCameraAnim(false)
{
	FieldOfView = 90.0f;
	AspectRatio = 1.777778f;
	OrthoWidth = 512.0f;
	OrthoNearClipPlane = 0.0f;
	OrthoFarClipPlane = WORLD_MAX;
	bConstrainAspectRatio = false;
	bUseFieldOfViewForLOD = true;
	PostProcessBlendWeight = 1.0f;
	ProjectionMode = ECameraProjectionMode::Perspective;
}

void UAnimNotifyState_ComputerGraphics::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* InCharacter = Cast<AActor>(MeshComp->GetOuter())) {
		// 仅在客户端 或者是编辑器开启调试的时候 生效
		if (InCharacter->GetLocalRole() == ENetRole::ROLE_AutonomousProxy
		#if WITH_EDITOR
			|| Cast<AAnimationEditorPreviewActor>(MeshComp->GetOuter())
		#endif
			) {

			// Spawn参数
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// 相机位置 = 人坐标加一段偏移向量
			FVector CamaraPos = InCharacter->GetActorLocation() + RelativePos;
			// 若不想使用自己控制的controller去直接控制旋转, 则需要自己计算一套逻辑; 让相机看向人
			FRotator NewRot = PreRotation;
			if (!bControllerRotation) {
				FVector ViewPos = CameraViewOffsetValue + InCharacter->GetActorLocation();
				// 对应蓝图里的API : FindLookAtRotation; 用人视角位置减去相机位置
				NewRot = FRotationMatrix::MakeFromX(ViewPos - CamaraPos).Rotator();
			}

			// 观察视角跳转至场景生成相机.
			if (ACameraActor* InSpawnedCamera = InCharacter->GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), CamaraPos, NewRot, SpawnParameters)) {
				// 设定相机参数
				{
					InSpawnedCamera->SetLifeSpan(TotalDuration);// actor存续时长
					InSpawnedCamera->GetCameraComponent()->SetFieldOfView(FieldOfView);// FOV
					InSpawnedCamera->GetCameraComponent()->SetOrthoWidth(OrthoWidth);// 正交投影宽
					InSpawnedCamera->GetCameraComponent()->SetOrthoNearClipPlane(OrthoNearClipPlane);// 近裁剪面
					InSpawnedCamera->GetCameraComponent()->SetOrthoFarClipPlane(OrthoFarClipPlane);// 远裁剪面
					InSpawnedCamera->GetCameraComponent()->SetAspectRatio(AspectRatio);// AspectRatio
					InSpawnedCamera->GetCameraComponent()->SetConstraintAspectRatio(bConstrainAspectRatio);// 限制ratio开关
					InSpawnedCamera->GetCameraComponent()->SetUseFieldOfViewForLOD(bUseFieldOfViewForLOD);// LOD开关
					InSpawnedCamera->GetCameraComponent()->SetProjectionMode(ProjectionMode);// 透视投影
					InSpawnedCamera->GetCameraComponent()->PostProcessSettings = MyPostProcessSettings;// 后处理设定集
				}
				// 跳转至场景内上文生成的预设相机
				if (ACharacter* InMyCharacter = Cast<ACharacter>(InCharacter)) {
					if (APlayerController* InController = Cast<APlayerController>(InMyCharacter->GetController())) {
						InController->SetViewTargetWithBlend(InSpawnedCamera, bPlayCameraAnim ? TotalDuration : 0.f);
					}
				}
			}
		}
	}
}

void UAnimNotifyState_ComputerGraphics::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UAnimNotifyState_ComputerGraphics::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// 启用相机复位,复位至人身上,再次观察人
	if (bCameraHoming) {
		if (ACharacter* InCharacter = Cast<ACharacter>(MeshComp->GetOuter())) {
			if (InCharacter->GetLocalRole() == ENetRole::ROLE_AutonomousProxy) {
				if (APlayerController* InController = Cast<APlayerController>(InCharacter->GetController())) {
					InController->SetViewTargetWithBlend(InCharacter, 0.f);
				}
			}
		}
	}
}