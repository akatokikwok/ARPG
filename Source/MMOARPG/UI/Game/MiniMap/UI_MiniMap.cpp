

// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_MiniMap.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"

//#include "Core/RuleOfTheGameUserSettings.h"

void UUI_MiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	
}

//Tick
void UUI_MiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//const TMap<FGuid, FCharacterData> &CharacterDatas = GetGameState()->GetCharacterDatas();
	//
	//if (UCanvasPanelSlot *ImagePanelSlot = Cast<UCanvasPanelSlot>(MiniMapImage->Slot))
	//{
	//	const float MaxMiniMapSize		= GLOBAL_MANAGEMENT_MACRO()->GetGlobalConfiguration()->MaxMiniMapSize;
	//	const float MaxTargetArmLength	= GLOBAL_MANAGEMENT_MACRO()->GetGlobalConfiguration()->MaxTargetArmLength;
	//	
	//	FVector2D LocalSize2D = MyGeometry.GetDrawSize();

	//	//地图缩放
	//	FVector2D ZoomSize;
	//	{
	//		float TargetArmLength = Cast<ATowerDefenceGameCamera>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetTargetArmLength();
	//		ZoomSize = FVector2D((MaxMiniMapSize / MaxTargetArmLength) * TargetArmLength);
	//	
	//		ImagePanelSlot->SetSize(ZoomSize);
	//	}

	//	//地图位置
	//	FVector2D MinImageMapOffset;
	//	{
	//		FVector Location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//		MinImageMapOffset.Y = -(ZoomSize.X - (ZoomSize.X / MapSize.BigMapRealSize.X) * Location.X);
	//		MinImageMapOffset.X = -(ZoomSize.Y / MapSize.BigMapRealSize.Y) * Location.Y;
	//	
	//		ImagePanelSlot->SetPosition(MinImageMapOffset);
	//	}

	//	if (!URuleOfTheGameUserSettings::GetRuleOfTheGameUserSettings()->GetRealisticMap())
	//	{
	//		//场景中的角色
	//		for (auto& Tmp : CharacterDatas)
	//		{
	//			if (Tmp.Value.Health > 0.f)
	//			{
	//				if (!IsExistence(Tmp.Key))
	//				{
	//					if (UImage* Point = NewObject<UImage>(GetWorld(), UImage::StaticClass()))
	//					{
	//						if (UCanvasPanelSlot* PanelSlot = MiniMap->AddChildToCanvas(Point))
	//						{
	//							PanelSlot->SetZOrder(1.0f);
	//							PanelSlot->SetAnchors(0.5f);
	//							Point->SetBrushFromTexture(Tmp.Value.Icon.LoadSynchronous());
	//							PanelSlot->SetSize(FVector2D(32.f));
	//							PanelSlot->SetAlignment(FVector2D(.5f));
	//							CharacterIcons.Add(Tmp.Key, PanelSlot);
	//						}
	//					}
	//				}
	//				else
	//				{
	//					if (UCanvasPanelSlot* PanelSlot = CharacterIcons[Tmp.Key].Get())
	//					{
	//						FVector2D MinMapPos;
	//						MinMapPos.Y = ZoomSize.X - (ZoomSize.X / MapSize.BigMapRealSize.X) * Tmp.Value.Location.X + MinImageMapOffset.Y;
	//						MinMapPos.X = (ZoomSize.Y / MapSize.BigMapRealSize.Y) * Tmp.Value.Location.Y + MinImageMapOffset.X;

	//						ResetLocation(PanelSlot, MinMapPos, LocalSize2D);
	//					}
	//				}
	//			}
	//		}

	//		TArray<FGuid> RemoveGuid;
	//		for (auto& Tmp : CharacterIcons)
	//		{
	//			if (!CharacterDatas.Contains(Tmp.Key))
	//			{
	//				if (UCanvasPanelSlot* PanelSlot = CharacterIcons[Tmp.Key].Get())
	//				{
	//					MiniMap->RemoveChild(PanelSlot->Content);
	//					RemoveGuid.Add(Tmp.Key);
	//				}
	//			}
	//		}

	//		for (auto& Tmp : RemoveGuid)
	//		{
	//			CharacterIcons.Remove(Tmp);
	//		}
	//	}
	//}
}

bool UUI_MiniMap::IsExistence(const FGuid &ID)
{
	for (auto &Tmp : CharacterIcons)
	{
		if (Tmp.Key == ID)
		{
			return true;
		}
	}

	return false;
}

void UUI_MiniMap::ResetLocation(UCanvasPanelSlot* PanelSlot, const FVector2D &MinMapPos, const FVector2D &LocalSize2D)
{
	FVector2D IconSize = PanelSlot->GetSize() / 2;

	auto IsRange = [&](const float &CompareValue, const float &Value)->bool
	{
		return Value > -CompareValue && Value < CompareValue;
	};

	FVector2D NewPos = MinMapPos;
	if (MinMapPos.X <= LocalSize2D.X &&
		MinMapPos.Y <= LocalSize2D.Y &&
		MinMapPos.X >= -LocalSize2D.X &&
		MinMapPos.Y >= -LocalSize2D.Y)
	{

	}
	else if (MinMapPos.Y < -LocalSize2D.Y && IsRange(LocalSize2D.X, MinMapPos.X))//上
	{
		NewPos = FVector2D(MinMapPos.X, -LocalSize2D.Y + IconSize.Y);
	}
	else if (MinMapPos.X < -LocalSize2D.X && IsRange(LocalSize2D.Y, MinMapPos.Y))//左
	{
		NewPos = FVector2D(-LocalSize2D.X + IconSize.Y, MinMapPos.Y);
	}
	else if (MinMapPos.Y > LocalSize2D.Y && IsRange(LocalSize2D.X, MinMapPos.X))//下
	{
		NewPos = FVector2D(MinMapPos.X, LocalSize2D.Y - IconSize.Y);
	}
	else if (MinMapPos.X > LocalSize2D.X && IsRange(LocalSize2D.Y, MinMapPos.Y))//右
	{
		NewPos = FVector2D(LocalSize2D.X - IconSize.X, MinMapPos.Y);
	}
	else if (MinMapPos.X < -LocalSize2D.X && MinMapPos.Y < -LocalSize2D.Y)//左上
	{
		NewPos = -LocalSize2D + IconSize;
	}
	else if (MinMapPos.X < -LocalSize2D.X && MinMapPos.Y > LocalSize2D.Y)//左下
	{
		NewPos = FVector2D(-LocalSize2D.X + IconSize.X, LocalSize2D.Y - IconSize.Y);
	}
	else if (MinMapPos.X > LocalSize2D.X && MinMapPos.Y > LocalSize2D.Y)//右下
	{
		NewPos = LocalSize2D - IconSize;
	}
	else if (MinMapPos.X > LocalSize2D.X && MinMapPos.Y < -LocalSize2D.Y)//右上
	{
		NewPos = FVector2D(LocalSize2D.X - IconSize.X, -LocalSize2D.Y + IconSize.Y);
	}

	PanelSlot->SetPosition(NewPos);
}