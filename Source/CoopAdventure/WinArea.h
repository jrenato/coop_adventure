#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"

#include "WinArea.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWinAreaWinCondition);


UCLASS()
class COOPADVENTURE_API AWinArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AWinArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool HasWinCondition;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* WinAreaBox;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCWin();
	
	UPROPERTY(BlueprintAssignable)
	FOnWinAreaWinCondition OnWinCondition;
};
