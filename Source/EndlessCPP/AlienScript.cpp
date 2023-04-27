// Fill out your copyright notice in the Description page of Project Settings.


#include "AlienScript.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAlienScript::AAlienScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

// Called when the game starts or when spawned
void AAlienScript::BeginPlay()
{
	Super::BeginPlay();

	FireTimerDelegate.BindUFunction(this, "TryFireAtPlayer");
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, FireTimerDelegate, FireInterval, true);
}

// Called every frame
void AAlienScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AAlienScript::UpdateRotation()
{
	if(GetInstigator()==nullptr)
	{
		return false; // player is dead
	}

	auto PlayerLocation = GetInstigator()->GetActorLocation();

	auto NewRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), PlayerLocation);

	this->SetActorRotation(NewRotation, ETeleportType::TeleportPhysics);

	return true;
}

void AAlienScript::TryFireAtPlayer()
{
	if(GetInstigator()==nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s instigator: %s"), *this->GetActorNameOrLabel(), *GetInstigator()->GetActorNameOrLabel());
	
	auto PlayerLocation = GetInstigator()->GetActorLocation();
	auto DistanceToPlayer = FVector::Distance(this->GetActorLocation(), PlayerLocation);

	if(DistanceToPlayer < DistanceToPlayerToStartFiring)
	{
		UpdateRotation();
		// spawn projectile
		auto SpawnLocation = this->GetActorLocation() + (this->GetActorForwardVector() * 10);
		FTransform ProjectileSpawnTransform;
		ProjectileSpawnTransform.SetLocation(SpawnLocation);
		ProjectileSpawnTransform.SetRotation(this->GetActorQuat());
		auto Projectile = GetWorld()->SpawnActor<AActor>(AlienProjectileBlueprint, ProjectileSpawnTransform, ProjectileSpawnParams);
		
	}
}

