# Coop Adventure

A Coop Adventure game made in Unreal 5.3.2 using C++

## Notes

### Declaring a Multicast RPC

Declare a *UFUNCTION* using *NetMulticast* and *Reliable*:

```
UFUNCTION(NetMulticast, Reliable)
void MulticastRPCFunction()
```

The body's name must be followed by *_Implementation*:

```
void AMyClass::MulticastRPCFunction_Implementation()
{

}
```

### Declaring a multicast delegate

Use the macro *DECLARE_DYNAMIC_MULTICAST_DELEGATE* to declare it:

```
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMulticastReceived);
```

If it contains one or more parameters, use one of the variations:

```
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStateTimeOfDayChanged, float, Hour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGameStateTimeOfDayChanged, float, Hour, int32, Minute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGameStateTimeOfDayChanged, float, Hour, int32, Minute, int32, Sec);
... and so on.
```

Declare a method using the delegate type:

```
UPROPERTY(BlueprintAssignable)
FGameStateTimeOfDayChanged OnTimeUpdated;
```

Finally, you can broadcast it using:

```
OnTimeUpdated.Broadcast(CurrentHouer, CurrentMinute, CurrentSeconds);
```
