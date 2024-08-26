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

void UGAbilitySystemComponent::GrantInitialAbilities()
{
	for (const TPair<EAbilityInputID, TSubclassOf<UGA_AbilityBase>>& AbilityPair : Abilities)
	{
		GiveAbility(FGameplayAbilitySpec{ AbilityPair.Value, 1, (int)AbilityPair.Key, GetOwner() });
	}
}

void UGAbilitySystemComponent::ApplyFullStat()
{
	if (FullStatEffect) 
	{
		ApplyGameplayEffect(FullStatEffect);
	}
}
