// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessGameMode.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverDelegate);

UCLASS()
class ENDLESSCPP_API AEndlessGameMode : public AGameModeBase
{
	AEndlessGameMode();
	GENERATED_BODY()


protected:

public:
	UPROPERTY(EditAnywhere)
	int StartingPlayerHealth = 3;

	UPROPERTY(EditAnywhere)
	int PlayerMaxHealth = 3;

	UPROPERTY(EditAnywhere, Category="Speed")
	float StartingSpeedFactor = 1.0;

	UPROPERTY(EditAnywhere, Category="Speed")
	float MinSpeedFactor = 1.0;

	UPROPERTY(EditAnywhere, Category="Speed")
	float MaxSpeedFactor = 10.0;
	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> WorldMoverBlueprint;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	int PlayerAmount = 1;
	int PlayersDead = 0;

	UFUNCTION()
	void AddDeath();

	UPROPERTY(BlueprintAssignable)
	FGameOverDelegate OnGameOver;
	
};
