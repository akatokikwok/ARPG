#pragma once
#include "CoreMinimal.h"
#include "SimpleRuntimeGamePrintingType.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType, Blueprintable)
struct FSimplePrintSlot
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimplePrintSlot")
		FString Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimplePrintSlot")
		FString Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimplePrintSlot")
		FString ImgID;
};

namespace SimpleRuntimeGamePrinting
{
	/**
	 * 绘制空间, 负责通过一系列XML链式编程语法输出一个定制字符串内容
	 */
	struct SIMPLERUNTIMEGAMEPRINTING_API FPrintSlot
	{
		struct SIMPLERUNTIMEGAMEPRINTING_API FSlot
		{
			// 设置 富文本的 XML格式颜色
			FSlot& Color(const FString& InColor);

			// 以图片ID路径设置一下 最终字符串
			FSlot& Image(const FString& InImage);

			// 拿取最终字符串内容
			const FString& ToString() const;

			// 暂存最终字符串内容
			bool GetString(FString& OutString) const;

			// 重载[]操作符, 输出一个最终加工合并出的字符串
			FSlot& operator[](const FString& InContent)
			{
				if (!ColorContent.IsEmpty()) {
					Content += FString::Printf(TEXT("<%s>%s</>"), *ColorContent, *InContent);
				}
				return *this;
			};

		protected:
			FString Content;// 最终合并出来的Content

			FString ColorContent;// 编辑器里指定配好的某种颜色字体
		};

		// 构造一块放字符的空间
		FSlot& AddSlot();
		// 输出 最终字符串
		const FString ToString() const;
		// 输出 最终字符串
		bool GetString(FString& OutString) const;
		//
		~FPrintSlot();

	protected:
		// 存储Slot的容器
		TArray<FSlot> Slots;
	};
}

// 重载操作符之后的使用示例
//{
//	FPrintSlot Slot;
//	Slot.AddSlot()
//	.Color("Blue")
//	[
//		"Hello kasd akasd iojasaskod "
//	]
//	.Image("HelloCCCC")
//}