// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessLevelScript.h"
#include <Engine/World.h>

#include "EndlessGameMode.h"


AEndlessLevelScript::AEndlessLevelScript() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEndlessLevelScript::BeginPlay()
{
	Super::BeginPlay();

	GameMode = (AEndlessGameMode*)GetWorld()->GetAuthGameMode();
	
	SpawnedGroundPieces.Empty();
	SpawnedObstaclesAndCollectibles.Empty();

	// generate 10 starting pieces
	for (int i = 0; i < BlocksAheadOfPlayer; i++) {
		SpawnGroundPiece(CalculateGroundPieceSpawnPosition());
	}
}

FVector AEndlessLevelScript::CalculateGroundPieceSpawnPosition()
{
	if (SpawnedGroundPieces.Num() == 0) { // first block (put it under the player)
		return FVector(0,0,-150);
	}
	
	// other blocks: based on last block
	auto LastBlock = SpawnedGroundPieces.Last();
	auto NextBlockOffset = LastBlock->GetActorForwardVector() * BlockLength;
	auto Result = LastBlock->GetActorLocation() + NextBlockOffset;
	return Result;
}

void AEndlessLevelScript::SpawnGroundPiece(FVector Position)
{
	// when we have a bunch of pieces behind us we can just move them ahead of us instead of spawning new ones
	if(SpawnedGroundPieces.Num() >= (BlocksAheadOfPlayer+BlocksBehindPlayer)) 
	{
		auto OldestPiece = SpawnedGroundPieces[0];
		OldestPiece->SetActorLocation(Position);
		SpawnedGroundPieces.Add(OldestPiece);
		SpawnedGroundPieces.RemoveAt(0);
	}
	else
	{
		FTransform SpawnTransform = FTransform(Position);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto NewPiece = GetWorld()->SpawnActor<AActor>(GroundPieceBlueprint, SpawnTransform, SpawnParams);
		SpawnedGroundPieces.Add(NewPiece);
	}
}

void AEndlessLevelScript::SpawnObstacleOrCollectible()
{

	if(RandomThingsAndObstacles.Num() == 0)
	{
		return; // no blueprints assigned
	}

	TArray<FVector> PossibleLocations;
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, 0, 150)); // middle
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, 0, 300)); // middle top
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, -150, 150)); // left
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, -150, 300)); // left top
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, 150, 150)); // right
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, 150, 300)); // right top

	auto HowManyToSpawn = FMath::RandRange(0, PossibleLocations.Num()-1); // -1 to always leave one spot open
	
	for (int i = 0; i < HowManyToSpawn; i++)
	{
		auto LocationIndex = FMath::RandRange(0, PossibleLocations.Num()-1);
		auto Location = PossibleLocations[LocationIndex];
		FTransform SpawnTransform = FTransform(Location);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto RandomInt = FMath::RandRange(0, RandomThingsAndObstacles.Num() - 1);
		auto RandomizedThing = RandomThingsAndObstacles[RandomInt];
		auto NewSpawn = GetWorld()->SpawnActor<AActor>(RandomizedThing, SpawnTransform, SpawnParams);
		if(NewSpawn)
		{
			SpawnedObstaclesAndCollectibles.Add(NewSpawn);
			UE_LOG(LogTemp, Warning, TEXT("Spawned %s"), *NewSpawn->GetActorNameOrLabel());
			PossibleLocations.RemoveAt(LocationIndex);
		}
		
	}
}

void AEndlessLevelScript::SpawnEnemy()
{
	if(EnemyBlueprints.Num() == 0)
	{
		return;
	}
	
	TArray<FVector> PossibleLocations;
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, -400, 400));
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, -600, 400));
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, 400, 400));
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, 600, 400));

	auto HowManyToSpawn = 1; // could be made random in the future
	for (int i = 0; i < HowManyToSpawn; i++)
	{
		auto LocationIndex = FMath::RandRange(0, PossibleLocations.Num()-1);
		auto Location = PossibleLocations[LocationIndex];
		FTransform SpawnTransform = FTransform(Location);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto RandomInt = FMath::RandRange(0, EnemyBlueprints.Num() - 1);
		auto RandomizedThing = EnemyBlueprints[RandomInt];
		auto NewSpawn = GetWorld()->SpawnActor<AActor>(RandomizedThing, SpawnTransform, SpawnParams);
		if(NewSpawn)
		{
			SpawnedObstaclesAndCollectibles.Add(NewSpawn);
			UE_LOG(LogTemp, Warning, TEXT("Spawned %s"), *NewSpawn->GetActorNameOrLabel());
			PossibleLocations.RemoveAt(LocationIndex);
		}
		
	}
}



void AEndlessLevelScript::Tick(float DeltaSeconds)
{
	// check distance to player, if we're approaching the last piece: create new one
	auto LastPieceLocation = SpawnedGroundPieces.Last()->GetActorLocation();
	auto PlayerLocation = FVector(0,0,0); // player should always be at 0,0,0 anyway
	auto DistanceBetween = FVector::Distance(LastPieceLocation, PlayerLocation);
	if (DistanceBetween < (BlocksAheadOfPlayer*BlockLength)) {
		// TODO: math based on spawnedgroundpieces num (eg 40000 here and .Num() > 50 means 10 pieces will be behind player at all times
		SpawnGroundPiece(CalculateGroundPieceSpawnPosition());
	}

	// move ground towards the player
	for(int i = 0; i < SpawnedGroundPieces.Num(); i++)
	{
		SpawnedGroundPieces[i]->AddActorWorldOffset(GroundMovementEveryTick * GameMode->GetSpeedFactor());
	}

	// move collectibles/obstacles towards player
	for(int i = 0; i < SpawnedObstaclesAndCollectibles.Num(); i++)
	{
		if(SpawnedObstaclesAndCollectibles[i] == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Collectible was NULL! Removing it from array... hopefully"));
			SpawnedObstaclesAndCollectibles.RemoveAt(i);
			UE_LOG(LogTemp, Error, TEXT("Removed!"));
			continue;
		}
		
		SpawnedObstaclesAndCollectibles[i]->AddActorWorldOffset(GroundMovementEveryTick * GameMode->GetSpeedFactor());

		// check if behind player (remove it if so)
		if(SpawnedObstaclesAndCollectibles[i]->GetActorLocation().X <= -500)
		{
			UE_LOG(LogTemp, Error, TEXT("Behind player, removing"));
			SpawnedObstaclesAndCollectibles[i]->Destroy();
			SpawnedObstaclesAndCollectibles.RemoveAt(i);
		}
	}
	
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Yellow, FString::Printf(TEXT("Ground pieces in array: %i"), SpawnedGroundPieces.Num()));
	GEngine->AddOnScreenDebugMessage(3, 1, FColor::Yellow, FString::Printf(TEXT("Collectibles/obstacles in array: %i"), SpawnedObstaclesAndCollectibles.Num()));
	
	// AEndlessGameMode * GameMode = (AEndlessGameMode*)GetWorld()->GetAuthGameMode();
	// if(GameMode->GetScore() % 10 == 0)
	// {
	// 	SpawnObstacleOrCollectible();
	// 	
	// }

	if(GetGameTimeSinceCreation() - LastObstacleOrCollectibleSpawn > (1 / GameMode->GetSpeedFactor()))
	{
		LastObstacleOrCollectibleSpawn = GetGameTimeSinceCreation();
		SpawnObstacleOrCollectible();
		SpawnEnemy();
		// SpeedFactor += 0.01;
	}
	
	GEngine->AddOnScreenDebugMessage(4, 1, FColor::Yellow, FString::Printf(TEXT("Speed factor: %f"), GameMode->GetSpeedFactor()));
	Super::Tick(DeltaSeconds);
}
