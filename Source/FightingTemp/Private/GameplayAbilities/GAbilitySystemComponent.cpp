// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/GAbilitySystemComponent.h"

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

void UGAbilitySystemComponent::ApplyFullStat()
{
	if (FullStatEffect) 
	{
		ApplyGameplayEffect(FullStatEffect);
	}
}
