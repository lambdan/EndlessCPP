// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessPlayerState.h"

void AEndlessPlayerState::BeginPlay()
{
	Super::BeginPlay();
	Score = 0;
	StartingSpeedFactor = Cast<AEndlessGameMode>(GetWorld()->GetAuthGameMode())->StartingSpeedFactor;

	// bind score ticker (add score every sec)
	AddScoreDelegate.BindUFunction(this, "AddScore", 1);
	GetWorld()->GetTimerManager().SetTimer(AddScoreHandle, AddScoreDelegate, 0.1, true);

	

	GameMode = Cast<AEndlessGameMode>(GetWorld()->GetAuthGameMode());
	check(GameMode!=nullptr);

	CurrentSpeedFactor = StartingSpeedFactor;
}

float AEndlessPlayerState::GetKilometersPerHour()
{
	return CurrentSpeedFactor * GameMode->WorldMoveAmount * (1/GameMode->WorldMoveTickrate) * 60 * 60 / 100000;
}

void AEndlessPlayerState::SetWorldMover(AWorldMover* NewWorldMover)
{
	WorldMover = NewWorldMover;
	WorldMover->SetSpeed(CurrentSpeedFactor);
}

void AEndlessPlayerState::SetSpeedFactor(float NewSpeedFactor)
{
	CurrentSpeedFactor = NewSpeedFactor;
	WorldMover->SetSpeed(CurrentSpeedFactor);
}

void AEndlessPlayerState::ReduceSpeed(float Factor)
{
	// CurrentSpeedFactor -= 0.5;
		CurrentSpeedFactor -= (CurrentSpeedFactor - StartingSpeedFactor) / Factor;
		CurrentSpeedFactor = FMath::Max(StartingSpeedFactor, CurrentSpeedFactor); // never go below starting speed
	WorldMover->SetSpeed(CurrentSpeedFactor);
}


void AEndlessPlayerState::AddScore(int Amount)
{
	Score += Amount * CurrentSpeedFactor;
	CurrentSpeedFactor += 1.0/200.0;
	WorldMover->SetSpeed(CurrentSpeedFactor);

// 	if(!PlayerIsHurt())
// 	{
// 		CurrentSpeedFactor += (1 / SpeedFactorDivisionFactor);
// 	}
// 	
	OnScoreUpdatedDelegate.Broadcast();
	
}



void AEndlessPlayerState::Died()
{
	UE_LOG(LogTemp, Warning, TEXT("Died in player state"));
	GetWorld()->GetTimerManager().ClearTimer(AddScoreHandle);
	OnGameOver.Broadcast();
}


int64 AEndlessPlayerState::GetCurrentScore()
{
	return Score;
}



