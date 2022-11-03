#include "Widget/Editor/DeduceAttributeCurveTable.h"
#include "UObject/NumericalAlgorithmCurveObject.h"
#include "CurveEditor.h"
#include "ICurveEditorModule.h"
#include "SCurveEditorPanel.h"
#include "ICurveEditorBounds.h"
#include "Curves/RichCurve.h"
#include "RichCurveEditorModel.h"
#include "Tree/SCurveEditorTree.h"
#include "Tree/CurveEditorTree.h"
#include "Tree/ICurveEditorTreeItem.h"
#include "Tree/SCurveEditorTreeSelect.h"
#include "Tree/SCurveEditorTreePin.h"

#define LOCTEXT_NAMESPACE "FDeduceAttributeCurveTable"

/**
 * 当本标签页被创建弹出的时候需要一个绑定的ID; 可以理解为唯一身份查找标识
 */
struct FSimpleEditorDACTable
{
	static const FName DeduceAttributeCurveTableID;
};
const FName FSimpleEditorDACTable::DeduceAttributeCurveTableID = TEXT("SimpleEditorDACTable");


FSDeduceAttributeCurveTable::FSDeduceAttributeCurveTable()
	: DeduceAttributeDataTables(nullptr)
	, DeduceAttributeDataTable(nullptr)
{

}

void FSDeduceAttributeCurveTable::InitLayout()
{
	// 先取消这个身份的注册(理解为关闭旧的)
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FSimpleEditorDACTable::DeduceAttributeCurveTableID);

	// 注册的时候绑定 生成tab视口callback
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FSimpleEditorDACTable::DeduceAttributeCurveTableID, FOnSpawnTab::CreateRaw(this, &FSDeduceAttributeCurveTable::SpawnTab_CurveAsset))
		.SetDisplayName(LOCTEXT("FSimpleEditorDACTableTitle", "DeduceAttributeCurveTable"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// 开始布置层级; 属于重载符号的链式编程
	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Simple_DeduceAttributeCurve_Layout")
	->AddArea // 先添加一块区域
	(
		FTabManager::NewArea(640, 800) ->Split// 在已有的区域里再切割一块区域
		(
			FTabManager::NewStack() ->AddTab
			(
				FSimpleEditorDACTable::DeduceAttributeCurveTableID, ETabState::OpenedTab
			)
		)
	);

	// 把布局塞进窗口
	FGlobalTabmanager::Get()->RestoreFrom(Layout, TSharedPtr<SWindow>());
}

// 注册曲线编辑器数据 并启动标签页
void FSDeduceAttributeCurveTable::Construct(FDeduceAttributeDataTables& InDeduceAttributeDataTables)
{
	DeduceAttributeDataTables = &InDeduceAttributeDataTables;
	DeduceAttributeDataTable = nullptr;

	// 借助唯一身份启动这个标签视口
	FGlobalTabmanager::Get()->TryInvokeTab(FSimpleEditorDACTable::DeduceAttributeCurveTableID);
}

// 注册曲线编辑器数据 并启动标签页
void FSDeduceAttributeCurveTable::Construct(FDeduceAttributeData& InDeduceAttributeDataTable)
{
	DeduceAttributeDataTable = &InDeduceAttributeDataTable;
	DeduceAttributeDataTables = nullptr;

	// 借助唯一身份启动这个标签视口
	FGlobalTabmanager::Get()->TryInvokeTab(FSimpleEditorDACTable::DeduceAttributeCurveTableID);
}

/** 帮助开发者生成曲线标签页 */
TSharedRef<SDockTab> FSDeduceAttributeCurveTable::SpawnTab_CurveAsset(const FSpawnTabArgs& Args)
{
	CurveEditor = MakeShared<FCurveEditor>();
	CurveEditor->GridLineLabelFormatXAttribute = LOCTEXT("GridXLabelFormat", "{0}");

	// 边界裁剪
	TUniquePtr<ICurveEditorBounds> EditorBounds = MakeUnique<FStaticCurveEditorBounds>();
	EditorBounds->SetInputBounds(-1.05, 1.05);

	// SCurveEditorTree资源
	CurveEditorPanel = SNew(SCurveEditorPanel, CurveEditor.ToSharedRef())
		.TreeContent()
		[
			SNew(SCurveEditorTree, CurveEditor)
		];

	
	TSharedRef<SDockTab> NewDockTab = 
		SNew(SDockTab).Icon(FEditorStyle::GetBrush("CurveAssetEditor.Tabs.Properties"))
		[
			SNew(SBorder).BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder")).Padding(0.0f)
			[
				CurveEditorPanel.ToSharedRef()
			]
		];

	return NewDockTab;
}

#undef LOCTEXT_NAMESPACE