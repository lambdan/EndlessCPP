// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTookDamageDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRestoredHealthDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInvincibilityTimerStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInvincibilityTimerEndedDelegate);

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
	bool CurrentlyInvincible = false;

	UPROPERTY(EditAnywhere)
	bool bFlashDuringInvincibility = true;
	
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
	UFUNCTION()
	void StartInvincibilityTimer();
	UFUNCTION()
	void StopInvincibilityTimer();

	UPROPERTY(BlueprintAssignable)
	FTookDamageDelegate OnTookDamage;

	UPROPERTY(BlueprintAssignable)
	FRestoredHealthDelegate OnRestoredHealth;

	UPROPERTY(BlueprintAssignable)
	FHealthChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FDiedDelegate OnDied;

	UPROPERTY(BlueprintAssignable)
	FInvincibilityTimerStarted OnInvincibilityStarted;
	
	UPROPERTY(BlueprintAssignable)
	FInvincibilityTimerEndedDelegate OnInvincibilityEnded;

	UPROPERTY(EditAnywhere)
	float InvincibilityDuration = 2;

	UPROPERTY(EditAnywhere)
	float InvincibilityFlashRate = 0.2f;

	FTimerHandle InvincibilityTimerHandle;
	FTimerDelegate InvincibilityTimerDelegate;

	FTimerHandle InvincibilityFlashingTimerHandle;
	FTimerDelegate InvincibilityFlashingTimerDelegate;

	UFUNCTION(BlueprintPure)
	bool IsInvincible();

	UFUNCTION()
	void ToggleHidden();

	UFUNCTION(BlueprintPure)
	int GetMaxHealth();

	UFUNCTION(BlueprintPure)
	bool IsDead();
};
