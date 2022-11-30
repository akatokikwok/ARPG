#include "Settings/SNDObjectSettings.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/DataTable.h"
#include "DataTableUtils.h"

USNDObjectSettings::USNDObjectSettings()
	: BaseTable(nullptr)
{
	// CSV路径初始化一下(若不存在则创建)
	CSVSavePath.Path = TEXT("D:/Project/UEProj/ARPG/Saved/GameplayAbilitiesCSV");// 先给一个写死的路径
	//CSVSavePath.Path = FPaths::ProjectSavedDir() / TEXT("GameplayAbilitiesCSV");
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*CSVSavePath.Path)) {
		PlatformFile.CreateDirectory(*CSVSavePath.Path);
	}
}

/** 解析基础表 */
bool USNDObjectSettings::AnalysisBaseTable()
{
	if (BaseTable) {
		// 先清空 这份表集
		AttributeDatas.Empty();
		// 取出基础表里的rowmap(实际上就是那些attributes); <Name, 内存块(里面含有蓝 血 Lv)>数据结构.
		TMap<FName, uint8*>& RowMap = const_cast<TMap<FName, uint8*>&>(BaseTable->GetRowMap());

		// 再从基础表里取出 含有那些属性的一个结构
		if (const UScriptStruct* RowStruct = BaseTable->GetRowStruct()) {
			// 声明并构建1个白模板, 是1个表;
			FDeduceAttributeDataTables TmpAttributeDataTables;

			/* 1. 遍历rowstruct; 提出各个名称,存到一个集合里*/
			//  StructProps是 基础表的rowstruct;
			TArray<FProperty*> StructProps;
			for (TFieldIterator<FProperty> It(RowStruct); It; ++It) {
				FProperty* Prop = *It;
				check(Prop != nullptr);
				StructProps.Add(Prop);
			}
			// 再往白模板里注册, 注册这一系列的"key字段"
			for (int32 i = 0; i < StructProps.Num(); i++) {
				FDeduceAttributeData& InAttributeData = TmpAttributeDataTables.AttributeDatas.AddDefaulted_GetRef();// 先给数组添加一个元素再返回默认引用
				InAttributeData.Key = *StructProps[i]->GetName();
			}

			/* 2.关于单个rowmap迭代器<-扫描基础表的rowmap(即基础表内的总数据和); 提取一系列的"Value字段"*/
			for (auto RowIt = RowMap.CreateConstIterator(); RowIt; ++RowIt) {
				// 表集里新增一个元素, 即一个单表
				FDeduceAttributeDataTables& InAttributeData = AttributeDatas.Add_GetRef(TmpAttributeDataTables);
				// 元素的字段被填充为
				InAttributeData.TableName = RowIt.Key();
				// rowmap里单个元素的内存块
				uint8* RowData = RowIt.Value();
				
				/* 再扫描一次 基础表的Rowstruct*/
				for (int32 PropIdx = 0; PropIdx < StructProps.Num(); PropIdx++) {
					// 单个具体属性的具体值 由rowmap和rowstruct组合搭配提纯而出; 使用工具API来返回对应偏移的内存块; 单表单属性的具体值被注册好 (来源是来自RowStruct); 
					InAttributeData.AttributeDatas[PropIdx].Value = 
						DataTableUtils::GetPropertyValueAsString(StructProps[PropIdx], RowData, EDataTableExportFlags::None);
					
					// 类型收集与判定
					if (StructProps[PropIdx]->IsA(FIntProperty::StaticClass()) ||
						StructProps[PropIdx]->IsA(FInt8Property::StaticClass()) ||
						StructProps[PropIdx]->IsA(FInt64Property::StaticClass())) {
						InAttributeData.AttributeDatas[PropIdx].AttributeDataType = EDeduceAttributeDataType::DEDUCETYPE_INT32;
					}
					else if (StructProps[PropIdx]->IsA(FFloatProperty::StaticClass()) ||
						StructProps[PropIdx]->IsA(FDoubleProperty::StaticClass())) {
						InAttributeData.AttributeDatas[PropIdx].AttributeDataType = EDeduceAttributeDataType::DEDUCETYPE_FLOAT;
					}
					else if (StructProps[PropIdx]->IsA(FStrProperty::StaticClass()) ||
						StructProps[PropIdx]->IsA(FTextProperty::StaticClass()) ||
						StructProps[PropIdx]->IsA(FNameProperty::StaticClass()) ||
						StructProps[PropIdx]->IsA(FArrayProperty::StaticClass())) {// 如果是Array类型则也强制识别为string
						InAttributeData.AttributeDatas[PropIdx].AttributeDataType = EDeduceAttributeDataType::DEDUCETYPE_STRING;
					}
					else {
						InAttributeData.AttributeDatas[PropIdx].AttributeDataType = EDeduceAttributeDataType::DEDUCETYPE_FLOAT;
					}
				}
			}
			return true;
		}
	}
	return false;
}

void USNDObjectSettings::SaveObjectConfig()
{
	// Slate样式数据保存至上面定义的路径
	SaveConfig(CPF_Config, *(this->GetDefaultConfigFilename()));
}

void USNDObjectSettings::LoadObjectConfig()
{
	LoadConfig(GetClass(), *(this->GetDefaultConfigFilename()));
}

FString USNDObjectSettings::GetDefaultConfigFilename()
{
	// 先定义1个指定位置的保存路径
	// GetClass()->ClassConfigName这一句 与UCLASS(config = SNDObjectSettings)的命名有关系
	return FPaths::ProjectConfigDir() / TEXT("Default") + GetClass()->ClassConfigName.ToString() + TEXT(".ini");
}

