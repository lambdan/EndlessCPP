// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessGameMode.h"

#include "EndlessCharacter.h"
#include "EndlessGameInstance.h"
#include "EndlessPlayerState.h"
#include "Kismet/GameplayStatics.h"



AEndlessGameMode::AEndlessGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEndlessGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEndlessGameMode::AddDeath()
{
	PlayersDead += 1;
	if(PlayersDead==PlayerAmount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALL PLAYERS DEAD!"));
		OnGameOver.Broadcast();
	}
}


void AEndlessGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayersDead = 0;
	
	auto EndlessGameInstance = Cast<UEndlessGameInstance>(GetGameInstance());
	PlayerAmount = EndlessGameInstance->GetPlayerAmount();
	
	UE_LOG(LogTemp, Warning, TEXT("Player count is %i"), PlayerAmount);

	for(int i = 0; i < PlayerAmount; i++)
	{
		auto NewPlayer = UGameplayStatics::GetPlayerController(GetWorld(), i);
		if (NewPlayer == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Creating player for id %i"), i);
			NewPlayer = UGameplayStatics::CreatePlayer(GetWorld(), i, true);
		}
		auto NewPawn = NewPlayer->GetPawnOrSpectator();
		auto StartLocation = FVector(0, 2000*i, 0);
		NewPawn->SetActorLocation(StartLocation);
		auto Character = Cast<AEndlessCharacter>(NewPawn);
		Character->SetStartPosition(StartLocation);

		FTransform SpawnTransform = FTransform(StartLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = NewPawn;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto NewWorldMover = GetWorld()->SpawnActor<AWorldMover>(WorldMoverBlueprint, SpawnTransform, SpawnParams);

		auto PlayerState = NewPlayer->GetPlayerState<AEndlessPlayerState>();
		PlayerState->SetWorldMover(NewWorldMover);

		PlayerState->OnGameOver.AddDynamic(this, &AEndlessGameMode::AddDeath);
		
	}
	
}
