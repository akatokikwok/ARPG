// Copyright Epic Games, Inc. All Rights Reserved.
/**
 * 这张文件可以用来反射出 MMOARPGTagList.h
 */

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

/** 标签组是否启用对齐模式, 可由项目状态决定启用或禁用. */
/* 正式开发技能标签的时候强烈建议设置为非对齐, 允许插入和替换*/
bool bTagsAlignment = true;

/** 对标签组的顺序编排,输出一组新的标签组 */
void SequentialArrangement(const TArray<FString>& InTags, TArray<FString>& OutTags)
{
	// 检测指定的路径文件 "GameplayTagsOrder.ini" 是否存在.
	const FString TagsOrderPaths = FPaths::ProjectConfigDir() / TEXT("GameplayTagsOrder.ini");
	if (IFileManager::Get().FileExists(*TagsOrderPaths)) {
		// 指定文件里的标签组.
		TArray<FString> TmpTags;
		FFileHelper::LoadFileToStringArray(TmpTags, *TagsOrderPaths);

		/** 文件标签组不为空 */
		if (TmpTags.Num() != 0) {

			// 找到有差异的标签.
			TArray<FString> DiffTags;
			/* 一次大的比对逻辑并记录那些删掉的*/
			for (auto& meta1 : TmpTags) {
				// 默认不存在匹配的双方
				bool bExist = false;
				// 让入参标签组和文件内标签组执行 比对; 直至找到匹配的双方
				for (auto& meta2 : InTags) {
					if (meta1 == meta2) {
						bExist = true;
						break;
					}
				}
				// 经历过上一步比对之后,若仍处于不存在状态; 则证明是被删掉了
				if (!bExist) {
					DiffTags.Add(meta1);
				}
			}

			/* 若开启了对齐模式.*/
			if (bTagsAlignment) {
				/* 执行"对齐"; 从文件里移除那些符合条件的元素. */
				for (auto& Element : DiffTags) {
					TmpTags.Remove(Element);
				}
			}
			/* 若对齐模式被禁用 */
			else {
				for (auto& Element : DiffTags) {
					int32 Index = INDEX_NONE;
					if (TmpTags.Find(Element, Index)) {
						TmpTags[Index] = FString::Printf(TEXT("INDEX_NONE_%i"), Index);
					}
				}
			}

			/** 输出标签组设定为 被加工过了的文件标签组 */
			OutTags = TmpTags;

			// 空缺
			TArray<int32> Positions;
			for (auto& TmpTag : OutTags) {
				if (TmpTag.Contains(TEXT("INDEX_NONE_"))) {
					int32 Index = INDEX_NONE;
					if (OutTags.Find(TmpTag, Index)) {
						Positions.AddUnique(Index);
					}
				}
			}

			// 填充行为lambda
			auto Fill = [&](const FString& InTmpTag) ->bool {
				if (Positions.Num()) {
					OutTags[Positions.Pop()] = InTmpTag;
					return true;
				}
				return false;
			};

			/** 在原有的数据基础上再扩展元素. */
			for (auto& Tmp : InTags) {
				if (!OutTags.Contains(Tmp)) {
					// 先尝试添加到空位
					if (!Fill(Tmp)) {
						// 再尝试添加到后面
						OutTags.AddUnique(Tmp);
					}
				}
			}
		}
		/** 文件标签组为空 */
		else {
			OutTags = InTags;
		}
	}
	// 这条路径下不存在这个文件 则把输出标签组直接设置为输入标签组.
	else {
		OutTags = InTags;
	}

	// 直接存储并输出新的标签组; 文件内容被刷新.
	FFileHelper::SaveStringArrayToFile(OutTags, *TagsOrderPaths);
}

void FSimpleCombatEditorModule::PluginButtonClicked()
{
	/** 首先拿到UE编辑器的GameplayTags自定义设置 */
	if (const UGameplayTagsSettings* MutableDefault = GetMutableDefault<UGameplayTagsSettings>()) {
		// I: 先提出所有的GTag,目前是6个
		TArray<FString> TmpTags;
		for (auto& Tmp : MutableDefault->GameplayTagList) {
			TmpTags.Add(Tmp.Tag.ToString());
		}

		/* 重新编排顺序输出一个符合次序的新的标签组. */
		/* 借此来阻止新添加的标签会占据在老标签前面 造成生成的JSON文件内容出错. */
		TArray<FString> Tags;
		SequentialArrangement(TmpTags, Tags);

		/* II: 拼接字符串或文件名行为 */
		//
		FString CPPName = FApp::GetProjectName() + FString(TEXT("TagList"));// 欲构造的文件名字 == 项目名字 拼接 "TagList"
		// MMOARPGTagList.h 的名字构造方法, 放在Engine/Plugins/MMOARPGCommon/Source/MMOARPGCommon/Public里
		FString TagsProjectFileName = FPaths::EnginePluginsDir() / TEXT("MMOARPGCommon/Source/MMOARPGCommon/Public") / CPPName + TEXT(".h");
// 		FString TagsProjectFileName = FPaths::GameSourceDir() / FApp::GetProjectName() / CPPName + TEXT(".h");
		// 拼出MMOARPG_API
		FString APIString = TEXT("");
// 		FString APIString = FString::Printf(TEXT("%s_API"), FApp::GetProjectName()).ToUpper();

		/* III: 构建TagList.h里的所有代码内容. */
		/// 真正的一页文件被哪些代码填充.
		TArray<FString> TagCodes;
		{
			// Lambda--智能生成枚举函数
			auto SpawnEnumFunctionCode = [&](int32 Index, int32 StartPos)->int32 {
				TMap<FName, uint64> TagsMap;// GameTag标签缓存池, 一根标签对应一个十六进制值.
				int32 TagsPos = StartPos;// 记录位置.(行号)
				FString EnumName = FString::Printf(TEXT("EGamePlayTags%i"), Index);// 规定本行的枚举名字.
				TagCodes.Add(TEXT("/* Enumerations can be generated dynamically from gameplaytag. */"));// 写一行注释
				TagCodes.Add(FString::Printf(TEXT("enum class %s : uint64"), *EnumName));// 写一行类似于 enum class EGamePlayTags0 : uint32
				
				TagCodes.Add(TEXT("{"));
				for (int32 i = StartPos; i < Tags.Num(); i++) {
					if (i < (StartPos + 64)) {
						// 未超过32位限制
						
						// 类似 Player.Attack.ComboLinkage 规定转化为  Player_Attack_ComboLinkage
						FString InTags = Tags[i];
						InTags.ReplaceInline(TEXT("."), TEXT("_"));
						uint64 InNewValue_Hex = 1llu << (i - StartPos); // 每六十四个技能满了再进来一次.
						TagCodes.Add(FString::Printf(TEXT("    %s = %llu,"), *InTags, InNewValue_Hex));// %llu是无符号六十四位, %x是十六进制. %i 则可以匹配八进制、十进制、十六进制表示的整数; 这样造成递增移位, 2 4 8 16
						TagsPos++;// 一旦偏移就需要 增加位置.

						TagsMap.Add(*Tags[i], InNewValue_Hex);// 标签缓存池里注册一对pair <技能标签名, 十六进制值>
					}
					else {
						// 超过32限制就中断,让它再进一次.
						break;
					}
				}
				TagCodes.Add(TEXT("};"));
				TagCodes.Add(TEXT(""));// 空一行

				/* part 1*/
				TagCodes.Add(TEXT("/* Convert GamplayTag tag to enumeration */"));// 写一行注释 /* 开始拼函数了.*/
				TagCodes.Add(FString::Printf(TEXT("%s %s NameTo%s(const FName &InName)"), *EnumName, *APIString, *EnumName));
				TagCodes.Add(TEXT("{"));
				for (auto& Tmp : TagsMap) {/* 扫描<GameTag, HEX>缓存池*/
					
					FString EnumValue = Tmp.Key.ToString();
					EnumValue.ReplaceInline(TEXT("."), TEXT("_"));// Player.Attack.ComboLinkage 规定转化为  Player_Attack_ComboLinkage

					TagCodes.Add(FString::Printf(TEXT("	if (InName == TEXT(\"%s\")) {"), *Tmp.Key.ToString()));
					TagCodes.Add(FString::Printf(TEXT("		 return %s::%s;"), *EnumName, *EnumValue));// 拼接出类似于	return EGamePlayTags0::Player_Attack_ComboLinkage
					TagCodes.Add(TEXT("	} else"));
				}
				TagCodes.Last().RemoveFromEnd(TEXT("else"));// 移除最后一个没必要的else
				TagCodes.Add(TEXT(""));
				TagCodes.Add(FString::Printf(TEXT("    return (%s)0llu;"), *EnumName));// 函数有默认返回值.
				TagCodes.Add(TEXT("}"));
				TagCodes.Add(TEXT(""));

				/* part 2*/
				TagCodes.Add(TEXT("/* Convert enumeration to gameplaytag. */"));
				TagCodes.Add(FString::Printf(TEXT("FString %s %sToName(const %s& InTag)"), *APIString, *EnumName, *EnumName));// FString MMOARPG_API EGameplayTags0ToName(const EGameplayTags0& InTag)
				TagCodes.Add(TEXT("{"));
				TagCodes.Add(TEXT("	switch(InTag)"));
				TagCodes.Add(TEXT("	{"));
				for (auto& Tmp : TagsMap) {
					FString EnumValue = Tmp.Key.ToString();
					EnumValue.ReplaceInline(TEXT("."), TEXT("_"));

					TagCodes.Add(FString::Printf(TEXT("		case %s::%s:"), *EnumName, *EnumValue));
					TagCodes.Add(FString::Printf(TEXT("		 return TEXT(\"%s\");"), *Tmp.Key.ToString()));
					TagCodes.Add(TEXT(""));
				}
				TagCodes.Last().RemoveFromEnd(TEXT(""));
				TagCodes.Add(TEXT("	}"));
				TagCodes.Add(TEXT(""));
				TagCodes.Add(TEXT("	return FString();"));
				TagCodes.Add(TEXT("}"));
				TagCodes.Add(TEXT(""));

				/* part 3*/
				TagCodes.Add(TEXT("/* Convert a composite enumeration to the corresponding label. */"));
				TagCodes.Add(FString::Printf(TEXT("void %s NameTo%ss(const FName &InName,TArray<FName> &OutName)"), *APIString, *EnumName, *EnumName));
				TagCodes.Add(TEXT("{"));
				TagCodes.Add(TEXT("	uint64 Tags = FCString::Strtoui64(*InName.ToString(),NULL,10);"));
				for (auto& Tmp : TagsMap) {
					FString EnumValue = Tmp.Key.ToString();
					EnumValue.ReplaceInline(TEXT("."), TEXT("_"));
					TagCodes.Add(FString::Printf(TEXT("	if(Tags & (int32)%s::%s)"), *EnumName, *EnumValue));
					TagCodes.Add(TEXT("	{"));
					TagCodes.Add(FString::Printf(TEXT("		OutName.AddUnique(TEXT(\"%s\"));"), *Tmp.Key.ToString()));
					TagCodes.Add(TEXT("	}"));
				}
				TagCodes.Add(TEXT("}"));
				TagCodes.Add(TEXT(""));

				/* part 4*/
				TagCodes.Add(TEXT("/* Convert multiple enumerations into one label for storage. */"));
				TagCodes.Add(FString::Printf(TEXT("FName %s %ssToName(const TArray<FName> &InName)"), *APIString, *EnumName));
				TagCodes.Add(TEXT("{"));
				TagCodes.Add(TEXT("    uint64 Tags = 0llu;"));
				TagCodes.Add(TEXT("	for(auto &Tmp : InName)"));
				TagCodes.Add(TEXT("	{"));
				for (auto& Tmp : TagsMap) {
					FString EnumValue = Tmp.Key.ToString();
					EnumValue.ReplaceInline(TEXT("."), TEXT("_"));
				
					TagCodes.Add(FString::Printf(TEXT("		if(Tmp == FName(TEXT(\"%s\")))"), *Tmp.Key.ToString()));
					TagCodes.Add(TEXT("		{"));
					TagCodes.Add(FString::Printf(TEXT("			Tags |= (uint64)%s::%s;"), *EnumName, *EnumValue));
					TagCodes.Add(TEXT("		} else"));
				}
				TagCodes.Last().RemoveFromEnd(TEXT("else"));
				TagCodes.Add(TEXT("	}"));
				TagCodes.Add(TEXT(""));
				TagCodes.Add(TEXT("	return *FString::Printf(TEXT(\"%llu\"), Tags);"));// 给一个默认返回值.
				TagCodes.Add(TEXT("}"));

				/* part 5*/
				return TagsPos;// 返回当前位置(行号)
			};
			// End; Lambda--智能生成枚举.
			//////////////////////////////////////////////////////////////////////////

			/* 为了内存对齐, 让枚举对齐, 比如72行, 按32对齐, 则要补足余数, 制作为3个枚举而非2个枚举*/
			float InValue = (float)Tags.Num() / 64.f;
			int32 InTotal = FMath::CeilToInt(InValue);// 枚举总数.
			/* 开始位置记录. */
			int32 TagsPos = 0;

			/* 文件各行编写*/
			TagCodes.Add(TEXT("//Copyright (C) grb.2022.All Rights Reserved."));// 首行标注一下版权声明.
			TagCodes.Add(TEXT(""));// 空一行
			TagCodes.Add(TEXT("#pragma once"));
			TagCodes.Add(TEXT("#include \"CoreMinimal.h\""));
			TagCodes.Add(TEXT(""));
			TagCodes.Add(TEXT("// The file used to reflect gameplay tags is used. 此页文件是被反射生成而非人为编写"));
			TagCodes.Add(TEXT("// 此文件禁止被多个文件include,否则会造成链接错误."));
			TagCodes.Add(TEXT(""));

			/* part 0: 扫描并生成各个枚举 */
			for (int32 i = 0; i < InTotal; ++i) {
				TagsPos = SpawnEnumFunctionCode(i/*第几个枚举*/, TagsPos/* 开始行数*/);
			}
			TagCodes.Add(TEXT(""));

			/* part 1*/
			TagCodes.Add(TEXT("//////////////////////////Main/////////////////////////////"));
			TagCodes.Add(TEXT("/* Convert the server's sequence to a label. */"));
			TagCodes.Add(FString::Printf(TEXT("void %s AnalysisArrayNameToGamePlayTags(const TArray<FName> &InNames,TArray<FName> &OutNames)"), *APIString));
			TagCodes.Add(TEXT("{"));
			for (int32 i = 0; i < InTotal; i++) {
				FString EnumName = FString::Printf(TEXT("EGamePlayTags%i"), i);

				TagCodes.Add(FString::Printf(TEXT("	for(auto &Tmp : InNames)")));
				TagCodes.Add(TEXT("	{"));
				TagCodes.Add(FString::Printf(TEXT("		NameTo%ss(Tmp,OutNames);"), *EnumName));
				TagCodes.Add(TEXT("	}"));
			}
			TagCodes.Add(TEXT("}"));
			TagCodes.Add(TEXT(""));

			/* part 2.*/
			TagCodes.Add(TEXT("/* Convert multiple tags to a sequence stored by the server. */"));
			TagCodes.Add(FString::Printf(TEXT("void %s AnalysisGamePlayTagsToArrayName(const TArray<FName> &InNames,TArray<FName> &OutNames)"), *APIString));
			TagCodes.Add(TEXT("{"));
			for (int32 i = 0; i < InTotal; i++) {
				FString EnumName = FString::Printf(TEXT("EGamePlayTags%i"), i);
				TagCodes.Add(FString::Printf(TEXT("	FName TagName = %ssToName(InNames);"), *EnumName));
				TagCodes.Add(TEXT("	if(TagName != NAME_None)"));
				TagCodes.Add(TEXT("	{"));
				TagCodes.Add(TEXT("		OutNames.AddUnique(TagName);"));
				TagCodes.Add(TEXT("	}"));
			}
			TagCodes.Add(TEXT("}"));
		}

		/* IV: 把字符串数据写入这个.h */
		FFileHelper::SaveStringArrayToFile(TagCodes, *TagsProjectFileName);
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