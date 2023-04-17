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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UFUNCTION(BlueprintCallable)
	void AddActorToMoveWithWorld(AActor* NewActor);

};
