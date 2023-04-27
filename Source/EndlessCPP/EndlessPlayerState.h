// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "WorldMover.h"
#include "GameFramework/PlayerState.h"
#include "EndlessPlayerState.generated.h"

// class WorldMover;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScoreUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpeedUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverTriggered);

/**
 * 
 */
UCLASS()
class ENDLESSCPP_API AEndlessPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UHealthComponent* HealthComponent;
	AWorldMover* WorldMover;
	AEndlessGameMode* GameMode;
	
	int64 Score = 0;
	float CurrentSpeedFactor = 1.0;
	
	UFUNCTION(BlueprintPure)
	int64 GetCurrentScore();
	
	UFUNCTION(BlueprintPure)
	float GetKilometersPerHour();

	UFUNCTION(BlueprintPure)
	float GetSpeedFactor();

	UFUNCTION(BlueprintPure)
	float GetSpeedFactorPercentage();

	UFUNCTION(BlueprintCallable)
	void SetWorldMover(AWorldMover* NewWorldMover);

	UFUNCTION(BlueprintCallable)
	void SetSpeedFactor(float NewSpeedFactor);

	UFUNCTION(BlueprintCallable)
	void ReduceSpeed(float Factor);
	
	UFUNCTION(BlueprintCallable)
	void AddScore(int Amount);

	UFUNCTION(BlueprintCallable)
	void AddSpeed(float Amount);

	
	
	UPROPERTY(BlueprintAssignable)
	FScoreUpdatedDelegate OnScoreUpdatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpeedUpdatedDelegate OnSpeedUpdatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FGameOverTriggered OnGameOver;

	FTimerHandle AddScoreHandle;
	FTimerDelegate AddScoreDelegate;

	FTimerHandle AddSpeedHandle;
	FTimerDelegate AddSpeedDelegate;
	
	virtual void BeginPlay() override;

	
	
	void Died();
};