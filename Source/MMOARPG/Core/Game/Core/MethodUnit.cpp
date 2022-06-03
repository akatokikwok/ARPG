#include "MethodUnit.h"

// namespace MethodUnit
// {
//   	template<class T>
//   	void MMOARPG_API ServerCallAllPlayerController(UWorld* NewWorld, TFunction<EServerCallType(T*)> InImplement)
//   	{
//   		for (FConstPlayerControllerIterator It = NewWorld->GetPlayerControllerIterator(); It; ++It) {
//   			if (T* InPlayerController = Cast<T>(It->Get())) {
//   				// 检查仿函数(处理controller)返回值是不是处于结束.满足则断开.不满足则继续遍历.
//   				if (InImplement(InPlayerController) == EServerCallType::PROGRESS_COMPLETE) {
//   					break;
//   				}
//   			}
//   		}
//   	}
//   
//   	template<class T>
//   	void MMOARPG_API ServerCallAllPlayer_arpg(UWorld* NewWorld, TFunction<EServerCallType(T*)> InImplement)
//   	{
//   		ServerCallAllPlayerController<APlayerController>(
//   			NewWorld, 
//   			// 让仿函数(处理pawn)获取处理结果, 若出错则断开.
//   			[&](APlayerController* InPlayerContoroller) ->EServerCallType {
//   				if (T* InPawn = Cast<T>(InPlayerContoroller->GetPawn())) {
//   					return InImplement(InPawn);
//   				}
//   				return EServerCallType::INPROGRESS;}
//   		);
//   	}
// }