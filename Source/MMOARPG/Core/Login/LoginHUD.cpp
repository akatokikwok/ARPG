// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginHUD.h"
#include "../../UI/Login/UI_LoginMain.h"

ALoginHUD::ALoginHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_LoginMain> LoginMainBPClass(TEXT("/Game/UI/Login/UI_LoginMain_BP"));
	LoginMainClass = LoginMainBPClass.Class;
}

void ALoginHUD::BeginPlay()
{
	Super::BeginPlay();
	if (LoginMainClass != nullptr) {
		LoginMain = CreateWidget<UUI_LoginMain>(GetWorld(), LoginMainClass);
// 		LoginMain->AddToPlayerScreen(2u);
		LoginMain->AddToViewport();
	}

}
