// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UI_LoginMain.h"
#include "Stream\SimpleIOStream.h"

void UUI_Login::NativeConstruct()
{
	Super::NativeConstruct();

	SignInButton->OnReleased.AddDynamic(this, &UUI_Login::SignIn);
	RegisterButton->OnReleased.AddDynamic(this, &UUI_Login::Register);

}

void UUI_Login::NativeDestruct()
{
	Super::NativeDestruct();

}

void UUI_Login::SignIn()
{
	// UUI_LoginMain设定为本面板的持有者面板.
	// 借助持有者面板来使用 登录逻辑.
	if (UUI_LoginMain* InLoginMain = UUI_Base::GetParents<UUI_LoginMain>()) {
		FString AccountString = Account->GetText().ToString();
		FString PasswordString = Password->GetText().ToString();
		InLoginMain->SignIn(AccountString, PasswordString);
	}
}

void UUI_Login::Register()
{
	// UUI_LoginMain设定为本面板的持有者面板.
	// 借助持有者面板来使用 注册逻辑.
	if (UUI_LoginMain* InLoginMain = UUI_Base::GetParents<UUI_LoginMain>()) {
		InLoginMain->Register();
	}
}

/** 加密账户密码到本地.*/
bool UUI_Login::EncryptionToLocal(const FString& InPaths)
{
	FString AccountString = Account->GetText().ToString();
	FString PasswordString = Password->GetText().ToString();

	// 不合理的路径纠正.
	FString InCopyPaths = InPaths;
	FPaths::NormalizeFilename(InCopyPaths);// 将所有反斜杠规范化为正斜杠.
	InCopyPaths.RemoveFromEnd(TEXT("/"));

	FString ErrorPath = FPaths::ProjectDir();
	check(InCopyPaths != ErrorPath);// 满足非错误路径的时候才会继续执行.

	// 判空.
	if (!AccountString.IsEmpty() && !PasswordString.IsEmpty() && !InPaths.IsEmpty()) {
		TArray<uint8> PasswordBuffer;// 此字节码需要凑齐16倍数.
		FSimpleIOStream Stream(PasswordBuffer);// 存储数据的流.

		Stream << PasswordString;// Stream负责把string压缩成uint8

		// MD5哈希加密.
		FString HashKey = FMD5::HashAnsiString(*AccountString);
		for (int32 i = 0; i < 32; ++i) {
			HashKey = FMD5::HashAnsiString(*HashKey);// 再加密32次.
		}

		// 算法, 让流再载入完整倍数.
		if (!HashKey.IsEmpty()) {
			int32 Remain = 0;
			int32 Total = PasswordBuffer.Num();
			if (Total % 16) {
				float Value = (float)Total / 16.0f;
				int32 Ceil = FMath::CeilToInt(Value);

				int32 CeilToal = Ceil * 16;
				Remain = CeilToal - Total;
			}
			for (int32 i = 0; i < Remain; ++i) {
				uint8 Tmp = 0;
				Stream << Tmp;
			}

			ANSICHAR* InKey = TCHAR_TO_UTF8(*HashKey);
			FAES::EncryptData(PasswordBuffer.GetData(), PasswordBuffer.Num(), InKey);// 使用FAES::EncryptData API加密.

			IFileManager::Get().DeleteDirectory(*InCopyPaths);// 不管之前有多少旧的账户密码都默认删干净.
			FFileHelper::SaveArrayToFile(PasswordBuffer, *(InCopyPaths / AccountString + TEXT(".arpg")));// 重新储存密码.

			return true;
		}
	}

	return false;
}

/** 从本地解密密码. */
bool UUI_Login::DecryptionFromLocal(const FString& InPaths)
{
	// 不合理的路径纠正.
	FString InCopyPaths = InPaths;
	FPaths::NormalizeFilename(InCopyPaths);// 将所有反斜杠规范化为正斜杠.
	InCopyPaths.RemoveFromEnd(TEXT("/"));

	// 遍历查找加密文件.
	TArray<FString> Filenames;
	IFileManager::Get().FindFilesRecursive(Filenames, *InPaths, TEXT("*.*"), true, false);
	for (auto& Tmp : Filenames) {
		// 例如从D:\new_video\111.arpg 里取得111.arpg
		FString FilenameHead = FPaths::GetCleanFilename(Tmp);
		if (FilenameHead.Contains(TEXT(".arpg"))) {// 检查文件里是不是含有后缀.arpg
			// 读取出已加过密的密码.
			TArray<uint8> InByte;// InByte负责存储"加密密码里的数据".
			FFileHelper::LoadFileToArray(InByte, *Tmp);
			FilenameHead.RemoveFromEnd(TEXT(".arpg"));// 移除后缀之后则变为账户.
			if (InByte.Num() > 0) {// 判空.
				// MD5哈希加密.
				FString HashKey = FMD5::HashAnsiString(*FilenameHead);// 对账号执行哈希加密.
				for (int32 i = 0; i < 32; ++i) {
					HashKey = FMD5::HashAnsiString(*HashKey);// 再加密32次.
				}

				if (!HashKey.IsEmpty()) {
					FSimpleIOStream Stream(InByte);// 申请1个流.

					// 用流读出来密码.
					ANSICHAR* InKey = TCHAR_TO_UTF8(*HashKey);
					FAES::DecryptData(InByte.GetData(), InByte.Num(), InKey);// 解密出来.
					FString PasswordString;
					Stream >> PasswordString;

					Account->SetText(FText::FromString(FilenameHead));// 账户.
					Password->SetText(FText::FromString(PasswordString));// 密码.

					return true;
				}
			}
		}
	}

	return false;
}
