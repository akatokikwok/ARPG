#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

/**
 * 用于面板映射的一个自定义SelectString结构
 */
class FSimpleSelectStringDetail : public IPropertyTypeCustomization
{
public:
	/*
	* 当属性的标题（显示属性的详细信息面板中的行）时调用
	* 如果行中未添加任何内容，则不会显示标题。
	* @param PropertyHandle 正在自定义的属性句柄
	* @param HeaderRow 可以添加小部件的行
	* @param CustomizationUtils 自定义实用程序
	*/
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	/**
	 * 当应自定义属性的子级或添加额外的行时调用
	 * @param PropertyHandle 正在自定义的属性句柄
	 * @param HeaderRow 可以添加小部件的行
	 * @param CustomizationUtils 自定义实用程序
	 */
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	/** 获取面板映射实例化 */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

public:
	//
	FText MySelectText;
	
	//
	TSharedPtr<IPropertyHandle> SelectKey;
};