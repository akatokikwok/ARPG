#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"

namespace SimpleActorAction
{
	class SIMPLEACTORBROWSING_API FSimplePanelMove : public FTickableGameObject
	{
	public:
		FSimplePanelMove();

		//Monitored by main thread 
		virtual void Tick(float DeltaTime);
		virtual TStatId GetStatId() const;

		void Register(APlayerController* InController, AActor* InTarget, float InDistance);

		void BeginMove();
		void EndMove();

		void ResetTarget(AActor* InNewTarget);
	protected:
		bool bMove;// 启用视口上下移动功能.
		APlayerController* Controller;
		TWeakObjectPtr<AActor> Target;
		FVector2D MousePos_last;
		FVector Location;// 垂直长度上的中点.
		float Distance;// 垂直总长度; 但计算时候一般取一半.
	};
}