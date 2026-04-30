// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "TalentComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHITEBOX_API UTalentComp : public UActorComponent
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this component's properties
	UTalentComp();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <class UPDA_Talent*> GrantedTalents;
	class UAbilitySystemComponent* ASC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UTalentTree> WBP_TalentTree;
	TMap<class UPDA_Talent*, TArray<FGameplayAbilitySpecHandle>> GrantedTalents_GA;
	TMap<class UPDA_Talent*, TArray<FActiveGameplayEffectHandle>> GrantedTalents_GE;
	UPROPERTY()
	UTalentTree* TalentTreeWidget;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void GrantTalent(class UPDA_Talent* Talent);
	UFUNCTION(BlueprintCallable)
	void GetTalent(class UPDA_Talent* Talent);
	UFUNCTION(BlueprintCallable)
	void LevelUpTalent(class UPDA_Talent* Talent);
	UFUNCTION(BlueprintCallable)
	void ShowTalentUI();
	UFUNCTION(BlueprintCallable)
	void CloseTalentUI();
		
};
