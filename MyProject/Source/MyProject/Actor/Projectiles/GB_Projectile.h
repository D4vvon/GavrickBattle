// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GB_Projectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileHit, const FHitResult&, Hit, const FVector&, Direction);

UCLASS()
class MYPROJECT_API AGB_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AGB_Projectile();

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(FVector Direction);

	UPROPERTY(BlueprintAssignable)
	FOnProjectileHit OnProjectileHit;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComp;

	virtual void BeginPlay() override;

	virtual void OnProjectileLaunched();
private:
	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
