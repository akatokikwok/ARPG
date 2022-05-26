// Fill out your copyright notice in the Description page of Project Settings.

#include "HallPlayerState.h"

FCharacterAppearances& AHallPlayerState::GetCharacterAppearance()
{
	return CharacterAppearances;
}

/** 检查所有舞台角色是否存在符合指定槽位的数据包 */
bool AHallPlayerState::IsCharacterExistInSlot(const int32 InPos)
{
	return 
		CharacterAppearances.FindByPredicate([InPos](const FMMOARPGCharacterAppearance& InCA) ->bool {
			return InPos == InCA.SlotPosition;
		}) != nullptr;
}

void AHallPlayerState::RemoveCharacterAppearanceBySlot(int32 InSlot)
{
//   	FMMOARPGCharacterAppearance CA;
//   	CA.SlotPosition = InSlot;
//      const FMMOARPGCharacterAppearance& CA_Ref = CA;
//      if (CharacterAppearances.Num() > 0 && &CA_Ref != nullptr) {
//          CharacterAppearances.Remove(CA);
//      }
}

FMMOARPGCharacterAppearance* AHallPlayerState::GetRecentCharacter()
{
 	FDateTime MaxDateTime;
 	int32 Index = INDEX_NONE;// 仅负责记录特殊情况下的循环序数.

    /** 反复查找人物存档,直至找到最迟时间点的 */
 	for (int32 i = 0; i < CharacterAppearances.Num(); i++) {
        // FDateTime::Parse负责解析特定字符串成正规的时间.
 		FDateTime DateTime;
 		FDateTime::Parse(CharacterAppearances[i].Date, DateTime);

        // 溢出的特殊情况,刷新最大Date,并且记录一次循环的序数.
 		if (DateTime > MaxDateTime) {
 			MaxDateTime = DateTime;
 			Index = i;
 		}
 	}
    
    // 判空保护.
 	if (Index == INDEX_NONE) {
 		return nullptr;
 	}
 
 	return &CharacterAppearances[Index];// 仅拿取最大情况(即溢出情况)的那个序数.
}

FMMOARPGCharacterAppearance* AHallPlayerState::GetCharacterCA(const int32 InPosIndex)
{
	return CharacterAppearances.FindByPredicate([InPosIndex](const FMMOARPGCharacterAppearance& InCA) {
        return InPosIndex == InCA.SlotPosition; 
    });
}

FMMOARPGCharacterAppearance* AHallPlayerState::AddCharacterCA(const int32 InPosIndex)
{
	FMMOARPGCharacterAppearance* CA_final = nullptr;// 最终拿取到的CA存档.

	if (FMMOARPGCharacterAppearance* InCA = GetCharacterCA(InPosIndex)) {/* 存在指定槽号的CA*/
		CA_final = InCA;
	}
	else {/* 不存在指定槽号的CA, 创一个CA并把其槽号注册为指定的槽号*/
		CharacterAppearances.Add(FMMOARPGCharacterAppearance());
		FMMOARPGCharacterAppearance& InCARef = CharacterAppearances.Last();
		InCARef.SlotPosition = InPosIndex;

		CA_final = &InCARef;
	}

	return CA_final;
}

int32 AHallPlayerState::AddCharacterCA(const FMMOARPGCharacterAppearance& InCA)
{
    return CharacterAppearances.Add(InCA);
}
