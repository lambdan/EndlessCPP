// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessGameInstance.h"

void UEndlessGameInstance::SetPlayerAmount(int NewPlayerAmount)
{
	PlayerAmount = NewPlayerAmount;
}

int UEndlessGameInstance::GetPlayerAmount()
{
	return PlayerAmount;
}
