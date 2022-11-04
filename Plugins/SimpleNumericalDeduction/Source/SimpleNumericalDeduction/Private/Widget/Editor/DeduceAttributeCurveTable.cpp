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
//////////////////////////////////////////////////////////////////////////

/**
 * 各列(Label, Select, Pin)
 */
struct FSimpleEditorColumnNames
{
	static const FName Label;
	static const FName Select;
	static const FName Pin;
};
const FName FSimpleEditorColumnNames::Label = TEXT("Label");
const FName FSimpleEditorColumnNames::Select = TEXT("Select");
const FName FSimpleEditorColumnNames::Pin = TEXT("Pin");
//////////////////////////////////////////////////////////////////////////

#pragma region 用于曲线编辑器的TreeItem结构体
/**
 * 用于曲线编辑器的TreeItem
 */
struct FDACAssetEditorTreeItem : public ICurveEditorTreeItem
{
	// 带参构造器
	FDACAssetEditorTreeItem(TWeakObjectPtr<UCurveBase> InCurveOwner, const FRichCurveEditInfo& InEditInfo);

public:
	// 覆写 构建treewidget的虚方法
	virtual TSharedPtr<SWidget> GenerateCurveEditorTreeWidget(const FName& InColumnName, TWeakPtr<FCurveEditor> InCurveEditor, FCurveEditorTreeItemID InTreeItemID, const TSharedRef<ITableRow>& TableRow) override;
	// 覆写 创建曲线的model
	virtual void CreateCurveModels(TArray<TUniquePtr<FCurveModel>>& OutCurveModels) override;
private:
	TWeakObjectPtr<UCurveBase> CurveOwner;// 具体的曲线
	FRichCurveEditInfo EditInfo;// 富文本内容
	FText CurveName;// 曲线名称
	FLinearColor CurveColor;// 曲线颜色
};

/** 带参构造器 */
FDACAssetEditorTreeItem::FDACAssetEditorTreeItem(TWeakObjectPtr<UCurveBase> InCurveOwner, const FRichCurveEditInfo& InEditInfo)
	: CurveOwner(InCurveOwner)
	, EditInfo(InEditInfo)
{
	if (CurveOwner.IsValid()) {
		CurveName = FText::FromName(EditInfo.CurveName);// 曲线名字由富文本内容初始化
		CurveColor = FLinearColor(FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), 1.f);// 曲线颜色初始化
		//CurveOwner->GetCurveColor(EditInfo);
	}
}

/** 覆写 构建treewidget的虚方法 */
TSharedPtr<SWidget> FDACAssetEditorTreeItem::GenerateCurveEditorTreeWidget(const FName& InColumnName, TWeakPtr<FCurveEditor> InCurveEditor, FCurveEditorTreeItemID InTreeItemID, const TSharedRef<ITableRow>& TableRow)
{
	// 按列名称种类划分各自的构造逻辑.
	if (InColumnName == FSimpleEditorColumnNames::Label) {
		return SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().Padding(FMargin(4.f)).VAlign(VAlign_Center).HAlign(HAlign_Right).AutoWidth()
			[
				SNew(STextBlock).Text(CurveName).ColorAndOpacity(FSlateColor(CurveColor))
			];
	}
	else if (InColumnName == FSimpleEditorColumnNames::Select) {
		return SNew(SCurveEditorTreeSelect, InCurveEditor, InTreeItemID, TableRow);
	}
	else if (InColumnName == FSimpleEditorColumnNames::Pin) {
		return SNew(SCurveEditorTreePin, InCurveEditor, InTreeItemID, TableRow);
	}

	return nullptr;
}

/**  覆写 创建曲线的model */
void FDACAssetEditorTreeItem::CreateCurveModels(TArray<TUniquePtr<FCurveModel>>& OutCurveModels)
{
	if (!CurveOwner.IsValid()) {
		return;
	}
	// 先创建并定义1个富文本Model的uniqueptr
	TUniquePtr<FRichCurveEditorModelRaw> NewCurve = MakeUnique<FRichCurveEditorModelRaw>(static_cast<FRichCurve*>(EditInfo.CurveToEdit), CurveOwner.Get());
	NewCurve->SetShortDisplayName(CurveName);
	NewCurve->SetColor(CurveColor);
	// CurveModel被存到数组里
	OutCurveModels.Add(MoveTemp(NewCurve));
}
//////////////////////////////////////////////////////////////////////////
#pragma endregion 用于曲线编辑器的TreeItem结构体

#pragma region 曲线Table编辑器
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
#pragma endregion 曲线Table编辑器

#undef LOCTEXT_NAMESPACE