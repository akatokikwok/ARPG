#include "Widget/Editor/DebugAttributeDeduce.h"
#include "Log/SimpleNumericalDeductionLog.h"
#include "Widget/Slate/SDebugDataTableAttributeTable.h"

#define LOCTEXT_NAMESPACE "FDebugAttibuteDeduce"

/**
 * 描述Debug推演数据的身份ID
 */
struct FSimpleEditorDADTable
{
	static const FName DebugAttibuteDeduceTableID;
};
const FName FSimpleEditorDADTable::DebugAttibuteDeduceTableID = TEXT("DebugAttibuteDeduce");

FDebugAttibuteDeduce::FDebugAttibuteDeduce()
{

}

void FDebugAttibuteDeduce::InitLayout()
{
	// 解注册
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FSimpleEditorDADTable::DebugAttibuteDeduceTableID);
	// 注册视口
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		FSimpleEditorDADTable::DebugAttibuteDeduceTableID, 
		FOnSpawnTab::CreateRaw(this, &FDebugAttibuteDeduce::SpawnTab_DebugSettingWidget))
			.SetDisplayName(LOCTEXT("FDebugAttibuteDeduceSettingsTitle", "DebugAttibuteDeduceSettingsTable"));
		
	// 构造布局
	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Simple_DeduceSettings_Layout")->AddArea
	(
		FTabManager::NewArea(640, 800)->Split
		(
			FTabManager::NewStack()->AddTab
			(
				FSimpleEditorDADTable::DebugAttibuteDeduceTableID, ETabState::OpenedTab
			)
		)
	);
	// 视口内塞入布局
	FGlobalTabmanager::Get()->RestoreFrom(Layout, TSharedPtr<SWindow>());
}

void FDebugAttibuteDeduce::Construct()
{
	// 借助唯一身份启动这个标签视口
	FGlobalTabmanager::Get()->TryInvokeTab(FSimpleEditorDADTable::DebugAttibuteDeduceTableID);
}

TSharedRef<SDockTab> FDebugAttibuteDeduce::SpawnTab_DebugSettingWidget(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> NewDockTab = SNew(SDockTab)
	.Icon(FEditorStyle::GetBrush("CurveAssetEditor.Tabs.Properties"))
	[
		/** 这一步让视口去关联调试日志编辑器 */
		SNew(SBorder).BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder")).Padding(0.0f)
		[
			SNew(SDebugDataTableAttributeTable)// 视口内 生成一个调试日志系统编辑器
		]
		
	];
	return NewDockTab;
}

#undef LOCTEXT_NAMESPACE