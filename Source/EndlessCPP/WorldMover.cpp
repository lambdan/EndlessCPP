// Yes, this class should really be called WorldManager or something....

#include "WorldMover.h"

AWorldMover::AWorldMover()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorldMover::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = (AEndlessGameMode*)GetWorld()->GetAuthGameMode();
	
	SpawnedGroundPieces.Empty();
	SpawnedEnemies.Empty();
	SpawnedCollectibles.Empty();
	SpawnedObstacles.Empty();
	ShouldSpawn = true;
	
	// generate starting pieces
	for (int i = 0; i < BlocksAheadOfPlayer; i++) {
		SpawnGroundPiece(CalculateGroundPieceSpawnPosition());
	}
	
	WorldMoveTimerDelegate.BindUFunction(this, "MoveWorld");
	GetWorld()->GetTimerManager().SetTimer(WorldMoveTimerHandle, WorldMoveTimerDelegate, 0.01, true);
}

FVector AWorldMover::CalculateGroundPieceSpawnPosition()
{
	if (SpawnedGroundPieces.IsEmpty()) { // first block (put it under the player)
		// return FVector(0,0,-150);
		return this->GetActorLocation() + FVector(0,0,-150);
	}
	
	// other blocks: based on last block
	auto LastBlock = SpawnedGroundPieces.Last();
	auto NextBlockOffset = LastBlock->GetActorForwardVector() * BlockLength;
	auto Result = LastBlock->GetActorLocation() + NextBlockOffset;
	return Result;
}

void AWorldMover::SpawnGroundPiece(FVector Position)
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
		
		if(BlockLength==0)
		{
			BlockLength = NewPiece->GetActorScale().X * 100;
		}
		
		SpawnedGroundPieces.Add(NewPiece);
	}
}

void AWorldMover::SpawnObstacleOrCollectible()
{
	if(ObstacleBlueprints.IsEmpty())
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

	auto HowManyToSpawn = FMath::RandRange(3, PossibleLocations.Num()-1); // -1 to always leave one spot open
	
	for (int i = 0; i < HowManyToSpawn; i++)
	{
		auto LocationIndex = FMath::RandRange(0, PossibleLocations.Num()-1);
		auto Location = PossibleLocations[LocationIndex];
		FTransform SpawnTransform = FTransform(Location);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		int32 RandomInt;
		TSubclassOf<AActor> RandomizedThing;
		if(FMath::RandRange(0.0f, 1.0f) <= CollectibleSpawnProbability)
		{
			if(PlayerIsHurt) // spawn heaalth is player is hurt
			{
				RandomInt = FMath::RandRange(0, HealthBlueprints.Num() - 1);
				RandomizedThing = HealthBlueprints[RandomInt];
			} else // otherwise spawn coins
			{
				RandomInt = FMath::RandRange(0, CollectibleBlueprints.Num() - 1);
				RandomizedThing = CollectibleBlueprints[RandomInt];
			}
			
			auto NewSpawn = GetWorld()->SpawnActor<AActor>(RandomizedThing, SpawnTransform, SpawnParams);
			if(NewSpawn)
			{
				SpawnedCollectibles.Add(NewSpawn);
				UE_LOG(LogTemp, Warning, TEXT("Spawned %s"), *NewSpawn->GetActorNameOrLabel());
				PossibleLocations.RemoveAt(LocationIndex);
			}
		} else
		{
			// spawn obstacle
			RandomInt = FMath::RandRange(0, ObstacleBlueprints.Num() - 1);
			RandomizedThing = ObstacleBlueprints[RandomInt];
			auto NewSpawn = GetWorld()->SpawnActor<AActor>(RandomizedThing, SpawnTransform, SpawnParams);
			if(NewSpawn)
			{
				SpawnedObstacles.Add(NewSpawn);
				UE_LOG(LogTemp, Warning, TEXT("Spawned %s"), *NewSpawn->GetActorNameOrLabel());
				PossibleLocations.RemoveAt(LocationIndex);
			}
		}
		

		
	}

	if (HowManyToSpawn > 0)
	{
		// fill in the blank spots with invisible triggers
		for (auto& a : PossibleLocations)
		{
			FTransform SpawnTransform = FTransform(a);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			auto NewSpawn = GetWorld()->SpawnActor<AActor>(OpenSpotBlueprint, SpawnTransform, SpawnParams);
			if (NewSpawn)
			{
				SpawnedObstacles.Add(NewSpawn);
				UE_LOG(LogTemp, Warning, TEXT("Spawned %s"), *NewSpawn->GetActorNameOrLabel());
			}
		}
	}
	
}

void AWorldMover::SpawnEnemy()
{
	if(EnemyBlueprints.IsEmpty() || FMath::RandRange(0.0f, 1.0f) >= EnemySpawnProbability)
	{
		return; // no enemy blueprints assigned or probability not hit
	}
	
	TArray<FVector> PossibleLocations;
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, -400, 400));
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, -600, 400));
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, 400, 400));
	PossibleLocations.Add(CalculateGroundPieceSpawnPosition() + FVector(0, 600, 400));

	auto LocationIndex = FMath::RandRange(0, PossibleLocations.Num() - 1);
	auto Location = PossibleLocations[LocationIndex];
	FTransform SpawnTransform = FTransform(Location);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto RandomInt = FMath::RandRange(0, EnemyBlueprints.Num() - 1);
	auto RandomizedThing = EnemyBlueprints[RandomInt];
	auto NewSpawn = GetWorld()->SpawnActor<AActor>(RandomizedThing, SpawnTransform, SpawnParams);
	if (NewSpawn)
	{
		SpawnedEnemies.Add(NewSpawn);
		UE_LOG(LogTemp, Warning, TEXT("Spawned %s"), *NewSpawn->GetActorNameOrLabel());
		PossibleLocations.RemoveAt(LocationIndex);
	}
	
}



void AWorldMover::MoveWorld()
{
	
	// check distance to player, if we're approaching the last piece: create new one
	auto LastPieceLocation = SpawnedGroundPieces.Last()->GetActorLocation();
	auto DistanceToPlayer = LastPieceLocation.X; // player is always at X = 0
	if (DistanceToPlayer < (BlocksAheadOfPlayer * BlockLength))
	{
		SpawnGroundPiece(CalculateGroundPieceSpawnPosition());

	}

	// move ground towards the player
	for (int i = 0; i < SpawnedGroundPieces.Num(); i++)
	{
		SpawnedGroundPieces[i]->AddActorWorldOffset(FVector(-10,0,0) * Speed);
	}
	
	MoveActors(SpawnedObstacles);
	MoveActors(SpawnedEnemies);
	MoveActors(SpawnedCollectibles);
	
	if (GetGameTimeSinceCreation() - LastObstacleOrCollectibleSpawn > (1 * GameMode->StartingSpeedFactor / Speed))
	{
		LastObstacleOrCollectibleSpawn = GetGameTimeSinceCreation();
		if(ShouldSpawn)
		{
			SpawnObstacleOrCollectible();
			SpawnEnemy();
		}
		
	}
}

void AWorldMover::MoveActors(TArray<AActor*>& ActorsArray)
{
	for (int i = 0; i < ActorsArray.Num(); i++)
	{
		if (ActorsArray[i] == nullptr)
		{
			ActorsArray.RemoveAt(i);
			continue;
		}

		ActorsArray[i]->AddActorWorldOffset(FVector(-10,0,0) * Speed);

		// check if object is behind player (remove it if so)
		if (ActorsArray[i]->GetActorLocation().X <= -500)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is behind player. Destroying"), *ActorsArray[i]->GetActorNameOrLabel());
			ActorsArray[i]->Destroy();
			ActorsArray.RemoveAt(i);
		}
	}
}

void AWorldMover::SetSpeed(float NewSpeed)
{
	Speed = NewSpeed;
}

void AWorldMover::SetPlayerHurt(bool HurtState)
{
	PlayerIsHurt = HurtState;
}

void AWorldMover::StopSpawning()
{
	ShouldSpawn = false;
}

void AWorldMover::DespawnEnemiesAndObstacles()
{
	for(auto & a : SpawnedEnemies)
	{
		a->Destroy();
	}
	for(auto & a : SpawnedObstacles)
	{
		a->Destroy();
	}
}


