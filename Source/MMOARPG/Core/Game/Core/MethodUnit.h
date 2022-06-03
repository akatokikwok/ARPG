#pragma once
#include "CoreMinimal.h"

/**
 * 存放一些关于游玩的通用方法.
 */
namespace MethodUnit
{
	// 服务器呼叫类型: 进行中/结束.
	enum EServerCallType :uint8
	{
		INPROGRESS,
		PROGRESS_COMPLETE
	};

	/** 服务器呼叫 遍历寻找特定玩家控制 并用仿函数处理它们. */
	template<class T>
	void MMOARPG_API ServerCallAllPlayerController(UWorld* NewWorld, TFunction<EServerCallType(T*)> InImplement)
	{
		for (FConstPlayerControllerIterator It = NewWorld->GetPlayerControllerIterator(); It; ++It) {
			if (T* InPlayerController = Cast<T>(It->Get())) {
				// 检查仿函数(处理controller)返回值是不是处于结束.满足则断开.不满足则继续遍历.
				if (InImplement(InPlayerController) == EServerCallType::PROGRESS_COMPLETE) {
					break;
				}
			}
		}
	}

	/** 服务器呼叫 遍历寻找特定玩家 并用仿函数处理它们. */
	template<class T>
	void MMOARPG_API ServerCallAllPlayer(UWorld* NewWorld, TFunction<EServerCallType(T*)> InImplement)
	{
		ServerCallAllPlayerController<APlayerController>(
			NewWorld,
			// 让仿函数(处理pawn)获取处理结果, 若出错则断开.
			[&](APlayerController* InPlayerContoroller) ->EServerCallType {
				if (T* InPawn = Cast<T>(InPlayerContoroller->GetPawn())) {
					return InImplement(InPawn);
				}
				return EServerCallType::INPROGRESS; }
		);
	}

}