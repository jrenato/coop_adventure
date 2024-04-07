#include "PressurePlate.h"

APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = true;

	IsActivated = false;

	SetReplicates(true);
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(RootComp);
	TriggerMesh->SetIsReplicated(true);

	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));

	if (TriggerMeshAsset.Succeeded())
	{
		TriggerMesh->SetStaticMesh(TriggerMeshAsset.Object);
		TriggerMesh->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Stylized_Egypt/Meshes/building/SM_building_part_08"));

	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 0.5f));
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.2f));
	}
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerMesh->SetVisibility(false);
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));
}

void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	TArray<AActor*> OverlappingActors;
	TriggerMesh->GetOverlappingActors(OverlappingActors);

	AActor* TriggerActor = 0;

	for (int i = 0; i < OverlappingActors.Num(); i++)
	{
		if (OverlappingActors[i]->ActorHasTag("TriggerActor"))
		{
			TriggerActor = OverlappingActors[i];
			break;
		}
	}

	if (TriggerActor)
	{
		if (!IsActivated)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, TEXT("Activated"));
			IsActivated = true;
			OnActivated.Broadcast();
		}
	}
	else
	{
		if (IsActivated)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, TEXT("Deactivated"));
			IsActivated = false;
			OnDeactivated.Broadcast();
		}
	}
}

