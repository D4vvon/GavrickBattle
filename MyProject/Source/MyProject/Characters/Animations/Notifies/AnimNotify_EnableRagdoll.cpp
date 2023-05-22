// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EnableRagdoll.h"
#include "../../../GavrickBattleTypes.h"

void UAnimNotify_EnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionProfileName(CollisionProfileRagdoll);
}
