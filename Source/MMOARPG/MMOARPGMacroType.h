/**
 * 此文件用以存储一些常用的关于4个服务器直接收发协议的 头文件与宏还有宏方法.
 */
#pragma once
#include "Core/Common/MMOARPGGameInstance.h"
#include "SimpleProtocolsDefinition.h"

#define UE_MMOARPG_DEBUG_DS 0 // 用于开关调试功能的宏. 1开启0关闭. 打包的时候要设置成0. 在DS下测试数据要设置为1.

#if UE_MMOARPG_DEBUG_DS
static uint32 GameCount = 0;// 用于给机器人切换用户号用.
#endif


/** 包装一个发送协议的宏. */
#if PLATFORM_WINDOWS

#define SEND_DATA(InProtocols,...) \
if (UMMOARPGGameInstance* InGIns_A = GetGameInstance<UMMOARPGGameInstance>()) \
{ \
	SIMPLE_CLIENT_SEND(InGIns_A->GetClient(), InProtocols, __VA_ARGS__); \
}

#else

#define SEND_DATA(InProtocols, args...) \
if (UMMOARPGGameInstance* InGIns_A = GetGameInstance<UMMOARPGGameInstance>()) \
{ \
	SIMPLE_CLIENT_SEND(InGIns_A->GetClient(), InProtocols, ##args); \
}
#endif