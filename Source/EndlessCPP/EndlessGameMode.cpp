// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessGameMode.h"

AEndlessGameMode::AEndlessGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEndlessGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AEndlessGameMode::BeginPlay()
{
	Super::BeginPlay();
}