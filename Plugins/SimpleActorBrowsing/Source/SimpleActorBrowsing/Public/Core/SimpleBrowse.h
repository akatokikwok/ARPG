#pragma once
#include "CoreMinimal.h"
#include "Tickable.h"

// Namespace: 浏览任意3D物体用的命名空间.
namespace SimpleActorAction
{
	/** 3D物体浏览器. */
	class FSimpleBrowse : public FTickableGameObject
	{
	public:
		SIMPLEACTORBROWSING_API FSimpleBrowse(APlayerController* InController, AActor* InTarget);
		SIMPLEACTORBROWSING_API FSimpleBrowse();

		//Monitored by main thread 
		virtual void Tick(float DeltaTime) override;
		virtual TStatId GetStatId() const override;

		/** 注册外界Controller 和 目标至本类. */
 		void SIMPLEACTORBROWSING_API Register(APlayerController* InController, AActor* InTarget);

		/** 开启浏览. 加插件宏表示允许暴露给使用者,反之私有不暴露 */
		void SIMPLEACTORBROWSING_API OpenBrowsing();
		
		/** 关闭浏览. 加插件宏表示允许暴露给使用者,反之私有不暴露 */
		void SIMPLEACTORBROWSING_API EndBrowsing();
		
		/** 重设一个目标.  加插件宏表示允许暴露给使用者,反之私有不暴露 */
 		void SIMPLEACTORBROWSING_API ResetTarget(AActor* InNewTarget);
	
	protected:
		bool bRotateCharacter;// 启用旋转开关.
		APlayerController* Controller;

		/* TWeakObjectPtr是 持有UObject的智能指针
		 * 若使用UObject类，要引用它又不想因为引用了而影响GC,推荐使用TWeakObjectPtr<AActor>这种形式.
		 */
		TWeakObjectPtr<AActor> Target;// 接收旋转操作的目标人物.推荐使用弱指针,即使被销毁也会通知到开发者.
		
		FVector2D MousePos_last;// 最近一次鼠标在屏幕上的位置.
	};
}