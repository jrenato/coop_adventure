// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"


void PrintString(const FString& Str)
{
    if (!GEngine) return;
    GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, Str);
}


UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() {
    //PrintString("MSS Constructor");
}


void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    //PrintString("MSS Initialize");
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem) return;

    FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
    PrintString(SubsystemName);

    SessionInterface = OnlineSubsystem->GetSessionInterface();
    if (!SessionInterface.IsValid()) return;

    PrintString("Session Interface Valid");
}


void UMultiplayerSessionsSubsystem::Deinitialize()
{
    //UE_LOG(LogTemp, Warning, TEXT("MSS Deinitialize"));
}


void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
    PrintString(FString("Creating Server '") + ServerName + "'");
}


void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
    PrintString(FString("Finding Server '") + ServerName + "'");
}
