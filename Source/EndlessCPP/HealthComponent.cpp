// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "EndlessGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AEndlessGameMode* GameMod = (AEndlessGameMode*)GetWorld()->GetAuthGameMode();
	CurrentHealth = GameMod->StartingPlayerHealth;
	MaxHealth = GameMod->PlayerMaxHealth;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	GEngine->AddOnScreenDebugMessage(5, 1, FColor::Red, FString::Printf(TEXT("CurrentHealth: %i"), CurrentHealth));
}

void UHealthComponent::TakeDamage(int amount)
{
	CurrentHealth -= amount;
	CurrentHealth = FMath::Max(0, CurrentHealth); // prevent negative health
	OnTookDamage.Broadcast();

	if(CurrentHealth <= 0)
	{
		Died();
	}

	OnHealthChanged.Broadcast();
}

void UHealthComponent::RestoreHealth(int amount)
{
	CurrentHealth += amount;
	CurrentHealth = FMath::Min(CurrentHealth, MaxHealth);
	OnRestoredHealth.Broadcast();
	OnHealthChanged.Broadcast();
}


void UHealthComponent::Died()
{
	UE_LOG(LogDamage, Warning, TEXT("%s is dead"), *GetOwner()->GetActorNameOrLabel());
	OnDied.Broadcast();
}

int UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

