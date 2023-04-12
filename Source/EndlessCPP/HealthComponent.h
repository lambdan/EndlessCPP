// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTookDamageDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRestoredHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENDLESSCPP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int CurrentHealth;
	int MaxHealth;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(int amount);

	UFUNCTION(BlueprintCallable)
	void RestoreHealth(int amount);
	
	void Died();

	UFUNCTION(BlueprintPure)
	int GetCurrentHealth();

	UPROPERTY(BlueprintAssignable)
	FTookDamageDelegate OnTookDamage;

	UPROPERTY(BlueprintAssignable)
	FRestoredHealthDelegate OnRestoredHealth;

	UPROPERTY(BlueprintAssignable)
	FHealthChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FDiedDelegate OnDied;
	
};
