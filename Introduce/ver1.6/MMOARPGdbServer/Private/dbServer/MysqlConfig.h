#pragma once
#include "MMOARPGdbServerType.h"

/* 
 * 管理配置表的单例类
 */
class FSimpleMysqlConfig
{
public:
	static FSimpleMysqlConfig* Get();
	static void Destroy();

	void Init(const FString& InPath = FPaths::ProjectDir() / TEXT("MysqlConfig.ini"));// 外部指定路径然后初始化的接口
	const FMysqlConfig& GetInfo() const;// 直接访问到配置表的接口
protected:
private:
	static FSimpleMysqlConfig* Global;// 本单例
	FMysqlConfig ConfigInfo;
};