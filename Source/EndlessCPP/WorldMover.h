// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndlessGameMode.h"
#include "WorldMover.generated.h"



UCLASS()
class ENDLESSCPP_API AWorldMover : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldMover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int BlockLength = 0;

public:	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GroundPieceBlueprint;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ObstacleBlueprints;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> CollectibleBlueprints;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> HealthBlueprints;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> EnemyBlueprints;
	
	UPROPERTY(EditAnywhere)
	float CollectibleSpawnProbability = 0.3f;

	UPROPERTY(EditAnywhere)
	float EnemySpawnProbability = 0.1f;
	
	UPROPERTY(EditAnywhere)
	int BlocksBehindPlayer = 10;

	UPROPERTY(EditAnywhere)
	int BlocksAheadOfPlayer = 40;
	
	
	
	UPROPERTY()
	TArray<AActor*> SpawnedGroundPieces;

	UPROPERTY()
	TArray<AActor*> SpawnedObjects;

	UPROPERTY()
	float LastObstacleOrCollectibleSpawn;
	
	FVector CalculateGroundPieceSpawnPosition();
	void SpawnGroundPiece(FVector Position);
	void SpawnObstacleOrCollectible();
	void SpawnEnemy();

	AEndlessGameMode* GameMode;
	

	UFUNCTION()
	void MoveWorld();

	FTimerHandle WorldMoveTimerHandle;
	FTimerDelegate WorldMoveTimerDelegate;

	float Speed = 1.0;
	bool PlayerIsHurt = false;
	
	UFUNCTION(BlueprintCallable)
	void SetSpeed(float NewSpeed);

	UFUNCTION(BlueprintCallable)
	void SetPlayerHurt(bool HurtState);

};
