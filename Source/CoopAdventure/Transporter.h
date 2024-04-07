// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PressurePlate.h"
#include "Transporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPADVENTURE_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTransporter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector StartPoint;
	FVector EndPoint;

	bool ArePointsSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transporter")
	float MoveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transporter")
	TArray<APressurePlate*> TriggerActors;

	UPROPERTY(VisibleAnywhere, Category = "Transporter")
	int ActivatedTriggerCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transporter")
	bool AllTriggerActorsActivated;

	UFUNCTION()
	void SetPoints(FVector Start, FVector End);

	UFUNCTION()
	void OnPressurePlateActivated();

	UFUNCTION()
	void OnPressurePlateDeactivated();
};
