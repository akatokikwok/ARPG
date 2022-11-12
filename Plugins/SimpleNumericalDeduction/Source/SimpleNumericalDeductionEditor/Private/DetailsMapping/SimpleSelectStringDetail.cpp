#include "DetailsMapping/SimpleSelectStringDetail.h"
#include "DetailWidgetRow.h"
#include "SimpleNumericalDeductionType.h"

#define LOCTEXT_NAMESPACE "FSimpleSelectStringDetail"

/*
* 当属性的标题（显示属性的详细信息面板中的行）时调用
* 如果行中未添加任何内容，则不会显示标题。
* @param PropertyHandle 正在自定义的属性句柄
* @param HeaderRow 可以添加小部件的行
* @param CustomizationUtils 自定义实用程序
*/
void FSimpleSelectStringDetail::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// I. 制作1个builder; 严格意义上此builder即为一堆属性的载体窗口
	FMenuBuilder CharacterAttributeModeBuilder(true, nullptr);
	for (size_t i = 0; i < 10; ++i) {
		FString HelloStr = TEXT("Hello");
		FUIAction CharacterAction(
			FExecuteAction::CreateSP(this, &FSimpleSelectStringDetail::HandleCharacterKey, HelloStr)
		);

		CharacterAttributeModeBuilder.AddMenuEntry(LOCTEXT("Hello", "Hello"),
			LOCTEXT("HelloTip", "HelloTip"),
			FSlateIcon(),
			CharacterAction
		);
	}

	// II. 把Builder构建在HeadRow内
	HeaderRow
	.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()// 先用传入字段自身的名字
	]
	.ValueContent().MinDesiredWidth(125.0f).MaxDesiredWidth(325.0f)
	[
		SNew(SComboButton)// 存放1个下拉框
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &FSimpleSelectStringDetail::SelectText)
		]
		.MenuContent()
		[
			CharacterAttributeModeBuilder.MakeWidget()// 把CharacterAttributeModeBuilder构建在HeadRow内
		]
	];
}

/**
 * 当应自定义属性的子级或添加额外的行时调用
 * @param PropertyHandle 正在自定义的属性句柄
 * @param HeaderRow 可以添加小部件的行
 * @param CustomizationUtils 自定义实用程序
 */
void FSimpleSelectStringDetail::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
// 	Super::CustomizeChildren(PropertyHandle, ChildBuilder, CustomizationUtils);

}

/** 获取面板映射实例化 */
TSharedRef<IPropertyTypeCustomization> FSimpleSelectStringDetail::MakeInstance()
{
	return MakeShareable(new FSimpleSelectStringDetail());
}

// FUIAction会用到的回调方法.
void FSimpleSelectStringDetail::HandleCharacterKey(const FString InKey)
{

}

//
FText FSimpleSelectStringDetail::SelectText() const
{
	return LOCTEXT("xxx", "xxxx");
}

#undef LOCTEXT_NAMESPACE