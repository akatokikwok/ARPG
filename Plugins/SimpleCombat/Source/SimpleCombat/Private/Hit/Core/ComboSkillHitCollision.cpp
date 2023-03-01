#include "Hit/Core/ComboSkillHitCollision.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "Components/SplineComponent.h"

//
AHitCollision::AHitCollision(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 为了体现出持续施法法球运动轨迹,所以开启Tick.
	PrimaryActorTick.bCanEverTick = true;

	// 对于根组件初始化
	HitCollisionRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionRootComponent"));
	RootComponent = HitCollisionRootComponent;

	// 对于弹丸组件初始化
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->InitialSpeed = 1600.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->UpdatedComponent = RootComponent;

	InitialLifeSpan = 4.0f;// 初始化本actor生命时长.

	bNetLoadOnClient = false;
	bReplicates = false;

	// 默认是近程攻击型
	HitCollisionType = EHitCollisionType::HITCOLLISIONTYPE_SHORT_RANGE_ATTACK;

	// 持续施法的持续时长初始化为0秒
	CurrentSplineTime = 0.f;
}

//
void AHitCollision::BeginPlay()
{
	Super::BeginPlay();

	if (UPrimitiveComponent* InHitComponent = GetHitDamage()) {

		Collision(false);// 默认先禁用碰撞.
		InHitComponent->SetHiddenInGame(false);
		InHitComponent->OnComponentBeginOverlap.AddDynamic(this, &AHitCollision::HandleDamage);// 与场景物体相撞接触的时候绑定回调.
	}


	/** 持续施法的追踪曲线逻辑编写 */
	if (ISimpleComboInterface* InInstiagtor = Cast<ISimpleComboInterface>(GetInstigator())) {
		switch (HitCollisionType) {
			case EHitCollisionType::HITCOLLISIONTYPE_SHORT_RANGE_ATTACK:
			case EHitCollisionType::HITCOLLISIONTYPE_TRACK_LINE:
			{
				ProjectileMovement->MaxSpeed = 0.f;
				ProjectileMovement->InitialSpeed = 0.f;
				ProjectileMovement->ProjectileGravityScale = 0.f;
				break;
			}
		}

		switch (HitCollisionType) {
			case EHitCollisionType::HITCOLLISIONTYPE_SHORT_RANGE_ATTACK:
			{
				break;
			}
			case EHitCollisionType::HITCOLLISIONTYPE_DIRECT_LINE:
				break;
			case EHitCollisionType::HITCOLLISIONTYPE_LINE:
				break;
				/// 追踪类型的伤害
			case EHitCollisionType::HITCOLLISIONTYPE_TRACK_LINE:
			{
				ProjectileMovement->StopMovementImmediately();

				if (AActor* InTarget = InInstiagtor->GetTarget()) {// 先读取敌对目标

					/* new一个曲线并设置*/
					Spline = NewObject<USplineComponent>(this, TEXT("SplineInstance"));
					Spline->RegisterComponent();
					// 这个点被识别为spline的0号点
					Spline->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::Local);

					// 敌人到我的向量
					DistanceVector = -(InTarget->GetActorLocation() - this->GetActorLocation());
					// 取上面那个向量的中间点的3D坐标
					FVector Position = DistanceVector / 2 + GetActorLocation();
					// 抬高了之后就得到第二个点的坐标
					Position.Z = (DistanceVector.Size() / 2.f) * 0.5f;
					// 这个点被识别为spline的1号点
					Spline->SetLocationAtSplinePoint(1, Position, ESplineCoordinateSpace::Local);
					// 这个点被识别为spline的2号点
					Spline->AddSplinePoint(InTarget->GetActorLocation(), ESplineCoordinateSpace::Local);
				}
				break;
			}
			case EHitCollisionType::HITCOLLISIONTYPE_RANGE_LINE:
				break;
			case EHitCollisionType::HITCOLLISIONTYPE_RANGE:
				break;
			case EHitCollisionType::HITCOLLISIONTYPE_CHAIN:
				break;
		}
	}
}

//
void AHitCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** 持续施法的追踪曲线逻辑编写(Tick部分) */
	switch (HitCollisionType) {
		case EHitCollisionType::HITCOLLISIONTYPE_SHORT_RANGE_ATTACK:
			break;
		case EHitCollisionType::HITCOLLISIONTYPE_DIRECT_LINE:
			break;
		case EHitCollisionType::HITCOLLISIONTYPE_LINE:
			break;
		case EHitCollisionType::HITCOLLISIONTYPE_TRACK_LINE:
		{
			if (Spline) {
				if (ISimpleComboInterface* InInstigator = Cast<ISimpleComboInterface>(GetInstigator())) {
					// 持续时长累加
					CurrentSplineTime += DeltaTime;
					/* 先解算出每一帧落在这根向量上的分量的尺寸, 1000是一个系数,方便更明显/迟钝暴露效果*/
					float Distance = Spline->GetSplineLength() * CurrentSplineTime / (DistanceVector.Size() / 1000.f);
					/* 借助上一步结算出来的单帧落下尺寸, 求取spline此帧的坐标和旋转朝向(看上去的效果就是这个碰撞盒子随时长往敌人方向运动和推进). */
					FVector Loction = Spline->GetWorldLocationAtDistanceAlongSpline(Distance);
					FRotator Rotator = Spline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
					SetActorLocationAndRotation(Loction, Rotator);


// 					if (AActor* InTarget = InInstigator->GetTarget()) {
// 						float MyDistance = FVector::Distance(InTarget->GetActorLocation(), GetActorLocation());
// 						if (MyDistance < 50.f) {
// 							Destroyed();
// 						}
// 					}
				}
			}
			break;
		}
		case EHitCollisionType::HITCOLLISIONTYPE_RANGE_LINE:
			break;
		case EHitCollisionType::HITCOLLISIONTYPE_RANGE:
			break;
		case EHitCollisionType::HITCOLLISIONTYPE_CHAIN:
			break;
	}
}

// 回调虚方法: 处理碰撞接触时候的逻辑;  可由子类覆写.
void AHitCollision::HandleDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* InCharacter = Cast<ACharacter>(OtherActor)) {
		AttackedTarget.AddUnique(InCharacter);
	}
}

// 虚方法,让派生类覆写; 拿取对应的形状comp;
UPrimitiveComponent* AHitCollision::GetHitDamage()
{
	return nullptr;
}

// 设置 具备形状comp 的相对位置.
void AHitCollision::SetHitDamageRelativePosition(const FVector& InNewPostion)
{
	if (UPrimitiveComponent* InHitComponent = GetHitDamage()) {
		InHitComponent->SetRelativeLocation(InNewPostion);
	}
}

// 操作启用或禁用hitbox碰撞.
void AHitCollision::Collision(bool bTurnOnCollision)
{
	if (UPrimitiveComponent* InHitComponent = GetHitDamage()) {
		if (bTurnOnCollision) {
			InHitComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else {
			InHitComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

bool AHitCollision::IsExist(ACharacter* InNewTarget) const
{
	for (auto& Tmp : AttackedTarget) {
		if (Tmp.IsValid()) {
			if (Tmp == InNewTarget) {
				return true;
			}
		}
	}
	return false;
}
