// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/StaticMeshComponent.h"

#include "PressurePlate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressurePlateActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressurePlateDeactivated);

// Forward declarations to prevent circular references.
class UTransporter;

UCLASS()
class COOPADVENTURE_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	APressurePlate();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* TriggerMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsActivated;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTransporter* Transporter;

	UPROPERTY(BlueprintAssignable)
	FOnPressurePlateActivated OnActivated;

	UPROPERTY(BlueprintAssignable)
	FOnPressurePlateDeactivated OnDeactivated;
};
