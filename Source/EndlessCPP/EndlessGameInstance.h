// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EndlessGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSCPP_API UEndlessGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int PlayerAmount = 1;

	UFUNCTION(BlueprintCallable)
	void SetPlayerAmount(int NewPlayerAmount);

	UFUNCTION(BlueprintPure)
	int GetPlayerAmount();
};
