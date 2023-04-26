// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessGameMode.generated.h"


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
	float SpeedFactorDivisionFactor = 200;

	UPROPERTY(EditAnywhere, Category = "Scoring")
	float ScoreTickRate = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Scoring")
	int ScoreTickAmount = 1;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float WorldMoveTickrate = 1.0 / 100.0;
	
	UPROPERTY(EditAnywhere)
	float WorldMoveAmount = 30;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
};
