// Fill out your copyright notice in the Description page of Project Settings.
#define MAX_CHAR 255

#include "MissileSpawner.h"
#include <string>
#include <stdlib.h>
#include "PlatformFileManager.h"
#include "FileHelper.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AMissileSpawner::AMissileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	GlobalTime = 0;

	PlayerKilled = false;
	
	FrequencyIndex = 0;

	LoudnessIndex = 0;

}

// Called when the game starts or when spawned
void AMissileSpawner::BeginPlay()
{
	Super::BeginPlay();
	//Open Frequencies files
	FString Fichier;
	TArray<FString> AFichier;
	FString SaveDirectory = FString("C:/Users/Flavien/Documents/Unreal Projects/ProjetGrosseCorne/_loud_freq");
	FString FileName = FString("sonies.txt");
	bool AllowOverwriting = false;
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		// Get absolute file path
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		// Allow overwriting or file doesn't already exist
		if (PlatformFile.FileExists(*AbsoluteFilePath))
		{
			if (!FFileHelper::LoadFileToStringArray(AFichier, *AbsoluteFilePath)) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ERROR CAN'T LOAD ARRAY"));
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ERROR FILE"));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ERROR DIRECTORY TREE"));
	}
	
	for (int i = 0; i < AFichier.Num(); i++)
	{
		TArray<FString> Out;
		AFichier[i].ParseIntoArray(Out, TEXT(" : "), true);
		loud.push_back(Loudness(Out));
	}


	//FString vlcPath = "\"C:/Program Files (x86)/VideoLAN/VLC/vlc.exe\"";
	//FString musicPath = "\"C:/Users/Flavien/Documents/Unreal Projects/ProjetGrosseCorne/_loud_freq/music.mp3\"";
	//FString cmd = vlcPath + " " + musicPath;
	//GetWorld()->Exec(GetWorld(), *musicPath);
	//std::string stdMusicPath(TCHAR_TO_UTF8(*musicPath));
	//system(stdMusicPath.c_str());
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *musicPath);


	//"C:/Program Files (x86)/VideoLAN/VLC/vlc.exe" "C:/Users/Flavien/Documents/Unreal Projects/ProjetGrosseCorne/_loud_freq/music.mp3"
}

// Called every frame
void AMissileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GlobalTime += DeltaTime;

	if (GlobalTime >= loud.at(LoudnessIndex).time && !PlayerKilled) {
		LoudnessIndex++;
		FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		FVector random = FVector(0.f, rand() % 4000 - 2000, 1000.f);
		FVector missilePos = random;
		FRotator missileRotation = FRotationMatrix::MakeFromX(missilePos - playerPos).Rotator();

		//AMissile* missile = GetWorld()->SpawnActor<AMissile>(ClassToSpawn, missilePos, missileRotation);
		AMissile* missile = GetWorld()->SpawnActorDeferred<AMissile>(ClassToSpawn, FTransform(missileRotation, missilePos));
		if (missile)
		{
			missile->InitVelocity(loud.at(LoudnessIndex).value * 10 +500);
			UGameplayStatics::FinishSpawningActor(missile, FTransform(missileRotation, missilePos));
		}
		Flash(missilePos);
	}
	
}

void AMissileSpawner::APlayerKilled() {
	PlayerKilled = true;
}

void AMissileSpawner::Flash(FVector SpawnLocation) {

	//UGameplayStatics::SpawnEmitterAtLocation(this, flash, SpawnLocation, GetActorRotation());
}
