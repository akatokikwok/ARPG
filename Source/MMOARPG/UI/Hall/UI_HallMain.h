// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_Base.h"
#include "../Common/UI_Print.h"
#include "Element/UI_CharacterCreatePanel.h"
#include "Element/UI_RenameCreate.h"
#include "SimpleNetChannelType.h"
#include "Element/KneadFace/UI_EditorCharacter.h"
#include "UI_HallMain.generated.h"

/**
 * 大厅主界面Widget.
 */
UCLASS()
class MMOARPG_API UUI_HallMain : public UUI_Base
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 屏幕打印日志.
	void PrintLog(const FString& InMsg);
	void PrintLog(const FText& InMsg);

	// 播放Rename控件的淡入淡出动画.
	void PlayRenameIn();
	void PlayRenameOut();

	// 负责还原 Create面板.
	void ResetCharacterCreatePanel(bool bSpawnNewCharacter = true);
	/** 生成最近CA存档关联的舞台人物. */
	void SpawnRecentCharacter();
	/** 使最近存档关联的槽位按钮高亮. */
	void HighlightDefaultSelection();
	
	/** 向服务端核验角色命名 */
	void CheckRename(FString& InCharacterName);
	/** 向服务端发送创建角色的请求. */
	void CreateCharacter(const FMMOARPGCharacterAppearance& InCA);

	// 刷新Rename控件的关联槽位.
	void SetSlotPosition(const int32 InSlotIndex);

	// 删除角色入口.
	void DeleteCharacter(int32 InSlot);

	// 设定正在编辑的槽孔.
	void SetEditCharacter(const FMMOARPGCharacterAppearance* InCA);

	//
	void DestroyCharacter();

	/** 向服务端发送跳转至DS的请求. */
	void JumpDSServer(int32 InSlotID);

protected:
	/** 循环创建与绑定. 绑定客户端的接收. */
	void BindClientRcv();
	/** 重载UI_Base重要方法: RecvProtocol */
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;

	/** 网络消息协议绑定的回调. */
	UFUNCTION()
	void Callback_LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg);

	/** 根据核验结果分别打印提示. */
	void PrintLogByCheckName(ECheckNameType InCheckNameType);

protected:
	// 播放主界面淡入动画.
	void HallMainIn();
	// 播放主界面淡出动画.
	void HallMainOut();

private:
	// 接收代理.
	FDelegateHandle mRecvDelegate;
	// 打印控件.
	UPROPERTY(meta = (BindWidget))
		UUI_Print* UI_Print;
	// 负责创建角色的面板.
	UPROPERTY(meta = (BindWidget))
		UUI_CharacterCreatePanel* UI_CharacterCreatePanel;
	// 负责详细创建舞台人物信息的控件.
	UPROPERTY(meta = (BindWidget))
		UUI_RenameCreate* UI_RenameCreate;
	// 负责删除与编辑的控件.
	UPROPERTY(meta = (BindWidget))
		UUI_EditorCharacter* UI_EditorCharacter;
};
