// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DismembermentComponent.h"
#include "GameFramework/Character.h"
#include "Structure/FDismembermentBody.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/CollisionProfile.h"

// Sets default values for this component's properties
UDismembermentComponent::UDismembermentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	
	// ...
}


// Called when the game starts
void UDismembermentComponent::BeginPlay()
{
	Super::BeginPlay();
	for (FDismembermentBody& body : DismembermentBodies) {
		CharRef = Cast<ACharacter>(GetOwner());
		USkeletalMeshComponent* SkeleMeshComp = NewObject<USkeletalMeshComponent>(
			CharRef->GetMesh(),
			USkeletalMeshComponent::StaticClass(),
			body.BoneName);
		SkeleMeshComp->RegisterComponent();
		SkeleMeshComp->SetSkeletalMesh(body.Mesh);
		SkeleMeshComp->SetLeaderPoseComponent(CharRef->GetComponentByClass<USkeletalMeshComponent>());
		SkeleMeshComp->AttachToComponent(CharRef->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		// 必须显式开启 Query 碰撞；只设置 Response 不会改变默认的 NoCollision
		SkeleMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SkeleMeshComp->SetCollisionObjectType(ECC_Pawn);
		SkeleMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SkeleMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECollisionResponse::ECR_Block);
		body.SkeletalMeshComponent = SkeleMeshComp;

	}
	// ...
	
}


// Called every frame
void UDismembermentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FDismembermentBody UDismembermentComponent::getBodyPart(FName BoneName)
{

	UE_LOG(LogTemp, Warning, TEXT("getBodyPart: DismembermentBodies[0] name is %s"), *DismembermentBodies[0].SkeletalMeshComponent->GetName());
	UE_LOG(LogTemp, Warning, TEXT("getBodyPart: BoneName %s"), *BoneName.ToString());
	for (FDismembermentBody& body : DismembermentBodies) {
		if (body.BoneName == BoneName) {
			return body;
		}
	
	
	}
	return FDismembermentBody{};


}

void UDismembermentComponent::BodyTakeDamage(FName BoneName, float damageAmount ,FVector ImpactNormal,FVector ImpacLocation)
{
	FDismembermentBody body = getBodyPart(BoneName);

	if (!body.SkeletalMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BodyTakeDamage: MeshComp is nullptr"));
		return;
	}
	body.BodyHP = UKismetMathLibrary::FClamp(body.BodyHP - damageAmount, 0.f, 100.f);
	UE_LOG(LogTemp, Warning, TEXT("BodyTakeDamage: HP %f"), body.BodyHP);
	


	//body.SkeletalMeshComponent->SetVisibility(false);
	//USkeletalMeshComponent* LimbComp = NewObject<USkeletalMeshComponent>();
	//body.SkeletalMeshComponent->SetVisibility(false);
	//LimbComp->SetSkeletalMesh(body.Mesh);
	//LimbComp->SetLeaderPoseComponent(CharRef->GetComponentByClass<USkeletalMeshComponent>());
	//LimbComp->SetLeaderPoseComponent(nullptr);
	//LimbComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//LimbComp->SetAnimInstanceClass(nullptr);
	//LimbComp->SetEnablePhysicsBlending(false);
	//LimbComp->SetAllBodiesSimulatePhysics(true);
	//LimbComp->SetSimulatePhysics(true);


	// 解除主骨骼同步，准备独立模拟
	body.SkeletalMeshComponent->SetLeaderPoseComponent(nullptr);
	body.SkeletalMeshComponent->SetCollisionProfileName("Ragdoll",true);
	body.SkeletalMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	body.SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	body.SkeletalMeshComponent->Stop();
	//body.SkeletalMeshComponent->SetAnimInstanceClass(nullptr);
	//body.SkeletalMeshComponent->SetAllBodiesSimulatePhysics(true);
	//body.SkeletalMeshComponent->SetEnablePhysicsBlending(false);

	// 模拟物理前必须使用 QueryAndPhysics：QueryOnly 与 SetSimulatePhysics 不兼容，会导致警告且无法与地面碰撞下落
	body.SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	body.SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	body.SkeletalMeshComponent->SetSimulatePhysics(true);
	body.SkeletalMeshComponent->AddForceAtLocation(ImpactNormal.GetSafeNormal() * 1000, ImpacLocation, NAME_None);
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(),
		ImpacLocation,
		ImpacLocation + ImpactNormal*1000,
		2.f,
		FLinearColor::Red,
		3,
		2
		);
}

