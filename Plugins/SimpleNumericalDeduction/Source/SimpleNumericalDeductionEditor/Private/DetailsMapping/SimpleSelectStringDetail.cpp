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

	// 0. 拿到Key
	SelectKey = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleSelectString, SelectString));

	/** 先检测下拉菜单变量的handle; 下拉菜单里关联的SelectStrings是主角、配角、蘑菇怪的表名 */
	if (TSharedPtr<IPropertyHandle> StringsArrayPropertyHandle = 
			PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSimpleSelectString, SelectStrings))) {
		// 1.转成Array形态的句柄
		if (TSharedPtr<IPropertyHandleArray> ArrayHandle = StringsArrayPropertyHandle->AsArray()) {
			// 入参字段的提取数组;
			TArray<FString> MyStrings;
			
			// 2. 扫描 提取出的Array Handle里的所有元素并注入 MyStrings
			uint32 Number = 0;
			ArrayHandle->GetNumElements(Number);
			for (uint32 i = 0; i < Number; i++) {
				if (TSharedPtr<IPropertyHandle> StringElementHandle = ArrayHandle->GetElement(i)) {
					FString& Ele = MyStrings.AddDefaulted_GetRef();
					StringElementHandle->GetValueAsFormattedString(Ele);
				}
			}

			// 3. 扫描入参字段内句柄元素, 制作该builder
			for (size_t i = 0; i < MyStrings.Num(); ++i) {
				FUIAction CharacterAction(
					FExecuteAction::CreateSP(this, &FSimpleSelectStringDetail::HandleCharacterKey, MyStrings[i])
				);

				CharacterAttributeModeBuilder.AddMenuEntry(
					FText::FromString(MyStrings[i]),
					LOCTEXT("HelloTip", "HelloTip"),
					FSlateIcon(),
					CharacterAction
				);
			}
		}
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

// 点击下拉菜单内表名元素的时候的回调方法,会把表名传入
void FSimpleSelectStringDetail::HandleCharacterKey(const FString InKey)
{
	MySelectText = FText::FromString(InKey);
	if (SelectKey.IsValid()) {
		SelectKey->SetValueFromFormattedString(InKey);
	}
}

//
FText FSimpleSelectStringDetail::SelectText() const
{
	return MySelectText;
}

#undef LOCTEXT_NAMESPACE