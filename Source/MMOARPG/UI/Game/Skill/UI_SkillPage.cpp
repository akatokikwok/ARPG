#include "UI_SkillPage.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Blueprint/DragDropOperation.h"
#include "../../../Core/Game/MMOARPGGameState.h"
#include "../../../Core/Game/Character/Core/MMOARPGCharacterBase.h"
#include "Abilities/GameplayAbility.h"
#include "Element/UI_SkillSlot.h"
#include "../../../Core/Game/MMOARPGPlayerController.h"
#include "../../../Core/Game/MMOARPGHUD.h"

void UUI_SkillPage::NativeConstruct()
{
	Super::NativeConstruct();
	// 以一组技能标签来构建外观布局
	//LayoutSlot(TArray<FName>());

	if (AMMOARPGPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AMMOARPGPlayerController>()) {
		// 为委托"更新技能表(Page)" 注册回调
		InPlayerController->UpdateSkillTableDelegate.BindUObject(this, &UUI_SkillPage::UpdateSkillTable);

		//
		InPlayerController->InputComponent->BindAction(TEXT("SkillMenu"), IE_Pressed, this, &UUI_SkillPage::OnClickedWidget);
	}
	
	//
	CloseButton->OnPressed.AddDynamic(this, &UUI_SkillPage::OnClose);

	/** 在技能页里有2个HUD的公共事件(用来联系技能页与技能横框), 绑定回调 */
	if (AMMOARPGHUD* InMMOARPGHUD = UUI_Base::GetHUD<AMMOARPGHUD>()) {
		InMMOARPGHUD->BorderHeightDisplayDelegate.AddUObject(this, &UUI_SkillPage::SetBorderHeight);
		InMMOARPGHUD->ResetHeightDisplayDelegate.AddUObject(this, &UUI_SkillPage::ResetBorderHeight);
	}
}

void UUI_SkillPage::LayoutSlot(const TArray<FName>& InKeys)
{
	SlotArrayInventory->ClearChildren();
	if (/*SlotArrayInventory->GetChildrenCount() == 0 && */ SkillSlotClass != nullptr) {
		if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>()) {
			if (AMMOARPGCharacterBase* InCharacterBase = GetWorld()->GetFirstPlayerController()->GetPawn<AMMOARPGCharacterBase>()) {
				
				TArray<FCharacterSkillTable*> SkillTableRows;
				int32 CharacterID = InCharacterBase->GetID();
				if (InGameState->GetCharacterSkillsTables(CharacterID, SkillTableRows)) {/* 若提取人物基类的所有技能信息 并操作成功*/
					
					TArray<FCharacterSkillTable*> AllowSkillTableRows;// 本身不具有的技能s
					for (auto& AnySkillNameTmp : InKeys) {
						for (auto& SkillTmp : SkillTableRows) {
							if (UGameplayAbility* InGameplayAbility = Cast<UGameplayAbility>(SkillTmp->GameplayAbility->GetDefaultObject())) {
								/* 本质上就是 给定的一组名字 与 人物所有技能名字作相同比对,并提取出共有的技能 */
								if (AnySkillNameTmp.ToString() == InGameplayAbility->AbilityTags.ToStringSimple()) {
									AllowSkillTableRows.Add(SkillTmp);
									break;// 只要找到了1个就可以停止扫描所有技能行了
								}
							}
						}
					}

					int32 RowNumber = 20;
					int32 ColumNumber = 3;

					int32 Index = 0;
					for (int32 MyRow = 0; MyRow < RowNumber; MyRow++) {
						for (int32 MyColum = 0; MyColum < ColumNumber; MyColum++) {
							if (UUI_SkillSlot* SlotWidget = CreateWidget<UUI_SkillSlot>(GetWorld(), SkillSlotClass)) {// 构建出SkillSlot
								if (UUniformGridSlot* GridSlot = SlotArrayInventory->AddChildToUniformGrid(SlotWidget)) {
									GridSlot->SetColumn(MyColum);
									GridSlot->SetRow(MyRow);
									GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
									GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
								}

								// 初始化表
								if (AllowSkillTableRows.IsValidIndex(Index)) {
									if (AllowSkillTableRows[Index]->GameplayAbility) {
										if (UGameplayAbility* InGameplayAbility = Cast<UGameplayAbility>(AllowSkillTableRows[Index]->GameplayAbility->GetDefaultObject())) {// 技能信息行里的技能若确认存在
											FString TagName = InGameplayAbility->AbilityTags.ToStringSimple();
											SlotWidget->Update(*TagName, AllowSkillTableRows[Index]->Icon, 0.f, AllowSkillTableRows[Index]->SkillType);
										}
									}
								}

								Index++;
							}
						}
					}
				}
			}
		}
	}
}

void UUI_SkillPage::UpdateSkillTable(const TArray<FName>& InSkillTags)
{
	LayoutSlot(InSkillTags);
}

void UUI_SkillPage::CallSKillSlot(TFunction<bool(UUI_SkillSlot*)> InFunction)
{
	if (SlotArrayInventory) {
		for (int32 i = 0; i < SlotArrayInventory->GetChildrenCount(); i++) {
			if (InFunction(Cast<UUI_SkillSlot>(SlotArrayInventory->GetChildAt(i)))) {
				break;
			}
		}
	}
}

void UUI_SkillPage::OnClickedWidget()
{
	if (GetVisibility() == ESlateVisibility::Visible) {
		OnClose();
	}
	else {
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UUI_SkillPage::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_SkillPage::SetBorderHeight(EMMOARPGSkillType InSkillType)
{
	CallSKillSlot([&](UUI_SkillSlot* InSkillSlot)->bool {
		if (InSkillSlot) {
			if (InSkillSlot->GetSlotInfo().SkillType == InSkillType ||
				!InSkillSlot->GetSlotInfo().IsVaild()) {
				InSkillSlot->SetVisibilityBorderHeight(true);
				InSkillSlot->SetIsEnabled(true);
			}
			else {
				InSkillSlot->SetVisibilityBorderHeight(false);
				//InSkillSlot->SetIsEnabled(false);
			}
		}

		return false;
		});
}

void UUI_SkillPage::ResetBorderHeight()
{
	CallSKillSlot(
		[&](UUI_SkillSlot* InSkillSlot) {
			if (InSkillSlot) {
				InSkillSlot->SetVisibilityBorderHeight(false);
				InSkillSlot->SetIsEnabled(true);
			}

			return false;
		});
}
