// Fill out your copyright notice in the Description page of Project Settings.


#include "KnowledgeNode.h"


#define ENABLE_LOGGING 1
#include "utillllllssss.h"


#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AKnowledgeNode::AKnowledgeNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	if (0)
	{
		MySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
		MySphere->SetHiddenInGame(false, true);
		RootComponent = MySphere;

		SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
		// SphereMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SphereMesh->SetupAttachment(RootComponent);


		static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(
			TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);


		float Scale = 0.2;
		SphereMesh->SetWorldScale3D(FVector(Scale, Scale, Scale));
	}


	if (0)
	{
		TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
		RootComponent = TextComponent;

		// Configure the default text settings
		TextComponent->SetText(FText::FromString(TEXT("Hello, World!")));
		TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		TextComponent->SetWorldSize(100.f);
	}


	// This section is required because if we do not set a root component
	// If we attach objects to the root component of this actor
	// Then the objects will not be visible in the editor
	if (1)
	{
		MySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
		MySphere->SetHiddenInGame(true, true);
		RootComponent = MySphere;
		
	}
}

// Called when the game starts or when spawned
void AKnowledgeNode::BeginPlay()
{
	Super::BeginPlay();
}





// Called every frame
void AKnowledgeNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
