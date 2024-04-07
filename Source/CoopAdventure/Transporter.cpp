#include "Transporter.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;

	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
	ArePointsSet = false;
}


void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	for (APressurePlate* PressurePlate : TriggerActors)
	{
		PressurePlate->OnActivated.AddDynamic(this, &UTransporter::OnPressurePlateActivated);
		PressurePlate->OnDeactivated.AddDynamic(this, &UTransporter::OnPressurePlateDeactivated);
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


void UTransporter::OnPressurePlateActivated()
{
	ActivatedTriggerCount++;
}


void UTransporter::OnPressurePlateDeactivated()
{
	ActivatedTriggerCount--;
}
