#include "Settings/SNDObjectSettings.h"

USNDObjectSettings::USNDObjectSettings()
{
	// CSV路径初始化一下(若不存在则创建)
	CSVSavePath.Path = FPaths::ProjectSavedDir() / TEXT("GameplayAbilitiesCSV");
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*CSVSavePath.Path)) {
		PlatformFile.CreateDirectory(*CSVSavePath.Path);
	}
}

