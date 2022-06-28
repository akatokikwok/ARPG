// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleCombatEditor.h"
#include "SimpleCombatEditorStyle.h"
#include "SimpleCombatEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "GameplayTagsSettings.h"
#include "Misc/FileHelper.h"

static const FName SimpleCombatEditorTabName("SimpleCombatEditor");

#define LOCTEXT_NAMESPACE "FSimpleCombatEditorModule"

void FSimpleCombatEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimpleCombatEditorStyle::Initialize();
	FSimpleCombatEditorStyle::ReloadTextures();

	FSimpleCombatEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSimpleCombatEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSimpleCombatEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSimpleCombatEditorModule::RegisterMenus));
}

void FSimpleCombatEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSimpleCombatEditorStyle::Shutdown();

	FSimpleCombatEditorCommands::Unregister();
}

void FSimpleCombatEditorModule::PluginButtonClicked()
{
	/** 首先拿到UE编辑器的GameplayTags自定义设置 */
	if (const UGameplayTagsSettings* MutableDefault = GetMutableDefault<UGameplayTagsSettings>()) {
		// I: 先提出所有的GTag,目前是6个
		TArray<FString> Tags;
		for (auto& Tmp : MutableDefault->GameplayTagList) {
			Tags.Add(Tmp.Tag.ToString());
		}

		/* II: 拼接字符串或文件名行为 */
		//
		FString CPPName = FApp::GetProjectName() + FString(TEXT("TagList"));// 欲构造的文件名字 == 项目名字 拼接 "TagList"
		// MMOARPGTagList.h 的名字构造方法, 放在Source/MMOARPG/MMOARPGTagList.h
		FString TagsProjectFileName = FPaths::GameSourceDir() / FApp::GetProjectName() / CPPName + TEXT(".h");
		// 拼出MMOARPG_API
		FString APIString = FString::Printf(TEXT("%s_API"), FApp::GetProjectName()).ToUpper();

		/* III: 构建TagList.h里的所有代码内容. */
		/// 真正的一页文件被哪些代码填充.
		TArray<FString> TagCodes;
		{
			// Lambda--智能生成枚举函数
			auto SpawnEnumFunctionCode = [&](int32 Index, int32 StartPos)->int32 {
				int32 TagsPos = StartPos;
				FString EnumName = FString::Printf(TEXT("EGamePlayTags%i"), Index);// 规定本行的枚举名字.
				TagCodes.Add(TEXT("/*Enumerations can be generated dynamically from gameplaytag*/"));// 写一行注释
				TagCodes.Add(FString::Printf(TEXT("enum class %s :uint32"), *EnumName));// 写一行类似于 enum class EGamePlayTags114 : uint32

				return TagsPos;// 返回当前位置(行号)
			};

			/* 为了内存对齐, 让枚举对齐, 比如72行, 按32对齐, 则要补足余数, 制作为3个枚举而非2个枚举*/
			float InValue = (float)Tags.Num() / 64.f;
			int32 InTotal = FMath::CeilToInt(InValue);
			/* 开始位置记录. */
			int32 TagsPos = 0;

			/* 文件各行编写*/
			TagCodes.Add(TEXT("//Copyright (C) grb.2022.All Rights Reserved."));// 首行标注一下版权声明.
			TagCodes.Add(TEXT(""));// 空一行
			TagCodes.Add(TEXT("#pragma once"));
			TagCodes.Add(TEXT("#include \"CoreMinimal.h\""));
			TagCodes.Add(TEXT(""));
			TagCodes.Add(TEXT("//The file used to reflect gameplay tags is used. 此页文件是被反射生成而非人为编写"));
			TagCodes.Add(TEXT(""));

			/* 扫描并生成各个枚举 */
			for (int32 i = 0; i < InTotal; ++i) {
				TagsPos = SpawnEnumFunctionCode(i/*第几个枚举*/, TagsPos/* 开始行数*/);
			}
		}

		FFileHelper::SaveStringArrayToFile(TagCodes, *TagsProjectFileName);// 把字符串数据写入这个.h
	}


	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FSimpleCombatEditorModule::PluginButtonClicked()")),
							FText::FromString(TEXT("SimpleCombatEditor.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FSimpleCombatEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSimpleCombatEditorCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSimpleCombatEditorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleCombatEditorModule, SimpleCombatEditor)