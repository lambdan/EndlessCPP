// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlienScript.generated.h"

UCLASS()
class ENDLESSCPP_API AAlienScript : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlienScript();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool UpdateRotation();

	UFUNCTION()
	void TryFireAtPlayer();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AlienProjectileBlueprint;

	UPROPERTY(EditAnywhere)
	float DistanceToPlayerToStartFiring = 7000;

	UPROPERTY(EditAnywhere)
	float FireInterval = 2.0;

	FActorSpawnParameters ProjectileSpawnParams;

	FTimerDelegate FireTimerDelegate;
	FTimerHandle FireTimerHandle;

};
