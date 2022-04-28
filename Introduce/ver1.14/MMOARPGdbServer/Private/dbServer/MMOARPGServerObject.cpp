#include "MMOARPGServerObject.h"
#include "SimpleMySQLibrary.h"
#include "MysqlConfig.h"
#include "Log\MMOARPGdbServerLog.h"

void UMMOARPGServerObejct::Init()
{
	Super::Init();

	// 创建"读" 的数据库对象
	MysqlObjectRead = USimpleMySQLLibrary::CreateMysqlObject(nullptr,
		FSimpleMysqlConfig::Get()->GetInfo().User,
		FSimpleMysqlConfig::Get()->GetInfo().Host,
		FSimpleMysqlConfig::Get()->GetInfo().Pwd,
		FSimpleMysqlConfig::Get()->GetInfo().DB,
		FSimpleMysqlConfig::Get()->GetInfo().Port,
		FSimpleMysqlConfig::Get()->GetInfo().ClientFlags
	);

	// 创建"写" 的数据库对象.
	MysqlObjectWrite = USimpleMySQLLibrary::CreateMysqlObject(nullptr,
		FSimpleMysqlConfig::Get()->GetInfo().User,
		FSimpleMysqlConfig::Get()->GetInfo().Host,
		FSimpleMysqlConfig::Get()->GetInfo().Pwd,
		FSimpleMysqlConfig::Get()->GetInfo().DB,
		FSimpleMysqlConfig::Get()->GetInfo().Port,
		FSimpleMysqlConfig::Get()->GetInfo().ClientFlags
	);

	//仅测试代码.
//  	FString SQL = "SELECT * FROM wp_users WHERE ID = 1";
//  	TArray<FSimpleMysqlResult> Results;
//  	Get(SQL, Results);// 使用此语句查东西然后把结果存出来.
//  	for (auto& Tmp : Results) {
//  	
//  	}
	
}

void UMMOARPGServerObejct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void UMMOARPGServerObejct::Close()
{
	Super::Close();

	// 服务器关闭的时候执行安全清除 数据库对象.
	if (MysqlObjectWrite != nullptr) {
		MysqlObjectWrite->ConditionalBeginDestroy();
		MysqlObjectWrite = nullptr;
	}
	if (MysqlObjectRead != nullptr) {
		MysqlObjectRead->ConditionalBeginDestroy();
		MysqlObjectRead = nullptr;
	}
}

void UMMOARPGServerObejct::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

}

bool UMMOARPGServerObejct::Post(const FString& InSQL)
{
	// 只需要负责把SQL数据  写入 服务器.
	if (!InSQL.IsEmpty()) {
		if (MysqlObjectWrite != nullptr) {
			// 利用 SQL对象查找错误消息
			FString ErrMsg;
			USimpleMySQLLibrary::QueryLink(MysqlObjectWrite, InSQL, ErrMsg);
			if (ErrMsg.IsEmpty()) {
				return true;
			}
			else {
				// 打印出错误消息.
				UE_LOG(LogMMOARPGdbServer, Error, TEXT("MMOARPGdbServer Error : Post msg [ %s]"), *ErrMsg);
			}
		}
	}
	return false;
}

bool UMMOARPGServerObejct::Get(const FString& InSQL, TArray<FSimpleMysqlResult>& Results)
{
	// 只需要负责把SQL数据  写入 服务器.
	if (!InSQL.IsEmpty()) {
		if (MysqlObjectRead != nullptr) {
			// 调试信息、错误消息之类.
			FSimpleMysqlDebugResult Debug;
			Debug.bPrintToLog = true;
			FString ErrMsg;

			USimpleMySQLLibrary::QueryLinkResult(MysqlObjectRead,
				InSQL,
				Results,
				ErrMsg,
				EMysqlQuerySaveType::STORE_RESULT,// 先下后查模式.
				Debug
			);
			if (ErrMsg.IsEmpty()) {
				return true;
			}
			else {
				// 打印出错误消息.
				UE_LOG(LogMMOARPGdbServer, Error, TEXT("MMOARPGdbServer Error : Get msg [ %s]"), *ErrMsg);
			}
		}
	}
	return false;
}
