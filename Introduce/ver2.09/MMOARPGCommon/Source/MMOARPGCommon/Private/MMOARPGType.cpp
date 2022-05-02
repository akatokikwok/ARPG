#include "MMOARPGType.h"
#include "Json.h"
#include "Serialization/JsonSerializer.h"

namespace NetDataAnalysis
{
	void UserDataToString(const FMMOARPGUserData& InUserData, FString& OutString)
	{
		// 构建一个写入器.
		TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = 
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		Writer->WriteObjectStart();
		{
			Writer->WriteValue(TEXT("ID"), *FString::FromInt(InUserData.ID));
			Writer->WriteValue(TEXT("Account"), *InUserData.Account);
			Writer->WriteValue(TEXT("EMail"), *InUserData.EMail);
			Writer->WriteValue(TEXT("Name"), *InUserData.Name);
			Writer->WriteValue(TEXT("HeadportraitURL"), *InUserData.HeadportraitURL);
		}
		Writer->WriteObjectEnd();
		Writer->Close();
	}

	void StringToUserData(const FString& JsonString, FMMOARPGUserData& OutUserData)
	{
		/**
		 * 所谓json就是类似于这种
		 * "ID":1,
		 * "Accout":"grb",
		 * "Name":"grb",
		 */

		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		TSharedPtr<FJsonValue> ReadRoot;
		// 反序列化.
		if (FJsonSerializer::Deserialize(Reader, ReadRoot)) {
			// 
			if (TSharedPtr<FJsonObject> InJsonObj = ReadRoot->AsObject()) {
				OutUserData.ID = FCString::Atoi( *InJsonObj->GetStringField(TEXT("ID")) );
				OutUserData.Account = InJsonObj->GetStringField(TEXT("Account"));
				OutUserData.EMail = InJsonObj->GetStringField(TEXT("EMail"));
				OutUserData.Name = InJsonObj->GetStringField(TEXT("Name"));
				OutUserData.HeadportraitURL = InJsonObj->GetStringField(TEXT("HeadportraitURL"));
			}
		}

	}
}

