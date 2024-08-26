// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"

#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilities/IGGameplayCueInterface.h"
#include "GCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath, int32, PlayerId);

class UInputAction;
class UValueGauge;
class UAnimMontage;
class UGAbilitySystemComponent;
class UGAttributeSet;
class UGameplayEffect;
/*
* 
*/
UCLASS()
class AGCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGGameplayCueInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDeath OnPlayerDeath;

	float GetHealth();

	void ApplyFullStat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*****************************************************/
	/*                 Action Functions                  */
	/*****************************************************/
	UFUNCTION()
	void Block();
	UFUNCTION()
	void Grab();
	
	UFUNCTION()
	void LightAttack();
	UFUNCTION()
	void MediumAttack();
	UFUNCTION()
	void HeavyAttack();
	UFUNCTION()
	void SpecialAttack();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UGAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UGAttributeSet* AttributeSet;

private:
	void StunTagChanged(const FGameplayTag TagChanged, int32 NewStackCount);
	void DeathTagChanged(const FGameplayTag TagChanged, int32 NewStackCount);

	int32 GetPlayerLocalID() const;

	UPROPERTY(EditDefaultsOnly, Category = "Hitbox")
	class UHitboxComponent* HitboxComponent;

	UFUNCTION()
	void HandleDirectionalInput(const FInputActionValue& InputValue);

	FVector StartingPosition;

	UPROPERTY()
	UValueGauge* HealthBar;

public:	
	UFUNCTION()
	void InitAttributes();
	UFUNCTION()
	void InitAbilities();

	void SetHealthBar(UValueGauge* HealthBarToSet);

	void HealthUpdated(const FOnAttributeChangeData& ChangeData);

	void FlipCharacter(bool bIsFacingRight);

	void SetInputEnabled(bool state);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/*************************************************************/
	/*                       Gameplay Cue                        */
	/*************************************************************/
	virtual void StartStunAnim() override;
	virtual void StopStunAnim() override;
	virtual void PlayHitReaction() override;

public:
	/*****************************************************/
	/*                       Input                       */
	/*****************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LightAttackInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MediumAttackInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* HeavyAttackInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SpecialAttackInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* BlockInputAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* GrabInputAction;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayCue")
	UAnimMontage* StunMontage;
	UPROPERTY(EditDefaultsOnly, Category = "GameplayCue")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "GameplayCue")
	TSubclassOf<class UGameplayEffect> DeathEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HitReactionMontage;

	UFUNCTION()
	void PlayMontage(UAnimMontage* MontageToPlay);


};
