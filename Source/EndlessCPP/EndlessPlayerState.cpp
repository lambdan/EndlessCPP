// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessPlayerState.h"

void AEndlessPlayerState::BeginPlay()
{
	Super::BeginPlay();
	Score = 0;
	
	GameMode = Cast<AEndlessGameMode>(GetWorld()->GetAuthGameMode());
	check(GameMode!=nullptr);

	SetSpeedFactor(GameMode->StartingSpeedFactor);
	
	AddScoreDelegate.BindUFunction(this, "AddScore", 1);
	GetWorld()->GetTimerManager().SetTimer(AddScoreHandle, AddScoreDelegate, 0.1f, true);

	AddSpeedDelegate.BindUFunction(this, "AddSpeed", 0.01f);
	GetWorld()->GetTimerManager().SetTimer(AddSpeedHandle, AddSpeedDelegate, 0.1f, true);
}

float AEndlessPlayerState::GetKilometersPerHour()
{
	return CurrentSpeedFactor * GameMode->WorldMoveAmount * (1/GameMode->WorldMoveTickrate) * 60 * 60 / 100000;
}

float AEndlessPlayerState::GetSpeedFactor()
{
	return CurrentSpeedFactor;
}

void AEndlessPlayerState::SetWorldMover(AWorldMover* NewWorldMover)
{
	WorldMover = NewWorldMover;
	check(WorldMover!=nullptr);
}

void AEndlessPlayerState::SetSpeedFactor(float NewSpeedFactor)
{
	CurrentSpeedFactor = NewSpeedFactor;
	if(WorldMover == nullptr) // TODO why is this needed for 2 players...
	{
		UE_LOG(LogTemp, Error, TEXT("%s: WorldMover is null"), *GetActorNameOrLabel());
		return;
	}
	
	WorldMover->SetSpeed(CurrentSpeedFactor);
	OnSpeedUpdatedDelegate.Broadcast();
	GEngine->AddOnScreenDebugMessage(33+ GetPlayerController()->GetLocalPlayer()->GetControllerId() , 1, FColor::Yellow, FString::Printf(TEXT("%s Speed Factor %f"), *GetActorNameOrLabel() , CurrentSpeedFactor));
}

void AEndlessPlayerState::ReduceSpeed(float Factor)
{
	CurrentSpeedFactor *= 1.0/Factor;
	CurrentSpeedFactor = FMath::Max(1.0, CurrentSpeedFactor);
	SetSpeedFactor(CurrentSpeedFactor);
}


void AEndlessPlayerState::AddScore(int Amount)
{
	Score += Amount * CurrentSpeedFactor;
	OnScoreUpdatedDelegate.Broadcast();
}

void AEndlessPlayerState::AddSpeed(float Amount)
{
	CurrentSpeedFactor += Amount;
	CurrentSpeedFactor = FMath::Min(10.0, CurrentSpeedFactor);
	SetSpeedFactor(CurrentSpeedFactor);
}


void AEndlessPlayerState::Died()
{
	GetWorld()->GetTimerManager().ClearTimer(AddSpeedHandle);
	GetWorld()->GetTimerManager().ClearTimer(AddScoreHandle);
	OnGameOver.Broadcast();
}


int64 AEndlessPlayerState::GetCurrentScore()
{
	return Score;
}



