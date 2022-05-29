#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_MainBase.h"
#include "UI_HallMain.generated.h"
class UUI_CharacterCreatePanel;
class UUI_RenameCreate;
class UUI_EditorCharacter;

/**
 * 大厅主界面Widget.
 */
UCLASS()
class MMOARPG_API UUI_HallMain : public UUI_MainBase
{
	GENERATED_BODY()
private:
	// 负责创建角色的面板.
	UPROPERTY(meta = (BindWidget))
		UUI_CharacterCreatePanel* UI_CharacterCreatePanel;
	// 负责详细创建舞台人物信息的控件.
	UPROPERTY(meta = (BindWidget))
		UUI_RenameCreate* UI_RenameCreate;
	// 负责删除与编辑的控件.
	UPROPERTY(meta = (BindWidget))
		UUI_EditorCharacter* UI_EditorCharacter;

protected:
	/** 重载UI_Base重要方法: RecvProtocol */
	/** 服务器向 UI发送数据后,会激活此函数. */
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) override;

	/** 网络消息协议绑定的回调. */
	void LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg) override;

	/** 根据核验结果分别打印提示. */
	void PrintLogByCheckName(ECheckNameType InCheckNameType);

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

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

	/** 向服务端发送跳转至DS的请求. */
	void JumpDSServer(int32 InSlotID);

	// 删除角色入口.
	void DeleteCharacter(int32 InSlot);

	// 设定正在编辑的槽孔.
	void SetEditCharacter(const FMMOARPGCharacterAppearance* InCA);

	// 移除舞台角色.
	void DestroyCharacter();

protected:
	// 播放主界面淡入动画.
	void HallMainIn();
	// 播放主界面淡出动画.
	void HallMainOut();

};
