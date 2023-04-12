// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessGameMode.h"

#include "HealthComponent.h"


void AEndlessGameMode::AddScore(int amount)
{
	if(GameOver)
	{
		return;
	}
	Score += (amount * GetSpeedFactor());
	OnScoreUpdatedDelegate.Broadcast();
}

int64 AEndlessGameMode::GetScore()
{
	return Score;
}

float AEndlessGameMode::GetSpeedFactor()
{
	return StartingSpeedFactor + (GetGameTimeSinceCreation() / SpeedFactorDivisionFactor);
}

void AEndlessGameMode::BeginPlay()
{
	Super::BeginPlay();
	Score = 0;
	GameOver = false;

	// bind score ticker (add score every sec)
	AddScoreDelegate.BindUFunction(this, "AddScore", ScoreTickAmount);
	GetWorld()->GetTimerManager().SetTimer(AddScoreHandle, AddScoreDelegate, ScoreTickRate, true);

	// bind death event
	auto PlayerHealth = Cast<UHealthComponent>(GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetComponentByClass(UHealthComponent::StaticClass()));
	PlayerDiedDelegate.BindUFunction(this, "PlayerDied");
	PlayerHealth->OnDied.Add(PlayerDiedDelegate);
	
}

void AEndlessGameMode::PlayerDied()
{
	UE_LOG(LogTemp, Warning, TEXT("Player died"));
	GameOver = true;
	GetWorld()->GetTimerManager().ClearTimer(AddScoreHandle); // stop score ticker
}
