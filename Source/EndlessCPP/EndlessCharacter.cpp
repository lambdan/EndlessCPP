#include "EndlessCharacter.h"
#include "EndlessPlayerState.h"

// Sets default values
AEndlessCharacter::AEndlessCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEndlessCharacter::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp, Warning, TEXT("Startposition is %f"), StartPosition.Y);
	
	HealthComponent = Cast<UHealthComponent>(GetComponentByClass(UHealthComponent::StaticClass()));
	check(HealthComponent != nullptr);
	
	HealthComponent->OnHealthChanged.AddDynamic(this, &AEndlessCharacter::UpdatePlayerHurtState);
	HealthComponent->OnDied.AddDynamic(this, &AEndlessCharacter::Died);
	
}

// Called every frame
void AEndlessCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEndlessCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEndlessCharacter::CollectCoin_Implementation(int Amount)
{
	IICollector::CollectCoin_Implementation(Amount);
	Execute_Dodged(this);
	Cast<AEndlessPlayerState>(GetPlayerState())->AddScore(Amount);
	
}

void AEndlessCharacter::ReceiveDamage_Implementation(AActor* DamageCauser, int DamageAmount)
{
	IIDamageable::ReceiveDamage_Implementation(DamageCauser, DamageAmount);
	Execute_ResetDodges(this);
	HealthComponent->TakeDamage(DamageAmount);
}

void AEndlessCharacter::ReceiveHeal_Implementation(int HealAmount)
{
	IIHealable::ReceiveHeal_Implementation(HealAmount);
	Execute_Dodged(this);
	HealthComponent->RestoreHealth(HealAmount);
}

void AEndlessCharacter::Dodged_Implementation()
{
	IIDodger::Dodged_Implementation();
	Cast<AEndlessPlayerState>(GetPlayerState())->AddDodge();
}

void AEndlessCharacter::ResetDodges_Implementation()
{

	IIDodger::ResetDodges_Implementation();
	Cast<AEndlessPlayerState>(GetPlayerState())->ResetDodges();
}

void AEndlessCharacter::SetStartPosition(FVector NewStartPosition)
{
	StartPosition = NewStartPosition;
}

void AEndlessCharacter::CustomJump()
{
	// this really isnt that custom, its just that i check if you're dead
	if(HealthComponent->IsDead())
	{
		return;
	}
	this->Jump();
}

void AEndlessCharacter::MoveLeft()
{
	if(HealthComponent->IsDead())
	{
		return;
	}
	auto NewLocation = GetActorLocation();
	NewLocation.Y -= 150;
	NewLocation.Y = FMath::Max(StartPosition.Y - 150, NewLocation.Y);
	SetActorLocation(NewLocation);
}

void AEndlessCharacter::MoveRight()
{
	if(HealthComponent->IsDead())
	{
		return;
	}
	auto NewLocation = GetActorLocation();
	NewLocation.Y += 150;
	NewLocation.Y = FMath::Min(StartPosition.Y + 150, NewLocation.Y);
	SetActorLocation(NewLocation);
}

void AEndlessCharacter::LieDown()
{
	if(HealthComponent->IsDead())
	{
		return;
	}
	auto NewRotation = GetActorRotation();
	NewRotation.Pitch = 270;
	SetActorRotation(NewRotation);
}

void AEndlessCharacter::StandUp()
{
	if(HealthComponent->IsDead())
	{
		return;
	}
	auto NewRotation = GetActorRotation();
	NewRotation.Pitch = 0;
	SetActorRotation(NewRotation);
}

void AEndlessCharacter::UpdatePlayerHurtState()
{
	bool Hurt = HealthComponent->GetCurrentHealth() < HealthComponent->GetMaxHealth();
	Cast<AEndlessPlayerState>(GetPlayerState())->WorldMover->SetPlayerHurt(Hurt);
}


void AEndlessCharacter::Died()
{
	UE_LOG(LogTemp, Warning, TEXT("Died in code"));
	SetActorHiddenInGame(true);
	this->SetActorEnableCollision(false);
	Cast<AEndlessPlayerState>(GetPlayerState())->Died();
}
