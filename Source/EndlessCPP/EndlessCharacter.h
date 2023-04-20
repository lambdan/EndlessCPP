// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "IDamageable.h"
#include "IHealable.h"
#include "GameFramework/Character.h"
#include "EndlessCharacter.generated.h"

UCLASS()
class ENDLESSCPP_API AEndlessCharacter : public ACharacter, public IIDamageable, public IIHealable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEndlessCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int PositionIndex = 0; // -1 is left, 1 is right, 0 is center

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ReceiveDamage_Implementation(AActor* DamageCauser, int DamageAmount) override;
	virtual void ReceiveHeal_Implementation(int HealAmount) override;
	
	UFUNCTION(BlueprintCallable)
	void MoveLeft();

	UFUNCTION(BlueprintCallable)
	void MoveRight();

	UFUNCTION(BlueprintCallable)
	void LieDown();

	UFUNCTION(BlueprintCallable)
	void StandUp();

	UFUNCTION()
	void Died();
	
	UHealthComponent* HealthComponent;

};
