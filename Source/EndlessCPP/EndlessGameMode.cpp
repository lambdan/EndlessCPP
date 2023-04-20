// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessGameMode.h"

#include "HealthComponent.h"
#include "WorldMover.h"
#include "Kismet/GameplayStatics.h"

AEndlessGameMode::AEndlessGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEndlessGameMode::AddScore(int amount)
{
	if (GameOver)
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
	return CurrentSpeedFactor;
	// return StartingSpeedFactor + (GetGameTimeSinceCreation() / SpeedFactorDivisionFactor);
}

void AEndlessGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(!PlayerIsHurt())
	{
		CurrentSpeedFactor += (DeltaSeconds / SpeedFactorDivisionFactor);
	}
	
}


void AEndlessGameMode::BeginPlay()
{
	Super::BeginPlay();
	Score = 0;
	GameOver = false;
	CurrentSpeedFactor = StartingSpeedFactor;

	// bind score ticker (add score every sec)
	AddScoreDelegate.BindUFunction(this, "AddScore", ScoreTickAmount);
	GetWorld()->GetTimerManager().SetTimer(AddScoreHandle, AddScoreDelegate, ScoreTickRate, true);

	// bind death event
	PlayerHealth = Cast<UHealthComponent>(GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetComponentByClass(UHealthComponent::StaticClass()));
	check(PlayerHealth!=nullptr);
	PlayerDiedDelegate.BindUFunction(this, "PlayerDied");
	PlayerHealth->OnDied.Add(PlayerDiedDelegate);
	
}

void AEndlessGameMode::PlayerDied()
{
	UE_LOG(LogTemp, Warning, TEXT("Player died"));
	GameOver = true;
	GetWorld()->GetTimerManager().ClearTimer(AddScoreHandle); // stop score ticker

	
	// UGameplayStatics::SetGamePaused(GetWorld(), true);
	OnGameOver.Broadcast();
	
}

bool AEndlessGameMode::IsGameOver()
{
	return GameOver;
}

bool AEndlessGameMode::PlayerIsHurt()
{
	return PlayerHealth->GetCurrentHealth() < PlayerHealth->GetMaxHealth();
}

AWorldMover* AEndlessGameMode::GetWorldMover()
{
	return WorldMover;
}

void AEndlessGameMode::SetWorldMover(AWorldMover* NewWorldMover)
{
	WorldMover = NewWorldMover;
}

void AEndlessGameMode::SetSpeedFactor(float NewSpeedFactor)
{
	CurrentSpeedFactor = NewSpeedFactor;
}

void AEndlessGameMode::ReduceSpeed(float Factor)
{
	CurrentSpeedFactor -= (CurrentSpeedFactor - StartingSpeedFactor) / Factor;
	CurrentSpeedFactor = FMath::Max(StartingSpeedFactor, CurrentSpeedFactor); // never go below starting speed
}

float AEndlessGameMode::GetWorldMoveAmount()
{
	return WorldMoveAmount;
}

float AEndlessGameMode::GetKilometersPerHour()
{
	// TODO double check this math...
	auto TicksPerHour = (1/WorldMover->GetActorTickInterval()) * 60 * 60;
	auto CMperHour = (GetSpeedFactor() * GetWorldMoveAmount()) * TicksPerHour;
	return CMperHour/100000;
}

float AEndlessGameMode::GetStartingSpeedFactor()
{
	return StartingSpeedFactor;
}

