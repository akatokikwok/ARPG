#pragma once

#include "CoreMinimal.h"
#include "SimpleAdvancedAnimationType.generated.h"

typedef int32 FSAAHandle;// int32别名.

/**
 * 每只脚IK时候的IK信息.
 * Character可以有多只脚.
 */
USTRUCT(BlueprintType)
struct FFootIKInfo
{
	GENERATED_USTRUCT_BODY()
public:
	// 单只脚当前偏移.(相对于地面)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleAdvancedAnimation|FootIKInfo")
		float Offset;
};