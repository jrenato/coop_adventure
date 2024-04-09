#include "MovableActor.h"

AMovableActor::AMovableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	StartPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("StartPoint"));
	StartPoint->SetupAttachment(RootComp);
	StartPoint->SetRelativeLocation(FVector::Zero());
	
	EndPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("EndPoint"));
	EndPoint->SetupAttachment(RootComp);
	EndPoint->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();
	
	FVector StartLocation = GetActorLocation() + StartPoint->GetRelativeLocation();
	FVector EndLocation = GetActorLocation() + EndPoint->GetRelativeLocation();

	Transporter->SetPoints(StartLocation, EndLocation);
}

void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

