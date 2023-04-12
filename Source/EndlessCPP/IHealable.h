// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHealable.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIHealable : public UInterface
{
	GENERATED_BODY()
};

class ENDLESSCPP_API IIHealable
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveHeal(int HealAmount);
};


