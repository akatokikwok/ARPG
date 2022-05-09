// Copyright Epic Games, Inc. All Rights Reserved.


#include "MMOARPGGateServer.h"
#include "Log/MMOARPGGateServerLog.h"
#include "../Plugins/SimpleNetChannel/Source/SimpleNetChannel/Public/SimpleNetManage.h"
#include "../Plugins/SimpleNetChannel/Source/SimpleNetChannel/Public/Global/SimpleNetGlobalInfo.h"
#include "ServerList.h"
#include "RequiredProgramMainCPPInclude.h"
#include "GateServer/MMOARPGGateServerObject.h"
#include "GateServer/MMOARPGdbClientObject.h"


IMPLEMENT_APPLICATION(MMOARPGGateServer, "MMOARPGGateServer");

INT32_MAIN_INT32_ARGC_TCHAR_ARGV()
{
	GEngineLoop.PreInit(ArgC, ArgV);
	UE_LOG(LogMMOARPGGateServer, Display, TEXT("GateServer, Hello World"));
	
	/* 1.仅初始化分布式插件.*/
	FSimpleNetGlobalInfo::Get()->Init();// 初始化分布式插件.

	/* 2.创建网关框架下的 一些服务器实例.*/
	GateServer =
		FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_LISTEN, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
	
	dbClient = 
		FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);

	/** 3. 依据不同类型走不同的反射方式. */
	GateServer->NetworkObjectClass = UMMOARPGGateServerObejct::StaticClass();
	dbClient->NetworkObjectClass = UMMOARPGdbClientObject::StaticClass();



	/* 4.初始化检查.*/
	if (!GateServer->Init()) {
		delete GateServer;
		UE_LOG(LogMMOARPGGateServer, Error, TEXT("GateServer Instance Init Fail. "));
		return INDEX_NONE;
	}

	if (!dbClient->Init(TEXT("127.0.0.1"), 11221)) { /* 让db客户端链接到 db服务器,由于db服务器的.ini里的端口设置成了11221.*/
		delete dbClient;
		UE_LOG(LogMMOARPGGateServer, Error, TEXT("dbClient Instance Init Fail. "));
		return INDEX_NONE;
	}

	/* 5. 运行监测*/
	double LastTime = FPlatformTime::Seconds();
	while (!IsEngineExitRequested()) {
		// 休眠,来模拟每一帧都是0.03秒.
		FPlatformProcess::Sleep(0.03f);// 可在上线项目中注释此行,因为它会减缓时间.

		double Now = FPlatformTime::Seconds();
		float DeltaSeconds = Now - LastTime;// 求取每帧耗时.
		// 对这些 服务器实例Tick.每一帧检查链接.
		GateServer->Tick(DeltaSeconds);
		dbClient->Tick(DeltaSeconds);

		LastTime = Now;
	}

	// 附带销毁.
	FSimpleNetManage::Destroy(dbClient);
	FSimpleNetManage::Destroy(GateServer);

	FEngineLoop::AppExit();
	return 0;
}
