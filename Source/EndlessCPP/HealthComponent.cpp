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

	OnHealthChanged.Broadcast(); // so the HUD updates 
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
	if(CurrentlyInvincible || CurrentHealth <= 0) // I-frames or already dead
	{
		return;
	}
	
	CurrentHealth -= amount;
	CurrentHealth = FMath::Max(0, CurrentHealth); // prevent negative health
	OnTookDamage.Broadcast();

	if(CurrentHealth <= 0)
	{
		Died();
	} else {
		StartInvincibilityTimer();
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

void UHealthComponent::StartInvincibilityTimer()
{
	CurrentlyInvincible = true;
	OnInvincibilityStarted.Broadcast();
	InvincibilityTimerDelegate.BindUFunction(this, "StopInvincibilityTimer");
	GetWorld()->GetTimerManager().SetTimer(InvincibilityTimerHandle, InvincibilityTimerDelegate, InvincibilityDuration, false);

	if(bFlashDuringInvincibility)
	{
		InvincibilityFlashingTimerDelegate.BindUFunction(this, "ToggleHidden");
		GetWorld()->GetTimerManager().SetTimer(InvincibilityFlashingTimerHandle, InvincibilityFlashingTimerDelegate, InvincibilityFlashRate, true);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Invincibility started"));
}

void UHealthComponent::StopInvincibilityTimer()
{
	CurrentlyInvincible = false;
	GetWorld()->GetTimerManager().ClearTimer(InvincibilityTimerHandle);
	OnInvincibilityEnded.Broadcast();

	if(bFlashDuringInvincibility)
	{
		GetWorld()->GetTimerManager().ClearTimer(InvincibilityFlashingTimerHandle);
		GetOwner()->SetActorHiddenInGame(false);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Invincibility ended"));
}

bool UHealthComponent::IsInvincible()
{
	return CurrentlyInvincible;
}

void UHealthComponent::ToggleHidden()
{
	GetOwner()->SetActorHiddenInGame(!GetOwner()->IsHidden());
}

int UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}
