#pragma once
#include "CoreMinimal.h"

enum ELoginType
{
	LOGIN_DB_SERVER_ERROR,
	LOGIN_SUCCESS,
	LOGIN_ACCOUNT_WRONG,
	LOGIN_PASSWORD_WRONG,
};

struct MMOARPGCOMMON_API FMMOARPGUserData
{
	FMMOARPGUserData()
		: ID(INDEX_NONE)
	{
	
	}

	int32 ID;
	FString Account;
	FString EMail;
	FString Name;
	FString HeadportraitURL;

};

namespace NetDataAnalysis
{
	// 用户数据写入json.
	void UserDataToString(const FMMOARPGUserData& InUserData, FString& OutString);
	// Json转译成用户数据.
	void StringToUserData(const FString& JsonString, FMMOARPGUserData& OutUserData);

}