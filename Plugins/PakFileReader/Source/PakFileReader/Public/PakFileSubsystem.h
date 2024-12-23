// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PakFileSubsystem.generated.h"

UCLASS()
class PAKFILEREADER_API UPakFileSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static const bool IsRunningPIE();

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "PAK")
	FString const GetPakMountPoint(const FString& PakFilePath);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "PAK")
	TArray<FString> GetPakContent(const FString& PakFilePath, bool bOnlyCooked = true);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "PAK")
	FString GetPakMountContentPath(const FString& PakFilePath);

	UFUNCTION(BlueprintCallable, Category = "PAK")
	bool MountPakFile(const FString& PakFilePath, const FString& PakMountPoint);

	UFUNCTION(BlueprintCallable, Category = "PAK")
	bool UnmountPakFile(const FString& PakFilePath);

	UFUNCTION(BlueprintCallable, Category = "PAK")
	void RegisterMountPoint(const FString& RootPath, const FString& ContentPath);

	UFUNCTION(BlueprintCallable, Category = "PAK")
	void UnregisterMountPoint(const FString& RootPath, const FString& ContentPath);

	UFUNCTION(BlueprintCallable, Category = "PAK")
	bool MountAndRegisterPak(const FString& PakFilePath);

	UFUNCTION(BlueprintCallable, Category = "PAK")
	bool UnmountAndRegisterPak(FString PakFilePath);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PAK")
	UClass* LoadPakObjClassReference(FString PakContentPath);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PAK")
	UClass* LoadPakFileClass(const FString& FileName);
	


private:	
	FString Conv_PakContentPathToReferenceString(const FString PakContentPath, const FString PakMountPath);

	
protected:
	class FPakPlatformFile* PakPlatformFile = nullptr;
	class IPlatformFile* PlatformFileRef = nullptr;
	
};
