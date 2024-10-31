// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActortest77.generated.h"

UCLASS()
class UE544_BLANKKKKKKKKKK_API AMyActortest77 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActortest77();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	FVector Location1;
	FVector Location2;

	UStaticMeshComponent* TempComponent1;
	UStaticMeshComponent* TempComponent2;

};
