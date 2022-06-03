/**
 * 此文件用以存储一些常用的关于4个服务器直接收发协议的 头文件与宏还有宏方法.
 */

#include "Core/Common/MMOARPGGameInstance.h"
#include "SimpleProtocolsDefinition.h"

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