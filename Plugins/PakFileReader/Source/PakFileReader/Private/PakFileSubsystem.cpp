// Copyright Epic Games, Inc. All Rights Reserved.


#include "PakFileSubsystem.h"
#include "Misc/PackageName.h"
#include "IPlatformFilePak.h"

void UPakFileSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	
	IPlatformFile* ExistingPlatformFile = FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"));
	if(ExistingPlatformFile)
	{
		PakPlatformFile = static_cast<FPakPlatformFile*>(ExistingPlatformFile);
	}
	else
	{
		PakPlatformFile = new FPakPlatformFile();
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		PlatformFileRef = &PlatformFile;
		PakPlatformFile->Initialize(&PlatformFile, TEXT(""));
		FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
	}

	if (PakPlatformFile == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get PakPlatformFile"));
		return; 
	}
	
	UE_LOG(LogTemp, Log, TEXT("PakPlatformFile Valid"));
}

void UPakFileSubsystem::Deinitialize()
{
	if(PlatformFileRef)
	{
		FPlatformFileManager::Get().SetPlatformFile(*PlatformFileRef);
	}
}

const bool UPakFileSubsystem::IsRunningPIE()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

FString const UPakFileSubsystem::GetPakMountPoint(const FString& PakFilePath)
{
	FPakFile* Pak = nullptr;
	TRefCountPtr<FPakFile> PakFile = new FPakFile(PakPlatformFile, *PakFilePath, false);
	Pak = PakFile.GetReference();
	if (Pak->IsValid())
	{
		return Pak->GetMountPoint();
	}
	return FString();
}

TArray<FString> UPakFileSubsystem::GetPakContent(const FString& PakFilePath, bool bOnlyCooked /*= true*/)
{
	FPakFile* Pak = nullptr;
	TRefCountPtr<FPakFile> PakFile = new FPakFile(PakPlatformFile, *PakFilePath, false);

	Pak = PakFile.GetReference();
	TArray<FString> PakContent;

	if (Pak->IsValid())
	{
		FString ContentPath, PakAppendPath;
		FString MountPoint = GetPakMountPoint(PakFilePath);
		MountPoint.Split("/Content/", &ContentPath, &PakAppendPath);


		TArray<FPakFile::FFilenameIterator> Records;
		for (FPakFile::FFilenameIterator It(*Pak, false); It; ++It)
		{
			Records.Add(It);
		}

		for (auto& It : Records)
		{
			if (bOnlyCooked)
			{
				if (FPaths::GetExtension(It.Filename()) == TEXT("uasset"))
				{
					PakContent.Add(FString::Printf(TEXT("%s%s"), *PakAppendPath, *It.Filename()));
				}
			}
			else
			{
				PakContent.Add(FString::Printf(TEXT("%s%s"), *PakAppendPath, *It.Filename()));
			}
		}
	}
	return PakContent;
}

FString UPakFileSubsystem::GetPakMountContentPath(const FString& PakFilePath)
{
	FString ContentPath, RightStr;
	bool bIsContent;
	FString MountPoint = GetPakMountPoint(PakFilePath);
	bIsContent = MountPoint.Split("/Content/", &ContentPath, &RightStr);
	if (bIsContent)
	{
		return FString::Printf(TEXT("%s/Content/"), *ContentPath);
	}
	// Check Pak Content for /Content/ Path
	else
	{
		TArray<FString> Content = GetPakContent(PakFilePath);
		if (Content.Num() > 0)
		{
			FString FullPath = FString::Printf(TEXT("%s%s"), *MountPoint, *Content[0]);
			bIsContent = FullPath.Split("/Content/", &ContentPath, &RightStr);
			if (bIsContent)
			{
				return FString::Printf(TEXT("%s/Content/"), *ContentPath);
			}
		}
	}
	//Failed to Find Content
	return FString("");
}

bool UPakFileSubsystem::MountPakFile(const FString& PakFilePath, const FString& PakMountPoint)
{
	bool bIsMounted = false;
	int32 PakOrder = 0;

	FString InputPath = PakFilePath;
	FPaths::MakePlatformFilename(InputPath);

	if (!PakMountPoint.IsEmpty())
	{
		bIsMounted = PakPlatformFile->Mount(*InputPath, PakOrder, *PakMountPoint);
	}
	else
	{
		bIsMounted = PakPlatformFile->Mount(*InputPath, PakOrder);
	}

	if (bIsMounted)
	{
		UE_LOG(LogTemp, Log, TEXT("Pak Valid: %s | MountPoint: %s"), *InputPath, *PakMountPoint);
	}
	
	return bIsMounted;
}

bool UPakFileSubsystem::UnmountPakFile(const FString& PakFilePath)
{
	FString InputPath = PakFilePath;
	FPaths::MakePlatformFilename(InputPath);

	if(PakPlatformFile->Unmount(*InputPath))
	{
		UE_LOG(LogTemp, Log, TEXT("Pak Unmount Valid: %s"), *InputPath);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Pak Unmount Failed: %s"), *InputPath);
		return false;
	}
}

void UPakFileSubsystem::RegisterMountPoint(const FString& RootPath, const FString& ContentPath)
{
	FPackageName::RegisterMountPoint(RootPath, ContentPath);
}

void UPakFileSubsystem::UnregisterMountPoint(const FString& RootPath, const FString& ContentPath)
{
	FPackageName::UnRegisterMountPoint(RootPath, ContentPath);
}

bool UPakFileSubsystem::MountAndRegisterPak(const FString& PakFilePath)
{
	FString PakRootPath = "/Game/";
	if (!PakFilePath.IsEmpty())
	{
		if (MountPakFile(PakFilePath, FString()))
		{
			const FString MountPoint = GetPakMountContentPath(PakFilePath);
			RegisterMountPoint(PakRootPath, MountPoint);
			return true;
		}
	}
	return false;

}

bool UPakFileSubsystem::UnmountAndRegisterPak(FString PakFilePath)
{
	FString PakRootPath = "/Game/";
	if (!PakFilePath.IsEmpty())
	{
		if (UnmountPakFile(PakFilePath))
		{
			const FString MountPoint = GetPakMountContentPath(PakFilePath);
			UnregisterMountPoint(PakRootPath, MountPoint);
			GEngine->ForceGarbageCollection(true);
			return true;
		}
	}
	return false;
}

UClass* UPakFileSubsystem::LoadPakObjClassReference(FString PakContentPath)
{
	FString PakRootPath = "/Game/";
	const FString FileName = Conv_PakContentPathToReferenceString(PakContentPath, PakRootPath);
	return LoadPakFileClass(FileName);
}

UClass* UPakFileSubsystem::LoadPakFileClass(const FString& FileName)
{
	const FString ClassPath = FString::Printf(TEXT("%s.%s_C"), *FileName, *FPackageName::GetShortName(FileName));
	return StaticLoadClass(UObject::StaticClass(), nullptr, *ClassPath);
}

FString UPakFileSubsystem::Conv_PakContentPathToReferenceString(const FString PakContentPath, const FString PakMountPath)
{
	return FString::Printf(TEXT("%s%s.%s"),
		*PakMountPath, *FPaths::GetBaseFilename(PakContentPath, false), *FPaths::GetBaseFilename(PakContentPath, true));
}




