#include "Widget/Editor/DebugAttributeDeduce.h"
#include "Log/SimpleNumericalDeductionLog.h"


#define LOCTEXT_NAMESPACE "FDebugAttibuteDeduce"

FDebugAttibuteDeduce::FDebugAttibuteDeduce()
{

}

void FDebugAttibuteDeduce::InitLayout()
{

}

void FDebugAttibuteDeduce::Construct()
{

}

TSharedRef<SDockTab> FDebugAttibuteDeduce::SpawnTab_DebugSettingWidget(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> NewDockTab = SNew(SDockTab);

	return NewDockTab;
}

#undef LOCTEXT_NAMESPACE