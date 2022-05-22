// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/SimpleBrowse.h"
#include "Core/SimpleZoom.h"
#include "Core/SimplePanelMove.h"
#include "HallPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AHallPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//
	AHallPlayerController();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//
	virtual void SetupInputComponent() override;

public:
	/** 启用浏览3D Actor. */
	void ExecutionRotateCharacter();
	/** 关闭浏览3D Actor. */
	void StopRotateCharacter_callback();
	/** 重设定目标人物. */
	void ResetTarget(AActor* InTarget);
	/** 执行缩放. */
	void Zoom_callback(float InDeltaTime);

	// 启用 垂直方向上移动视口
	void BeginMove_callback();
	// 关闭 垂直方向上移动视口
	void EndMove_callback();

private:
	SimpleActorAction::FSimpleBrowse SimpleBrowse;// 3D物体浏览器.
	SimpleActorAction::FSimpleZoom SimpleZoom;// 3D物体缩放器.
	SimpleActorAction::FSimplePanelMove SimplePanelMove;// 3D物体垂直观察器.
};
