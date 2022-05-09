// Fill out your copyright notice in the Description page of Project Settings.


#include "HallHUD.h"
#include "../../UI/Hall/UI_HallMain.h"

AHallHUD::AHallHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_HallMain> HallMainBPClass(TEXT("/Game/UI/Hall/UI_HallMain_BP"));
	HallMainClass = HallMainBPClass.Class;
}

void AHallHUD::BeginPlay()
{
	Super::BeginPlay();
	if (HallMainClass != nullptr) {
		HallMain = CreateWidget<UUI_HallMain>(GetWorld(), HallMainClass);
// 		HallMain->AddToPlayerScreen(2u);
		HallMain->AddToViewport();
	}

}
