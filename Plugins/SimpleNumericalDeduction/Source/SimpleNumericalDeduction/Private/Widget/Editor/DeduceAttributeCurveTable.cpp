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

}

void FSDeduceAttributeCurveTable::Construct(FDeduceAttributeDataTables& InDeduceAttributeDataTables)
{
	DeduceAttributeDataTables = &InDeduceAttributeDataTables;
	DeduceAttributeDataTable = nullptr;

	// 借助唯一身份启动这个标签视口
	FGlobalTabmanager::Get()->TryInvokeTab(FSimpleEditorDACTable::DeduceAttributeCurveTableID);
}

void FSDeduceAttributeCurveTable::Construct(FDeduceAttributeData& InDeduceAttributeDataTable)
{
	DeduceAttributeDataTable = &InDeduceAttributeDataTable;
	DeduceAttributeDataTables = nullptr;

	// 借助唯一身份启动这个标签视口
	FGlobalTabmanager::Get()->TryInvokeTab(FSimpleEditorDACTable::DeduceAttributeCurveTableID);
}

TSharedRef<SDockTab> FSDeduceAttributeCurveTable::SpawnTab_CurveAsset(const FSpawnTabArgs& Args)
{

}

#undef LOCTEXT_NAMESPACE