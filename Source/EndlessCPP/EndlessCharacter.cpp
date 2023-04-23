// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessCharacter.h"

// Sets default values
AEndlessCharacter::AEndlessCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEndlessCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent = Cast<UHealthComponent>(GetComponentByClass(UHealthComponent::StaticClass()));
	check(HealthComponent != nullptr);

	HealthComponent->OnDied.AddDynamic(this, &AEndlessCharacter::Died);
}

// Called every frame
void AEndlessCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEndlessCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEndlessCharacter::ReceiveDamage_Implementation(AActor* DamageCauser, int DamageAmount)
{
	IIDamageable::ReceiveDamage_Implementation(DamageCauser, DamageAmount);
	HealthComponent->TakeDamage(DamageAmount);
	// UE_LOG(LogTemp, Error, TEXT("Took damage in code"));
}

void AEndlessCharacter::ReceiveHeal_Implementation(int HealAmount)
{
	IIHealable::ReceiveHeal_Implementation(HealAmount);
	HealthComponent->RestoreHealth(HealAmount);
	// UE_LOG(LogTemp, Error, TEXT("Healed in code"));
}

void AEndlessCharacter::MoveLeft()
{
	auto NewLocation = GetActorLocation();
	NewLocation.Y -= 150;
	NewLocation.Y = FMath::Max(-150, NewLocation.Y);
	SetActorLocation(NewLocation);
}

void AEndlessCharacter::MoveRight()
{
	auto NewLocation = GetActorLocation();
	NewLocation.Y += 150;
	NewLocation.Y = FMath::Min(150, NewLocation.Y);
	SetActorLocation(NewLocation);
}

void AEndlessCharacter::LieDown()
{
	auto NewRotation = GetActorRotation();
	NewRotation.Pitch = 270;
	SetActorRotation(NewRotation);
}

void AEndlessCharacter::StandUp()
{
	auto NewRotation = GetActorRotation();
	NewRotation.Pitch = 0;
	SetActorRotation(NewRotation);
}

void AEndlessCharacter::Died()
{
	// UE_LOG(LogTemp, Warning, TEXT("Died in code"));
	SetActorHiddenInGame(true);
}


