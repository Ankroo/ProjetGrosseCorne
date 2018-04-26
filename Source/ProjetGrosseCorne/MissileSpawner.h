// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>
#include "../Intermediate/ProjectFiles/Frequency.h"
#include "../Intermediate/ProjectFiles/Loudness.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.h"
#include "MissileSpawner.generated.h"

UCLASS()
class PROJETGROSSECORNE_API AMissileSpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UClass* ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ULightComponent* flash;

	float GlobalTime;
	float TimeSinceLastSpawn;

	bool PlayerKilled;

	std::vector<Loudness> loud;
	std::vector<Frequency> freq;

	int FrequencyIndex;
	int LoudnessIndex;
	
public:	
	// Sets default values for this actor's properties
	AMissileSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	public: void APlayerKilled();
	
	void Flash(FVector SpawnLocation);
};
