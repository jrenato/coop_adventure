#include "Transporter.h"

#include "PressurePlate.h"
#include "CollectableKey.h"


UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;

	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
	ArePointsSet = false;

	OwnerIsTriggerActor = false;
}


void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerIsTriggerActor)
	{
		TriggerActors.Add(Cast<APressurePlate>(GetOwner()));
	}

	for (AActor* TriggerActor : TriggerActors)
	{
		APressurePlate* PressurePlate = Cast<APressurePlate>(TriggerActor);
		if (PressurePlate)
		{
			PressurePlate->OnActivated.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
			PressurePlate->OnDeactivated.AddDynamic(this, &UTransporter::OnTriggerActorDeactivated);
			continue;
		}

		ACollectableKey* KeyActor = Cast<ACollectableKey>(TriggerActor);
		if (KeyActor)
		{
			KeyActor->OnCollected.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
		}
	}
}


void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TriggerActors.Num() == 0) return;

	AllTriggerActorsActivated = ActivatedTriggerCount >= TriggerActors.Num();

	AActor* TransporterOwner = GetOwner();

	if (TransporterOwner && TransporterOwner->HasAuthority() && ArePointsSet)
	{
		FVector CurrentLocation = TransporterOwner->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

		FVector TargetLocation = AllTriggerActorsActivated ? EndPoint : StartPoint;

		if (!CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			TransporterOwner->SetActorLocation(NewLocation);
		}
	}
}


void UTransporter::SetPoints(FVector Start, FVector End)
{
	if (Start.Equals(End)) return;

	StartPoint = Start;
	EndPoint = End;
	ArePointsSet = true;
}


void UTransporter::OnTriggerActorActivated()
{
	ActivatedTriggerCount++;
}


void UTransporter::OnTriggerActorDeactivated()
{
	ActivatedTriggerCount--;
}
