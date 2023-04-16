// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EndlessGameMode.h"
#include "EndlessLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSCPP_API AEndlessLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	AEndlessLevelScript();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GroundPieceBlueprint;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> RandomThingsAndObstacles;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> EnemyBlueprints;

	UPROPERTY(EditAnywhere)
	FVector GroundMovementEveryTick = FVector(-15,0,0);
	
	UPROPERTY(EditAnywhere)
	int BlocksBehindPlayer = 10;

	UPROPERTY(EditAnywhere)
	int BlocksAheadOfPlayer = 40;

	UPROPERTY(EditAnywhere)
	int BlockLength = 1000;
	
	UPROPERTY()
	TArray<AActor*> SpawnedGroundPieces;

	UPROPERTY()
	TArray<AActor*> SpawnedObstaclesAndCollectibles;

	UPROPERTY()
	float LastObstacleOrCollectibleSpawn;
	
	FVector CalculateGroundPieceSpawnPosition();
	void SpawnGroundPiece(FVector Position);
	void SpawnObstacleOrCollectible();
	void SpawnEnemy();

	AEndlessGameMode* GameMode;

protected:
	virtual void Tick(float DeltaSeconds) override;

};
