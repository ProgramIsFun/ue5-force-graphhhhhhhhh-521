// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "KnowledgeNode.generated.h"

struct ConnectedNode
{
	AKnowledgeNode* node;
	float delay;
	FVector offset;
};


// class FORCEGRAPH_API AKnowledgeNode : public AActor

UCLASS()
class UE544_BLANKKKKKKKKKK_API AKnowledgeNode : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKnowledgeNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	void IncreaseTextSize();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	int numberOfConnected = 0;
	float strength1111 = -60; 
	FVector velocity;
	

	UPROPERTY(VisibleAnywhere)
	USphereComponent* MySphere;



	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextComponent;
	
	UStaticMeshComponent* SphereMesh;
};
