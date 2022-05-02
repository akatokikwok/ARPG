#pragma once
#include "CoreMinimal.h"
#include "../../../../../../Source/SimpleNetChannel/Public/SimpleNetChannelType.h"

// 登录状态(如账户错误,密码错误,服务器错误,成功之类.)
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

/** 网关状态.*/
struct MMOARPGCOMMON_API FMMOARPGGateStatus
{
	FMMOARPGGateStatus()
		: GateConnectionNum(INDEX_NONE)// 若人数是-1则表明链接已满.
	{}

	int32 GateConnectionNum;// 链接人数.
	FSimpleAddrInfo GateServerAddrInfo;// 网关服务器地址.
};


namespace NetDataAnalysis
{
	// 用户数据写入json.
	void UserDataToString(const FMMOARPGUserData& InUserData, FString& OutString);
	// Json转译成用户数据.
	void StringToUserData(const FString& JsonString, FMMOARPGUserData& OutUserData);

}