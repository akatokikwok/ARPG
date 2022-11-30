// Fill out your copyright notice in the Description page of Project Settings.


#include "HotUpdateHUD.h"
#include "../../UI/HotUpdate/UI_HotUpdateMain.h"

AHotUpdateHUD::AHotUpdateHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_HotUpdateMain> HotUpdateMainBPClass(TEXT("/Game/UI/HotUpdate/UI_HotUpdateMain_BP"));
	HotUpdateMainClass = HotUpdateMainBPClass.Class;
}

void AHotUpdateHUD::BeginPlay()
{
	Super::BeginPlay();
	if (HotUpdateMainClass != nullptr) {
		HotUpdateMain = CreateWidget<UUI_HotUpdateMain>(GetWorld(), HotUpdateMainClass);
// 		HotUpdateMain->AddToPlayerScreen(2u);
		HotUpdateMain->AddToViewport();
	}
}