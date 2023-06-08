// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiomComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


void UExplosiomComponent::Explode(AController* Controller)
{
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		MaxDamage,
		MinDamage,
		GetComponentLocation(),
		InnerRadius,
		OuterRadius,
		DamageFalloff,
		DamageTypeClass,
		IgnoreActors,
		GetOwner(),
		Controller,
		ECC_Visibility //add new channel for damage throw some objects 
	);
	if (IsValid(ExplosionFX))
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetComponentLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionFX, GetComponentLocation(), GetComponentRotation());
	}
	if (OnExplosion.IsBound())
	{	
		OnExplosion.Broadcast();
	}
}
