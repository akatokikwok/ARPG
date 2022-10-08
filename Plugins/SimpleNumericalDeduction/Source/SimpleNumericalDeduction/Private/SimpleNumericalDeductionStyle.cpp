// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleNumericalDeductionStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FSimpleNumericalDeductionStyle::StyleInstance = nullptr;

void FSimpleNumericalDeductionStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FSimpleNumericalDeductionStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FSimpleNumericalDeductionStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SimpleNumericalDeductionStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FSimpleNumericalDeductionStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("SimpleNumericalDeductionStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("SimpleNumericalDeduction")->GetBaseDir() / TEXT("Resources"));

	Style->Set("SimpleNumericalDeduction.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FSimpleNumericalDeductionStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FSimpleNumericalDeductionStyle::Get()
{
	return *StyleInstance;
}
