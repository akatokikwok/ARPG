// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Channel/SimpleChannel.h"
#include "Components/Widget.h"
#include "MMOARPG\MMOARPGGameType.h"
#include "UI_Base.generated.h"

/**
 *
 */
UCLASS()
class MMOARPG_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()
	friend class UUI_LoginMain;
	friend class UUI_Login;
public:

protected:
	// API, 拿取Controller.
	template<typename T>
	T* GetPlayerController()
	{
		return GetWorld() != nullptr ? GetWorld()->GetFirstPlayerController<T>() : nullptr;
	};
	// API,拿取PAWN.
	template<typename T>
	T* GetPawn()
	{
		return GetWorld() != nullptr ?
			GetWorld()->GetFirstPlayerController() != nullptr ? GetWorld()->GetFirstPlayerController()->GetPawn<T>() : nullptr
			: nullptr;
	};
	// API, 拿取GInS.
	template<typename T>
	T* GetGameInstance()
	{
		return GetWorld() != nullptr ? GetWorld()->GetGameInstance<T>() : nullptr;
	};

	// API, 拿取PlayerState.
	template<class T>
	T* GetPlayerState()
	{
		return GetWorld() != nullptr ?
			(GetWorld()->GetFirstPlayerController() != nullptr ? GetWorld()->GetFirstPlayerController()->GetPlayerState<T>() : nullptr) 
				: nullptr;
	}

public:
	// 设定并拿取某一个类型的面板为 当前面板的持有者面板.
	template<class T>
	T* GetParents()
	{
		return Cast<T>(ParentWidget);
	}

	// 设定持有者面板.
	void SetParents(UWidget* InWidget) { ParentWidget = InWidget; }

protected:
	/** 服务器向 UI发送数据后,会激活此函数. */
	virtual void RecvProtocol(uint32 ProtocolNumber/*协议号*/, FSimpleChannel* Channel/*通道*/) {}

	/** 接口: 查找并拿取控件的指定动画,可能返空. */
	UWidgetAnimation* GetNameWidgetAnimation(const FString& InWidgetAnimName);

public:
	/** 接口: 执行播放控件动画, 需指定UMG动画具体名称. */
	void PlayWidgetAnim(const FString& InWidgetName);

public:
	void Print(const FName& InContent);
	void Print(const FText& InContent);
	void Print(const FString& InContent);

protected:
	//
	UPROPERTY()
	UWidget* ParentWidget;

};
