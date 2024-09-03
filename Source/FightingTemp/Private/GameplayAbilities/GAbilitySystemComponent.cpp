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
	for (const TPair<EAbilityInputID, TSubclassOf<UGA_AbilityBase>>& AbilityPair : Abilities)
	{
		// This Returns a Spec Handle. Look into this
		GiveAbility(FGameplayAbilitySpec{ AbilityPair.Value, 1, (int)AbilityPair.Key, GetOwner() });
	}

	// Foreach, (MediumAttackPair : MediumAttackAbilities)
	// Foreach, (HeavyAttackPair : HeavyAttackAbilities)
	// Foreach, (SpecialAttackPair : SpecialAttackAbilities)
	// 
	// Do this to Add Abilities to a directional based TMap where FVector is key and Ability is value
}

void UGAbilitySystemComponent::ApplyFullStat()
{
	if (FullStatEffect) 
	{
		ApplyGameplayEffect(FullStatEffect);
	}
}

void UGAbilitySystemComponent::TryActivateDirectionalAttack(FVector Direction, int InputID)
{
	// FVector Pair with ability spec
	// ActivatableAbilities.Items
}
