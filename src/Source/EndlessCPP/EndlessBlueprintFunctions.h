// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EndlessBlueprintFunctions.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSCPP_API UEndlessBlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure)
	static TArray<int64> TopScores(const TArray<int64> ScoresIn, const int TopAmount);
};
