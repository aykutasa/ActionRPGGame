// Fill out your copyright notice in the Description page of Project Settings.

#include "ARWeaponBase.h"
#include "AFAbilityComponent.h"
#include "AFAbilityInterface.h"
#include "AMAbilityManagerComponent.h"
#include "ARWeaponManagerComponent.h"
#include "../ARCharacter.h"
// Sets default values
AARWeaponBase::AARWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AARWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AARWeaponBase::OnAddToWeaponManager(EAMGroup Group, EAMSlot Slot
	, class APawn* InPOwner
	, class UAMAbilityManagerComponent* WeaponManager)
{
	UARWeaponManagerComponent* WManager = Cast<UARWeaponManagerComponent>(WeaponManager);
	WeaponManagerComponent = WManager;

	if (!WManager)
		return;
	
	POwner = InPOwner;
	FSimpleDelegate Delegate = FSimpleDelegate::CreateUObject(this, &AARWeaponBase::OnWeaponAbilityReady, Group);
	WManager->AddOnAbilityReadyEvent(WeaponAbility, Delegate);
	WManager->NativeEquipAbility(WeaponAbility, Group, Slot, this, false);
}
void AARWeaponBase::Equip()
{
	if (!POwner)
		return;

	AARCharacter* Character = Cast<AARCharacter>(POwner);
	if (!Character)
		return;

	AttachToActor(Character, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	//AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}
void AARWeaponBase::UnEquip()
{
	AARCharacter* Character = Cast<AARCharacter>(POwner);
	if (!Character)
		return;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AARWeaponBase::Holster(const FName& Socket)
{
	if (!POwner)
		return;

	AARCharacter* Character = Cast<AARCharacter>(POwner);
	if (!Character)
		return;
	//AttachToActor(Character, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
}
void AARWeaponBase::Multicast_Equip_Implementation()
{
	Equip();
}
void AARWeaponBase::Multicast_Holster_Implementation(const FName& Socket)
{
	Holster(Socket);
}
void AARWeaponBase::OnWeaponAbilityReady(EAMGroup Group)
{
	if (!POwner)
		return;

	AARCharacter* Character = Cast<AARCharacter>(POwner);
	if (!Character)
		return;
	if (!WeaponManagerComponent)
		return;

	WeaponManagerComponent->OnWeaponAbilityReady(WeaponAbility, this, Group);
}

void AARWeaponBase::OnRep_AttachmentReplication()
{
	Super::OnRep_AttachmentReplication();
	if (!POwner)
		return;

	AARCharacter* Character = Cast<AARCharacter>(POwner);
	if (!Character)
		return;
	//AttachToActor(Character, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetAttachmentReplication().AttachSocket);
}