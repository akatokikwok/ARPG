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
 * Curve数据模型的各组成部分(如左半屏的Label, Select, 右半屏曲线上的Pin点)
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
	// 覆写 创建曲线的model实例(可以让本treeitem去调用并创建出曲线); 本质上而言这个方法是一个提取曲线Model名称 Color属性的过程
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
	/// 这一块分别是左半屏的Label和右半屏Pin点

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

/** 为单属性的每个推导值关联曲线的富文本信息 */
void ConstructDeduceAttribute(FRichCurveEditInfo& InCurveEditInfo, FDeduceAttributeData* InDeduceAttributeData)
{
	if (InDeduceAttributeData) {
		InCurveEditInfo.CurveName = InDeduceAttributeData->Key;// 把诸如名字 血 蓝之类的赋过去
		InCurveEditInfo.CurveToEdit = new FRichCurve();
		InCurveEditInfo.CurveToEdit->SetDefaultValue(0.f);
		// 将每条单属性里的推导值 挨个儿与曲线EditInfo关联
		for (int32 i = 0; i < InDeduceAttributeData->DeduceValue.Num(); i++) {
			InCurveEditInfo.CurveToEdit->UpdateOrAddKey(i, FCString::Atof(*InDeduceAttributeData->DeduceValue[i]));
		}
	}
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
	
	/// Lambda封装: 处理单属性或者多属性都使用到的 解析到曲线逻辑
	auto CreateMyCurves = [&](UNumbericalAlgorithmCurveObject* InCurve) ->void {
		check(CurveEditor.IsValid());
		// 若曲线对象本身含有富文本数据, 扫描所有的富文本
		if (InCurve->HasRichCurves()) {
			for (const FRichCurveEditInfo& CurveData : InCurve->GetCurves()) {
				// 1. 画布上生成1个TreeItem并填充初始化
				TSharedPtr<FDACAssetEditorTreeItem> TreeItem = MakeShared<FDACAssetEditorTreeItem>(InCurve, CurveData);
				FCurveEditorTreeItem* NewItem = CurveEditor->AddTreeItem(FCurveEditorTreeItemID::Invalid());
				NewItem->SetStrongItem(TreeItem);

				// 2. 使用上一步加工的TreeItem调用其内部曲线
				for (const FCurveModelID& CurveModel : NewItem->GetOrCreateCurves(CurveEditor.Get())) {
					CurveEditor->PinCurve(CurveModel);
				}
			}
		}
	};

	/// 生成单个曲线--处理单属性数据
	if (DeduceAttributeDataTable) {
		// 把曲线U对象 New一份
		if (UNumbericalAlgorithmCurveObject* Curve = NewObject<UNumbericalAlgorithmCurveObject>()) {
			{
				// 组一份富文本曲线info
				FRichCurveEditInfo EditInfo;
				// 为单属性的每个推导值关联曲线的富文本信息
				::ConstructDeduceAttribute(EditInfo, DeduceAttributeDataTable);
				// 该曲线info被添加至数组
				Curve->AddCurves(EditInfo);

				// Lambda解析逻辑调用
				CreateMyCurves(Curve);
			}
		}
	}
	/// 生成多个曲线--处理多属性数据
	else if (DeduceAttributeDataTables) {
		// 把曲线U对象 New一份
		if (UNumbericalAlgorithmCurveObject* Curve = NewObject<UNumbericalAlgorithmCurveObject>()) {
			for (auto& Tmp : DeduceAttributeDataTables->AttributeDatas) {
				// 组一份富文本曲线info
				FRichCurveEditInfo EditInfo;
				::ConstructDeduceAttribute(EditInfo, &Tmp);
				// 该曲线info被添加至数组
				Curve->AddCurves(EditInfo);
			}
			// Lambda解析逻辑调用
			CreateMyCurves(Curve);
		}
	}

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