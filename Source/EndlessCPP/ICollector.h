// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICollector.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICollector : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENDLESSCPP_API IICollector
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CollectCoin(int Amount);
};
