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
	SpawnedObjects.Empty();
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

	auto HowManyToSpawn = FMath::RandRange(0, PossibleLocations.Num()-1); // -1 to always leave one spot open
	
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
		} else
		{
			// spawn obstacle
			RandomInt = FMath::RandRange(0, ObstacleBlueprints.Num() - 1);
			RandomizedThing = ObstacleBlueprints[RandomInt];
		}
		
		auto NewSpawn = GetWorld()->SpawnActor<AActor>(RandomizedThing, SpawnTransform, SpawnParams);
		if(NewSpawn)
		{
			SpawnedObjects.Add(NewSpawn);
			UE_LOG(LogTemp, Warning, TEXT("Spawned %s"), *NewSpawn->GetActorNameOrLabel());
			PossibleLocations.RemoveAt(LocationIndex);
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
		SpawnedObjects.Add(NewSpawn);
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

	// move collectibles/obstacles towards player
	for (int i = 0; i < SpawnedObjects.Num(); i++)
	{
		if (SpawnedObjects[i] == nullptr)
		{
			SpawnedObjects.RemoveAt(i);
			continue;
		}

		SpawnedObjects[i]->AddActorWorldOffset(FVector(-10,0,0) * Speed);

		// check if object is behind player (remove it if so)
		if (SpawnedObjects[i]->GetActorLocation().X <= -500)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is behind player. Destroying"), *SpawnedObjects[i]->GetActorNameOrLabel());
			SpawnedObjects[i]->Destroy();
			SpawnedObjects.RemoveAt(i);
		}
	}


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
	for(auto & a : SpawnedObjects)
	{
		a->Destroy();
	}
}
