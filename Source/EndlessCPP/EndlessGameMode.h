// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
// #include "WorldMover.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessGameMode.generated.h"

class WorldMover;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScoreUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverTriggered);

UCLASS()
class ENDLESSCPP_API AEndlessGameMode : public AGameModeBase
{
	AEndlessGameMode();
	GENERATED_BODY()


protected:
	float CurrentSpeedFactor;

public:
	int64 Score = 0; // int64 in case someone goes for a guiness world record

	UPROPERTY(EditAnywhere)
	int StartingPlayerHealth = 3;

	UPROPERTY(EditAnywhere)
	int PlayerMaxHealth = 3;

	UPROPERTY(EditAnywhere)
	float StartingSpeedFactor = 1.0;
	
	UPROPERTY(EditAnywhere)
	float SpeedFactorDivisionFactor = 200;

	UPROPERTY(EditAnywhere, Category = "Scoring")
	float ScoreTickRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Scoring")
	int ScoreTickAmount = 1;

	UFUNCTION(BlueprintCallable)
	void AddScore(int amount);

	UFUNCTION(BlueprintPure)
	int64 GetScore();

	UFUNCTION(BlueprintPure)
	float GetSpeedFactor();

	UPROPERTY(BlueprintAssignable)
	FScoreUpdatedDelegate OnScoreUpdatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FGameOverTriggered OnGameOver;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	bool GameOver = false;

	UFUNCTION(BlueprintCallable)
	void PlayerDied();

	FTimerHandle AddScoreHandle;
	FTimerDelegate AddScoreDelegate;

	// binding events https://forums.unrealengine.com/t/binding-events-in-c/36370/2
	TScriptDelegate<FWeakObjectPtr> PlayerDiedDelegate;

	UFUNCTION(BlueprintPure)
	bool IsGameOver();

	UFUNCTION(BlueprintPure)
	bool PlayerIsHurt();

	UFUNCTION(BlueprintPure)
	AWorldMover* GetWorldMover();

	UHealthComponent* PlayerHealth;
	AWorldMover* WorldMover;

	void SetWorldMover(AWorldMover* NewWorldMover);

	UFUNCTION(BlueprintCallable)
	void SetSpeedFactor(float NewSpeedFactor);

	UFUNCTION(BlueprintCallable)
	void ReduceSpeed(float Factor);

	UPROPERTY(EditAnywhere)
	float WorldMoveAmount = 30;

	UFUNCTION(BlueprintPure)
	float GetWorldMoveAmount();

	UFUNCTION(BlueprintPure)
	float GetKilometersPerHour();

	UFUNCTION(BlueprintPure)
	float GetStartingSpeedFactor();
};
