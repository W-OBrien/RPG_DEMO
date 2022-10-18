// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RPG_DEMO/RPG_DEMOGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRPG_DEMOGameModeBase() {}
// Cross Module References
	RPG_DEMO_API UClass* Z_Construct_UClass_ARPG_DEMOGameModeBase_NoRegister();
	RPG_DEMO_API UClass* Z_Construct_UClass_ARPG_DEMOGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_RPG_DEMO();
// End Cross Module References
	void ARPG_DEMOGameModeBase::StaticRegisterNativesARPG_DEMOGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ARPG_DEMOGameModeBase);
	UClass* Z_Construct_UClass_ARPG_DEMOGameModeBase_NoRegister()
	{
		return ARPG_DEMOGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_ARPG_DEMOGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ARPG_DEMOGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_RPG_DEMO,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARPG_DEMOGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "RPG_DEMOGameModeBase.h" },
		{ "ModuleRelativePath", "RPG_DEMOGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ARPG_DEMOGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARPG_DEMOGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ARPG_DEMOGameModeBase_Statics::ClassParams = {
		&ARPG_DEMOGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_ARPG_DEMOGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ARPG_DEMOGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ARPG_DEMOGameModeBase()
	{
		if (!Z_Registration_Info_UClass_ARPG_DEMOGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARPG_DEMOGameModeBase.OuterSingleton, Z_Construct_UClass_ARPG_DEMOGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ARPG_DEMOGameModeBase.OuterSingleton;
	}
	template<> RPG_DEMO_API UClass* StaticClass<ARPG_DEMOGameModeBase>()
	{
		return ARPG_DEMOGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ARPG_DEMOGameModeBase);
	struct Z_CompiledInDeferFile_FID_RPG_DEMO_Source_RPG_DEMO_RPG_DEMOGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RPG_DEMO_Source_RPG_DEMO_RPG_DEMOGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ARPG_DEMOGameModeBase, ARPG_DEMOGameModeBase::StaticClass, TEXT("ARPG_DEMOGameModeBase"), &Z_Registration_Info_UClass_ARPG_DEMOGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARPG_DEMOGameModeBase), 3016970045U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_RPG_DEMO_Source_RPG_DEMO_RPG_DEMOGameModeBase_h_997373916(TEXT("/Script/RPG_DEMO"),
		Z_CompiledInDeferFile_FID_RPG_DEMO_Source_RPG_DEMO_RPG_DEMOGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_RPG_DEMO_Source_RPG_DEMO_RPG_DEMOGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
