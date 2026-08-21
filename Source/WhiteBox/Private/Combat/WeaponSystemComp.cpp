// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/WeaponSystemComp.h"
#include "Enum/EWeapon.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Combat/Weapon/WeaponGeneral.h"
#include "Animation/PlayerAnimInstance.h"
#include "PDA/PDA_Weapon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWeaponSystemComp::UWeaponSystemComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

	
// Called when the game starts
void UWeaponSystemComp::BeginPlay()
{
	Super::BeginPlay();
	charRef = Cast<ACharacter>(GetOwner());
	AnimInstance = Cast<UPlayerAnimInstance>(charRef->GetMesh()->GetAnimInstance());
	Abilitycomp = charRef->GetComponentByClass<UAbilitySystemComponent>();


	FEquipedDelegate.BindLambda([this](UAnimMontage* Montage,bool bInterrupted) {
		
		if(bInterrupted)
		{
			AnimInstance->bIsWeaponChanging = false;
			EquipWeaponToSocketByEWeapon(ECurrentWeapons);

		}

		
		UE_LOG(LogTemp, Warning, TEXT("NEW FEquipedDelegate Equiped complete!"));
		EquipWeaponToSocketByEWeapon(ECurrentWeapons);

	});

	FUnArmDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {

		if (bInterrupted)
		{
			AnimInstance->bIsWeaponChanging = false;
			UnArmWeaponToSocketByEWeapon(EUnarmedWeapon);
		}
		UE_LOG(LogTemp, Warning, TEXT("UArmmedDelegate <%s> UnArm complete!"), *UEnum::GetValueAsString(EUnarmedWeapon));
		UnArmWeaponToSocketByEWeapon(EUnarmedWeapon);
		if (bIsChangingWeapon)
		{
			AnimInstance->bIsWeaponChanging = false;
			ECurrentWeapons = EChangeWeapon;
			EquipWeaponByEWeapon(EChangeWeapon);
			bIsChangingWeapon = false;
		}


		});
}


// Called every frame
void UWeaponSystemComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UWeaponSystemComp::ArmmedWeaponToSocket(EWeapons WeaponName)
{
	TArray<AWeaponGeneral*>& AllWeapon = AllGaintedWeaponStructs[WeaponName];
	for (AWeaponGeneral*& Weapon : AllWeapon)
	{
		SetActorToSocket(Weapon, Weapon->WeaponStructData.HandSocketName);

		UE_LOG(LogTemp, Warning, TEXT("HandSocketName: %s"),*Weapon->WeaponStructData.HandSocketName.ToString());
	}
}


void UWeaponSystemComp::UnArmmedWeaponToSocket(EWeapons WeaponName)
{

	if (WeaponName == EWeapons::NoWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnArmmedWeaponToSocket: No weapon to unarm"));
		return;

	}

	TArray<AWeaponGeneral*>& AllWeapon = AllGaintedWeaponStructs[WeaponName];
	for (AWeaponGeneral*& Weapon : AllWeapon)
	{
		SetActorToSocket(Weapon, Weapon->WeaponStructData.BackSocketName);
	}

}





void UWeaponSystemComp::ThrowWeapon()
{
}



void UWeaponSystemComp::GetWeaponHandle(UPDA_Weapon* WeaponData)
{
	EWeapons WeaponName = WeaponData->WeaponName;

	if(AllGaintedWeaponStructs.Contains(WeaponName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Already have this weapon!"));
		return;
	}

	auto Root = GetOwner<ACharacter>()->GetMesh();
	//TArray<FWeaponStruct> FWeaponStructs;
	TArray<AWeaponGeneral*> WeaponActors;
	for (FWeaponStruct& Weapon : WeaponData->WeaponStructs)
	{
		UClass* WeaponClass = AWeaponGeneral::StaticClass();
		if (IsValid(Weapon.WeaponGeneral))
		{
			WeaponClass = Weapon.WeaponGeneral->GetClass();
		}

		const FTransform SpawnTransform = GetOwner()->GetActorTransform();
		AWeaponGeneral* AWeapon = GetWorld()->SpawnActorDeferred<AWeaponGeneral>(
			WeaponClass,
			SpawnTransform,
			GetOwner(),
			Cast<APawn>(GetOwner()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (!AWeapon)
		{
			continue;
		}

		AWeapon->InitWeapon(Weapon);
		UGameplayStatics::FinishSpawningActor(AWeapon, SpawnTransform);
		WeaponActors.Add(AWeapon);
	}
	AllGaintedWeaponStructs.FindOrAdd(WeaponName) = WeaponActors;
	WeaponEquipedAnims.FindOrAdd(WeaponName) = WeaponData->EquipAnim;
	WeaponUnarmAnims.FindOrAdd(WeaponName) = WeaponData->UnarmAnim;

}

void UWeaponSystemComp::SwitchWeapon(EWeapons WeaponName)
{
	if (!AllGaintedWeaponStructs.Contains(WeaponName))
	{
		UE_LOG(LogTemp, Error, TEXT("Don't have this weapon!"));
		return;
	}

	if (ECurrentWeapons == WeaponName) {
		UE_LOG(LogTemp, Warning, TEXT("Already equiped this weapon!"));
		EChangeWeapon = WeaponName;
		UArmWeaponByEWeapon(WeaponName);
		EUnarmedWeapon = ECurrentWeapons;
		ECurrentWeapons = EWeapons::NoWeapon;
		return;
	}

	if(ECurrentWeapons == EWeapons::NoWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("No weapon equiped!"));
		EquipWeaponByEWeapon(WeaponName);
		return;
	}
	
	if(ECurrentWeapons != WeaponName)
	{
		EUnarmedWeapon = ECurrentWeapons;
		ECurrentWeapons = EWeapons::NoWeapon;
		EChangeWeapon = WeaponName;
		UArmWeaponByEWeapon(EUnarmedWeapon);
		bIsChangingWeapon = true;
		return;
	}
	








}

void UWeaponSystemComp::EquipWeaponByEWeapon(EWeapons WeaponName)
{
	TArray<AWeaponGeneral*>& AllWeapon = AllGaintedWeaponStructs[WeaponName];
	ECurrentWeapons = WeaponName;
	AnimInstance->bIsBattlePose = true;
	UE_LOG(LogTemp, Warning, TEXT("equip true!!"));
	AnimInstance->bIsWeaponChanging = true;


	UAnimMontage* EAnim = WeaponEquipedAnims[WeaponName];
	if (EAnim)
	{
		charRef->PlayAnimMontage(EAnim);
		AnimInstance->Montage_SetEndDelegate(FEquipedDelegate, EAnim);
	}

	else {

		EquipWeaponToSocketByEWeapon(WeaponName);

	}

	TSubclassOf<UAnimInstance> ABPLayer = AnimInstance->AnimInstanceLayers.FindRef(WeaponName);
	if (ABPLayer)
	{
		AnimInstance->GetSkelMeshComponent()->LinkAnimClassLayers(ABPLayer);
	}


	for (AWeaponGeneral*& Weapon : AllWeapon)
	{
		if (Weapon->WeaponStructData.WeaponAbilities.Num() > 0)
			GiveWeaponAbilitiesByWeaponStruct(Weapon->WeaponStructData);
	}















	//for(AWeaponGeneral*& Weapon : AllWeapon)
	//{
	//	if(Weapon->WeaponStructData.WeaponAbilities.Num()>0)
	//		GiveWeaponAbilitiesByWeaponStruct(Weapon->WeaponStructData);

	//	if(Weapon->WeaponStructData.CHA_EquipMontage)
	//	{
	//		charRef->PlayAnimMontage(Weapon->WeaponStructData.CHA_EquipMontage);
	//		UE_LOG(LogTemp, Warning, TEXT("Character Name: %s"),*charRef->GetName());
	//		AnimInstance->Montage_SetEndDelegate(FEquipedDelegate, Weapon->WeaponStructData.CHA_EquipMontage);
	//	}
	//	else
	//	{
	//		ECurrentWeapons = EChangeWeapon;
	//		//副手武器问题
	//		EquipWeaponByEWeapon(EChangeWeapon);
	//		UE_LOG(LogTemp, Warning, TEXT("NO equip CHA_Montage!!"));
	//	}

	//	if(Weapon->WeaponStructData.WPN_EquipMontage)
	//	{
	//		UAnimInstance* WeaponAnim = Weapon->WeaponMeshComp->GetAnimInstance();
	//		WeaponAnim->Montage_Play(Weapon->WeaponStructData.WPN_EquipMontage);
	//		WeaponAnim->Montage_SetEndDelegate(FEquipedDelegate, Weapon->WeaponStructData.WPN_EquipMontage);
	//		
	//	}
	//	else
	//	{
	//		ECurrentWeapons = EChangeWeapon;
	//		UE_LOG(LogTemp, Warning, TEXT("NO equip WEAPON_Montage!!"));
	//	}		

	//}


}

void UWeaponSystemComp::UArmWeaponByEWeapon(EWeapons WeaponName)
{
	TArray<AWeaponGeneral*>& AllWeapon = AllGaintedWeaponStructs[WeaponName];
	AnimInstance->bIsBattlePose = false;
	UE_LOG(LogTemp, Warning, TEXT("FWeaponStructs num: %i"), AllWeapon.Num());
	UE_LOG(LogTemp, Warning, TEXT("UnArmed complete! CurrentWeapon :%s"), *StaticEnum<EWeapons>()->GetNameStringByValue((int64)WeaponName));
	AnimInstance->bIsWeaponChanging = true;



	UAnimMontage* UAnim = WeaponUnarmAnims[WeaponName];

	if (UAnim)
	{
		charRef->PlayAnimMontage(UAnim);
		for (AWeaponGeneral*& Weapon : AllWeapon) {

			UE_LOG(LogTemp, Warning, TEXT("UnArmed complete! ActorNaME :%s"), *Weapon->GetName())
				RemoveWeaponAbilitiesByWeaponStruct(Weapon->WeaponStructData);
		}
		AnimInstance->Montage_SetEndDelegate(FUnArmDelegate, UAnim);
	}

	else {

		UnArmWeaponToSocketByEWeapon(WeaponName);
		for (AWeaponGeneral*& Weapon : AllWeapon) {

			UE_LOG(LogTemp, Warning, TEXT("UnArmed complete! ActorNaME :%s"), *Weapon->GetName())
				RemoveWeaponAbilitiesByWeaponStruct(Weapon->WeaponStructData);
		}
		EquipWeaponByEWeapon(EChangeWeapon);

	}

	




	/*for (AWeaponGeneral*& Weapon : AllWeapon) {

		UE_LOG(LogTemp, Warning, TEXT("UnArmed complete! ActorNaME :%s"), *Weapon->GetName())
		RemoveWeaponAbilitiesByWeaponStruct(Weapon->WeaponStructData);
		if (Weapon->WeaponStructData.CHA_UArmMontage)
		{
			UE_LOG(LogTemp, Warning, TEXT("HAS MONTAGE!!!"));
			charRef->PlayAnimMontage(Weapon->WeaponStructData.CHA_UArmMontage);
			AnimInstance->Montage_SetEndDelegate(FUnArmDelegate, Weapon->WeaponStructData.CHA_UArmMontage);
		}
		else
		{
			ECurrentWeapons = EChangeWeapon;
			EquipWeaponByEWeapon(EChangeWeapon);
			UE_LOG(LogTemp, Warning, TEXT("NO Uarm Montage!!"));
		}
		if (Weapon->WeaponStructData.WPN_UArmMontage)
		{	
			UAnimInstance* WeaponAnim = Weapon->WeaponMeshComp->GetAnimInstance();
			WeaponAnim->Montage_Play(Weapon->WeaponStructData.WPN_UArmMontage);
			WeaponAnim->Montage_SetEndDelegate(FUnArmDelegate, Weapon->WeaponStructData.WPN_UArmMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO Weapon Uarm Montage!!"));
		}
	}*/


}

void UWeaponSystemComp::SetActorToSocket(AActor* Actor, FName SocketName)
{

	if (!IsValid(charRef))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWeaponToSocket: charRef is invalid"));
		return;
	}

	if (!IsValid(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWeaponToSocket: Weapon is invalid"));
		return;
	}

	USkeletalMeshComponent* MeshRef = charRef->GetMesh();
	if (!MeshRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWeaponToSocket: MeshRef is null"));
		return;
	}


	FAttachmentTransformRules AttachRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);

	Actor->AttachToComponent(
		MeshRef,
		AttachRules,
		SocketName
	);


}

void UWeaponSystemComp::EquipWeaponToSocketByEWeapon(EWeapons WeaponName)
{
	if (WeaponName == EWeapons::NoWeapon)
		return;
	TArray<AWeaponGeneral*>& AllWeapon = AllGaintedWeaponStructs[WeaponName];
	for(AWeaponGeneral*& Weapon : AllWeapon)
	{
		SetActorToSocket(Weapon, Weapon->WeaponStructData.HandSocketName);
	}

}

void UWeaponSystemComp::UnArmWeaponToSocketByEWeapon(EWeapons WeaponName)
{
	TArray<AWeaponGeneral*>& AllWeapon = AllGaintedWeaponStructs[WeaponName];
	for (AWeaponGeneral*& Weapon : AllWeapon)
	{
		SetActorToSocket(Weapon, Weapon->WeaponStructData.BackSocketName);
	}

}


void  UWeaponSystemComp::GiveWeaponAbilitiesByWeaponStruct(FWeaponStruct Weapon)
{
	if (!Abilitycomp) return;

	for (TSubclassOf<UGameplayAbility> Ability : Weapon.WeaponAbilities)
	{
		if (!*Ability)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponSystemComp: Skipping null WeaponAbility."));
			continue;
		}
		FGameplayAbilitySpecHandle SpecHandle = Abilitycomp->GiveAbility(FGameplayAbilitySpec(
			Ability,
			1,
			-1,
			charRef
		));
		WeaponAbility.Add(SpecHandle);
	}
}

void UWeaponSystemComp::RemoveWeaponAbilitiesByWeaponStruct(FWeaponStruct Weapon)
{
	if (!Abilitycomp) return;
	for (FGameplayAbilitySpecHandle Ability : WeaponAbility) {
		Abilitycomp->ClearAbility(Ability);
	}
	WeaponAbility.Empty();
}

TArray<AWeaponGeneral*> UWeaponSystemComp::GetWeaponByEWeapon(EWeapons WeaponName)
{
	UE_LOG(LogTemp, Warning, TEXT("AllGaintedWeaponStructsnNum: %i"), AllGaintedWeaponStructs.Num());

	UE_LOG(LogTemp, Warning, TEXT("WeaponName: %s"), *AllGaintedWeaponStructs[WeaponName][0]->GetName());
	return AllGaintedWeaponStructs[WeaponName];
}
