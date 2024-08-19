// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GCharacterBase.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "GameplayAbilities/GAbilitySystemComponent.h"
#include "GameplayAbilities/GAttributeSet.h"

#include "Components/CapsuleComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Framework/HitboxActor.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Widgets/ValueGauge.h"

AGCharacterBase::AGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGAttributeSet>(TEXT("AttributeSet"));

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGAttributeSet::GetHealthAttribute()).AddUObject(this, &AGCharacterBase::HealthUpdated);
}

UAbilitySystemComponent* AGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AGCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	StartingPosition = GetActorLocation();
}

// Called every frame
void AGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	*	Force constrains the players position, the options in the editor didn't work
	*	so I just hard coded it in. Probably try to sort that out later.
	*/ 
	if (GetActorLocation().Y != StartingPosition.Y)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Y = StartingPosition.Y;

		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void AGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComp)
	{
		enhancedInputComp->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::HandleDirectionalInput);

		enhancedInputComp->BindAction(LightAttackInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::LightAttack);
		enhancedInputComp->BindAction(MediumAttackInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::MediumAttack);
		enhancedInputComp->BindAction(HeavyAttackInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::HeavyAttack);
		enhancedInputComp->BindAction(SpecialAttackInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::SpecialAttack);

		enhancedInputComp->BindAction(BlockInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::Block);
		enhancedInputComp->BindAction(GrabInputAction, ETriggerEvent::Triggered, this, &AGCharacterBase::Grab);
	}
}

void AGCharacterBase::HandleDirectionalInput(const FInputActionValue& InputValue)
{
	if (IsAnyMontagePlaying()) return;

	FVector2D input = InputValue.Get<FVector2D>();
	input.Normalize();

	if (!bInputEnabled) 
	{
		input = FVector2D::ZeroVector;
	}

	if (input.Y > 0)
	{
		Jump();
	}
	// TODO: implement crouching

	FVector Direction = FVector(input.X, 0, 0);

	AddMovementInput(Direction);
}

void AGCharacterBase::InitAttributes()
{
	AbilitySystemComponent->ApplyInitialEffects();
}

void AGCharacterBase::SetHealthBar(UValueGauge* HealthBarToSet)
{
	if (HealthBarToSet) 
	{
		HealthBar = HealthBarToSet;
	}
}

void AGCharacterBase::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (HealthBar)
	{
		HealthBar->SetValue(ChangeData.NewValue, AttributeSet->GetMaxHealth());
	}

	if (ChangeData.NewValue <= 0)
	{
		// Die
	}
}

void AGCharacterBase::FlipCharacter(bool bIsFacingRight)
{
	float direction = bIsFacingRight ? 1.0f : -1.0f;

	if (GetMesh()->GetComponentScale().Y == direction) return;

	FVector NewScale = GetMesh()->GetComponentScale();
	NewScale.Y = direction;

	GetMesh()->SetWorldScale3D(NewScale);
}

void AGCharacterBase::TakeDamage(float Damage)
{
	// Do health stuff

	// Apply hit stagger
}

void AGCharacterBase::SetInputEnabled(bool state)
{
	bInputEnabled = state;
}

#pragma region Action Functions

void AGCharacterBase::Block()
{

}

void AGCharacterBase::Grab()
{
	if (!bInputEnabled) return;

}

void AGCharacterBase::LightAttack()
{
	if (!bInputEnabled) return;

	PlayAnimMontage(LightAttackAnimationMontage);
}

void AGCharacterBase::MediumAttack()
{
	if (!bInputEnabled) return;

	PlayAnimMontage(MediumAttackAnimationMontage);
}

void AGCharacterBase::HeavyAttack()
{
	if (!bInputEnabled) return;

	PlayAnimMontage(HeavyAttackAnimationMontage);
}

void AGCharacterBase::SpecialAttack()
{
	if (!bInputEnabled) return;

	PlayAnimMontage(SpecialAttackAnimationMontage);
}

#pragma endregion

#pragma region Animation Montage Helper Functions

bool AGCharacterBase::IsAnyMontagePlaying() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return true;
	}

	return AnimInstance->IsAnyMontagePlaying();
}

void AGCharacterBase::PlayAnimMontage(UAnimMontage* MontageToPlay)
{
	if (!MontageToPlay) return;

	if (IsAnyMontagePlaying() == true) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	AnimInstance->Montage_Play(MontageToPlay);
}

#pragma endregion
