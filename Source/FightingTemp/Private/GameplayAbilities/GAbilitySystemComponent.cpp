// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GAbilitySystemComponent.h"
#include "GameplayAbilities/GA_AbilityBase.h"

void UGAbilitySystemComponent::ApplyGameplayEffect(TSubclassOf<UGameplayEffect> EffectToApply, int Level)
{
	FGameplayEffectSpecHandle specHandle = MakeOutgoingSpec(EffectToApply, Level, MakeEffectContext());
	ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
}

void UGAbilitySystemComponent::ApplyInitialEffects()
{
	for (const TSubclassOf<UGameplayEffect>& Effect : InitialEffects)
	{
		ApplyGameplayEffect(Effect, 1);
	}
}

// Maybe Rename to GrantInitialAttackAbilities
void UGAbilitySystemComponent::GrantInitialAbilities()
{
	for (int i = 0; i < LightAttackAbilities.Num(); i++) 
	{
		FGameplayAbilitySpecHandle SpecHandle =
			GiveAbility(FGameplayAbilitySpec{ LightAttackAbilities[i], 1, (int)EAbilityInputID::LightAttack, GetOwner()});

		if (i < AttackDirections.Num()) 
		{
			AssignAbilityAttackDirections(i, EAbilityInputID::LightAttack, SpecHandle);
		}
	}

	for (int i = 0; i < MediumAttackAbilities.Num(); i++)
	{
		FGameplayAbilitySpecHandle SpecHandle =
			GiveAbility(FGameplayAbilitySpec{ MediumAttackAbilities[i], 1, (int)EAbilityInputID::MediumAttack, GetOwner() });

		if (i < AttackDirections.Num())
		{
			AssignAbilityAttackDirections(i, EAbilityInputID::MediumAttack, SpecHandle);
		}
	}

	for (int i = 0; i < MediumAttackAbilities.Num(); i++)
	{
		FGameplayAbilitySpecHandle SpecHandle =
			GiveAbility(FGameplayAbilitySpec{ HeavyAttackAbilities[i], 1, (int)EAbilityInputID::HeavyAttack, GetOwner() });

		if (i < AttackDirections.Num())
		{
			AssignAbilityAttackDirections(i, EAbilityInputID::HeavyAttack, SpecHandle);
		}
	}

	for (int i = 0; i < MediumAttackAbilities.Num(); i++)
	{
		FGameplayAbilitySpecHandle SpecHandle =
			GiveAbility(FGameplayAbilitySpec{ SpecialAttackAbilities[i], 1, (int)EAbilityInputID::SpecialAttack, GetOwner() });

		if (i < AttackDirections.Num())
		{
			AssignAbilityAttackDirections(i, EAbilityInputID::SpecialAttack, SpecHandle);
		}
	}
}

void UGAbilitySystemComponent::ApplyFullStat()
{
	if (FullStatEffect) 
	{
		ApplyGameplayEffect(FullStatEffect);
	}
}

void UGAbilitySystemComponent::TryActivateDirectionalAttack(const FVector& Direction, const EAbilityInputID& InputType, bool IsGrounded)
{
	if (!IsGrounded)
	{
		// Do Air Attack
		UE_LOG(LogTemp, Warning, TEXT("I'm Not Grounded"));

		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("I'm Working"));

	FDirectionAttackKey Key;
	Key.Direction = Direction; // Maybe do some rounding to make sure this is accurate :)
	//Key.Direction = FVector(1, 0, 0); // For testing
	Key.InputType = InputType;

	UE_LOG(LogTemp, Warning, TEXT("FVector: %s, Enum Value: %d"), *Direction.ToString(), static_cast<int32>(InputType));

	FGameplayAbilitySpecHandle* SpecHandle = DirectionToAbilityHandleMap.Find(Key);
	if (SpecHandle) {
		UE_LOG(LogTemp, Warning, TEXT("I have ability spec"));

		TryActivateAbility(*SpecHandle, false);
	}
}

void UGAbilitySystemComponent::AssignAbilityAttackDirections(const int& Index, EAbilityInputID InputID, FGameplayAbilitySpecHandle SpecHandle)
{
	FDirectionAttackKey Key;
	Key.Direction = AttackDirections[Index];
	Key.InputType = InputID;

	DirectionToAbilityHandleMap.Add(Key, SpecHandle);
}
