// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/WeaponSystemComp.h"
#include "Enum/EWeapon.h"
#include "GameFramework/Character.h"
#include "Combat/CombatComponent.h"
#include "Animation/PlayerAnimInstance.h"
#include "Combat/Weapon/WeaponGeneral.h"

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
	charRef = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	AnimInstance = Cast<UPlayerAnimInstance>(charRef->GetMesh()->GetAnimInstance());
	Abilitycomp = charRef->GetComponentByClass<UAbilitySystemComponent>();
	FArmDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {
		if (bInterrupted)
		{
			AnimInstance->bIsWeaponChanging = false;
			if (IsValid(CurrentWeapon))
			{
				ArmmedWeaponToSocket(CurrentWeapon);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FArmDelegate: CurrentWeapon invalid on interrupt"));
			}
			UE_LOG(LogTemp, Warning, TEXT("Equiped bInterrupted!"));
		}
		UE_LOG(LogTemp, Warning, TEXT("Equiped complete!"));
		AnimInstance->bIsWeaponChanging = false;
		});
	FUnArmDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {
		if (bInterrupted)
		{
			AnimInstance->bIsWeaponChanging = false;
			if (IsValid(CurrentWeapon))
			{
				UnArmmedWeaponToSocket(CurrentWeapon);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FUnArmDelegate: CurrentWeapon invalid on interrupt"));
			}
			UE_LOG(LogTemp, Warning, TEXT("UnArmed bInterrupted!"));
		}
		UE_LOG(LogTemp, Warning, TEXT("UnArmed Complete!"));
		CurrentWeapon = nullptr;
		AnimInstance->bIsWeaponChanging = false;
		});
}


// Called every frame
void UWeaponSystemComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UWeaponSystemComp::ChangeWeapon(TSubclassOf<AWeaponGeneral>  WeaponClass)
//void UWeaponSystemComp::ChangeWeapon(EWeapons WeaponName)
{
	bool bWeaponFound = false;
	AWeaponGeneral* TargetWeapon{};
	for (AWeaponGeneral* ExistingWeapon : WeaponList) {
		if (ExistingWeapon && ExistingWeapon->IsA(WeaponClass))
		{
			bWeaponFound = true;
			TargetWeapon = ExistingWeapon;
			break;
		}
	}
		if (bWeaponFound)
		{
			RemoveWeaponAbilities(CurrentWeapon);
			GiveWeaponAbilities(TargetWeapon);
			if (!CurrentWeapon)
			{
				CurrentWeapon = TargetWeapon;
				ECurrentWeapons = TargetWeapon->WeaponName;
				EquipCurrentWeapon(TargetWeapon);
				AnimInstance->Montage_SetEndDelegate(FArmDelegate, CurrentWeapon->EquipMontage);
			}
			else if (TargetWeapon != CurrentWeapon) {
				if (CurrentWeapon->UArmMontage)
				{

					UE_LOG(LogTemp, Warning, TEXT("UArmMontage exist!"));
					ECurrentWeapons = EWeapons::NoWeapon;
					charRef->PlayAnimMontage(CurrentWeapon->UArmMontage);
					CurrentWeapon->ResertWeapon();
					TargetWeapon->ActiveWeapon();
					FChangeWeaponDelegate.BindLambda([this, TargetWeapon](UAnimMontage* Montage, bool bInterrupted) {
						if (bInterrupted)
						{
							if (CurrentWeapon)
								UnArmmedWeaponToSocket(CurrentWeapon);
							ArmmedWeaponToSocket(TargetWeapon);

						}
						else
						{
							charRef->PlayAnimMontage(TargetWeapon->EquipMontage);
							ArmmedWeaponToSocket(TargetWeapon);
							AnimInstance->bIsBattlePose = true;
							AnimInstance->Montage_SetEndDelegate(FArmDelegate, TargetWeapon->EquipMontage);
							UE_LOG(LogTemp, Warning, TEXT("changed weapon!!!"));
						}

						CurrentWeapon = TargetWeapon;
						ECurrentWeapons = TargetWeapon->WeaponName;
						}
					);
					AnimInstance->bIsBattlePose = false;
					AnimInstance->bIsWeaponChanging = true;
					AnimInstance->Montage_SetEndDelegate(FChangeWeaponDelegate, CurrentWeapon->UArmMontage);
				}

				else {

					charRef->PlayAnimMontage(TargetWeapon->EquipMontage);
					CurrentWeapon->ResertWeapon();
					CurrentWeapon = TargetWeapon;
					ECurrentWeapons = TargetWeapon->WeaponName;

				}
				UE_LOG(LogTemp, Warning, TEXT("changed!"));
			}
		}
}




void UWeaponSystemComp::GetWeapon(TSubclassOf<AWeaponGeneral>  TargetWeapon)
{
	bool bWeaponFound = false;
	for (AWeaponGeneral* ExistingWeapon : WeaponList)
	{
		if (ExistingWeapon && ExistingWeapon->IsA(TargetWeapon))
		{
			bWeaponFound = true;
			break;
		}
	}

	if (!bWeaponFound)
	{
		FTransform ActorTransform = GetOwner()->GetActorTransform();
		AWeaponGeneral* NewWeapon = GetWorld()->SpawnActor<AWeaponGeneral>(TargetWeapon, ActorTransform);

		auto Root = GetOwner<ACharacter>()->GetMesh();
		// 必须用引用 &Weapon，否则只修改了副本，NewWeapon->Weapons 里的 WeaponComp 仍然是空
		for (FWeaponStruct& Weapon : NewWeapon->Weapons)
		{
			int i = 0;
			UE_LOG(LogTemp, Warning, TEXT("Createed mesh!!!!!!!!!!!"));
			Weapon.WeaponComp = NewObject<USkeletalMeshComponent>(NewWeapon);
			if (!Weapon.WeaponMesh)
			{
				UE_LOG(LogTemp, Warning, TEXT("No Mesh!!!!!!!!!!!"));
				return;
			}
			Weapon.WeaponComp->SetSkeletalMesh(Weapon.WeaponMesh);
			Weapon.WeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Weapon.WeaponComp->RegisterComponent();
			Weapon.WeaponComp->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
			SetWeaponToSocket(Weapon.WeaponComp, Weapon.BackSocketName);

			Weapon.WeaponComp->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
			if (Weapon.WeaponABP)
				Weapon.WeaponComp->SetAnimInstanceClass(Weapon.WeaponABP);
			i++;

		}

		if (NewWeapon)
		{
			WeaponList.Add(NewWeapon);
			UE_LOG(LogTemp, Warning, TEXT("Weapon spawned successfully!"));


			for (const FWeaponStruct& FWeapon : NewWeapon->Weapons) {

				SetWeaponToSocket(FWeapon.WeaponComp, FWeapon.BackSocketName);
			}
			//SetWeaponToSocket(NewWeapon, NewWeapon->BackTargetSocket);

		}

	}
	
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HAS!!"));
	}
}

void UWeaponSystemComp::SetWeaponToSocket(USkeletalMeshComponent* Mesh, FName SocketName)
{
	if (!IsValid(charRef))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWeaponToSocket: charRef is invalid"));
		return;
	}

	if (!IsValid(Mesh))
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

	Mesh->AttachToComponent(
		MeshRef,
		AttachRules,
		SocketName
	);
}

void UWeaponSystemComp::ArmmedWeaponToSocket(AWeaponGeneral* Weanpon)
{
	if (!IsValid(Weanpon))
	{
		UE_LOG(LogTemp, Warning, TEXT("ArmmedWeaponToSocket: Weapon is invalid"));
		return;
	}
	Weanpon->ActiveWeapon();
	for (const FWeaponStruct& FWeapon : Weanpon->Weapons) {

		SetWeaponToSocket(FWeapon.WeaponComp, FWeapon.HandSocketName);
	}
	//SetWeaponToSocket(Weanpon, Weanpon->HandTargetSocket);
}

void UWeaponSystemComp::UnArmmedWeaponToSocket(AWeaponGeneral* Weanpon)
{
	if (!IsValid(Weanpon))
	{
		UE_LOG(LogTemp, Warning, TEXT("UnArmmedWeaponToSocket: Weapon is invalid"));
		return;
	}
	for (const FWeaponStruct& FWeapon : Weanpon->Weapons) {

		SetWeaponToSocket(FWeapon.WeaponComp, FWeapon.BackSocketName);
	}

	//SetWeaponToSocket(Weanpon, Weanpon->BackTargetSocket);
}

void UWeaponSystemComp::EquipCurrentWeapon(AWeaponGeneral* TargetWeapon)
{
	CurrentWeapon = TargetWeapon;
	ECurrentWeapons = TargetWeapon->WeaponName;
	AnimInstance->bIsBattlePose = true;
	UE_LOG(LogTemp, Warning, TEXT("equip true!!"));
	AnimInstance->bIsWeaponChanging = true;
	if (TargetWeapon->EquipMontage)
	{
		charRef->PlayAnimMontage(TargetWeapon->EquipMontage);
		AnimInstance->Montage_SetEndDelegate(FArmDelegate, TargetWeapon->EquipMontage);
	}
	else {


		UE_LOG(LogTemp, Warning, TEXT("NO equip Montage!!"));
	}
}

void UWeaponSystemComp::UnArmCurrentWeapon()
{
	if (!IsValid(CurrentWeapon))
	{
		UE_LOG(LogTemp, Warning, TEXT("UnArmCurrentWeapon: CurrentWeapon is invalid"));
		return;
	}

	CurrentWeapon->UArmWeapon();
	AnimInstance->Montage_SetEndDelegate(FUnArmDelegate, CurrentWeapon->UArmMontage);
	ECurrentWeapons = EWeapons::NoWeapon;
	AnimInstance->bIsBattlePose = false;
	AnimInstance->bIsWeaponChanging = true;
	AnimInstance->bIsIncombat = false;
}

void UWeaponSystemComp::GetThrowWeapons(TSubclassOf<AWeaponGeneral>  TargetWeapon)
{
	
	const FTransform ActorTransform = GetOwner()->GetActorTransform();
	CurrentThrowWeapon = GetWorld()->SpawnActor<AWeaponGeneral>(TargetWeapon, ActorTransform);
	ECurrentThrowWeapons = CurrentThrowWeapon->WeaponName;
	auto Root = GetOwner<ACharacter>()->GetMesh();
	// 必须用引用 &Weapon，否则只修改了副本，CurrentThrowWeapon->Weapons 里的 WeaponComp 仍然是空
	for (FWeaponStruct& Weapon : CurrentThrowWeapon->Weapons)
	{
		int i = 0;
		Weapon.WeaponComp = NewObject<USkeletalMeshComponent>(CurrentThrowWeapon);
		if(!Weapon.WeaponMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Mesh!!!!!!!!!!!"));
			return;
		}
		Weapon.WeaponComp->SetSkeletalMesh(Weapon.WeaponMesh);
		Weapon.WeaponComp->RegisterComponent();
		Weapon.WeaponComp->AttachToComponent(Root,FAttachmentTransformRules::KeepRelativeTransform);
		Weapon.WeaponComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CurrentThrowWeapon->WeaponMeshs.Add(Weapon.WeaponComp);
		SetWeaponToSocket(Weapon.WeaponComp, Weapon.HandSocketName);
		i++;

	}

	//SetWeaponToSocket(CurrentThrowWeapon, CurrentThrowWeapon->HandTargetSocket);
}




void UWeaponSystemComp::ThrowWeapon()
{
}

void UWeaponSystemComp::GiveWeaponAbilities(AWeaponGeneral* TargetWeapon)
{
	if (!Abilitycomp) return;


	for (FWeaponStruct Weapon : TargetWeapon->Weapons)
	{
		for (TSubclassOf<UGameplayAbility> Ability : Weapon.WeaponAbilities) {

			FGameplayAbilitySpecHandle SpecHandle = Abilitycomp->GiveAbility(FGameplayAbilitySpec(
				Ability,
				1,
				-1,
				charRef
			));
			WeaponAbility.Add(SpecHandle);
		}
		
	}
	
}

void UWeaponSystemComp::RemoveWeaponAbilities(AWeaponGeneral* TargetWeapon)
{
	if (!Abilitycomp) return;
	 
	for (FGameplayAbilitySpecHandle Ability : WeaponAbility) {

		Abilitycomp->ClearAbility(Ability);
	}
	WeaponAbility.Empty();

}



