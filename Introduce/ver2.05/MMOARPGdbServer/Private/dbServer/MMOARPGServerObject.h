/**
 * 是一个反射类,可以允许用户更自由地自定义.
 */

#pragma once
#include "CoreMinimal.h"
#include "..\Plugins\SimpleNetChannel\Source\SimpleNetChannel\Public\UObject\SimpleController.h"
#include "Blueprint/SimpleMysqlObject.h"
#include "Core/SimpleMysqlLinkType.h"
#include "MMOARPGServerObject.generated.h"

/** ServerObejct */
UCLASS()
class UMMOARPGServerObejct : public USimpleController
{
	GENERATED_BODY()

public:
	virtual void Init() override;// 有新的链接链入我们的服务器,会激活Init.
	virtual void Tick(float DeltaTime) override;// Tick服务器自身.
	virtual void Close() override;// Close.
	virtual void RecvProtocol(uint32 InProtocol) override;// 对方发的协议借助此方法感知到.

public:
	bool Post(const FString& InSQL);// 把数据库数据提到服务器上.
	bool Get(const FString& InSQL, TArray<FSimpleMysqlResult>& Results);// 把数据库数据拉下来.

protected:
	USimpleMysqlObject* MysqlObjectRead;// 数据库对象:读
	USimpleMysqlObject* MysqlObjectWrite;// 数据库对象:写

};