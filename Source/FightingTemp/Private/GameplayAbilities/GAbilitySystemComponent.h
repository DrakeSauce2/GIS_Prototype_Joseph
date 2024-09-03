// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/GAbilitySystemTypes.h"
#include "GAbilitySystemComponent.generated.h"

struct FGameplayAbilitySpec;
class UGA_AbilityBase;
/**
 * 
 */
UCLASS()
class UGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> EffectToApply, int Level = 1);

	void ApplyInitialEffects();
	void GrantInitialAbilities();
	void ApplyFullStat();

	void TryActivateDirectionalAttack(FVector Direction, int InputID);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Init")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Init")
	TSubclassOf<UGameplayEffect> FullStatEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Init")
	TMap<EAbilityInputID, TSubclassOf<class UGA_AbilityBase>> Abilities;
};
