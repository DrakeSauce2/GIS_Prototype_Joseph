// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"

#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "GCharacterBase.generated.h"

class UInputAction;
class UAnimMontage;
class UGAbilitySystemComponent;
class UGAttributeSet;
class UGameplayEffect;
/*
* 
*/
UCLASS()
class AGCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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
	virtual void LightAttack();
	UFUNCTION()
	virtual void MediumAttack();
	UFUNCTION()
	virtual void HeavyAttack();
	UFUNCTION()
	virtual void SpecialAttack();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UGAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UGAttributeSet* AttributeSet;

private:
	UFUNCTION()
	void HandleDirectionalInput(const FInputActionValue& InputValue);

	bool IsAnyMontagePlaying() const;
	void PlayAnimMontage(UAnimMontage* MontageToPlay);

	/*
	*	Change this later, I have no idea how to use the enable/disable functions on
	*	the APlayerController in cpp. So for now disabling input will be done through
	*	a bool.
	*/
	bool bInputEnabled = false; 

	FVector StartingPosition;

	UPROPERTY()
	UValueGauge* HealthBar;

	/*****************************************************/
	/*               Animation Montages                  */
	/*****************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* LightAttackAnimationMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* MediumAttackAnimationMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HeavyAttackAnimationMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* SpecialAttackAnimationMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* BlockAnimationMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* GrabAnimationMontage;

public:	
	void InitAttributes();

	void SetHealthBar(UValueGauge* HealthBarToSet);

	void HealthUpdated(const FOnAttributeChangeData& ChangeData);

	void FlipCharacter(bool bIsFacingRight);

	void TakeDamage(float Damage);

	void SetInputEnabled(bool state);

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
