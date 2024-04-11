#include "WinArea.h"

#include "CoopAdventureCharacter.h"

AWinArea::AWinArea()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WinAreaBox"));
	SetRootComponent(WinAreaBox);

	HasWinCondition = false;
}

void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;
	if (HasWinCondition) return;

	TArray<AActor*> OverlappingActors;
	WinAreaBox->GetOverlappingActors(OverlappingActors, ACoopAdventureCharacter::StaticClass());

	HasWinCondition = OverlappingActors.Num() >= 2;
	
	if (HasWinCondition)
	{
		UE_LOG(LogTemp, Warning, TEXT("WinArea has been activated"));
	}
}
