// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


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

    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
    SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
    SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
    SessionInterface->OnStartSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnStartSessionComplete);
}


void UMultiplayerSessionsSubsystem::Deinitialize()
{
    //UE_LOG(LogTemp, Warning, TEXT("MSS Deinitialize"));
}


void UMultiplayerSessionsSubsystem::CreateServer(FString ServerName)
{
    if (ServerName.IsEmpty())
    {
        PrintString(FString("Server name cannot be empty"));
        return;
    }

    FName SessionName = FName("Co-Op Adventure Session Name");
    FOnlineSessionSettings SessionSettings;

    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = false;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bAllowJoinViaPresence = true;

    SessionSettings.bIsLANMatch = false;

    if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
    {
        SessionSettings.bIsLANMatch = true;
    }

    SessionInterface->CreateSession(0, SessionName, SessionSettings);
}


void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
    PrintString(FString("Finding Server '") + ServerName + "'");
}


void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %s, %d"), *SessionName.ToString(), bWasSuccessful));
}


void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    PrintString(FString("OnFindSessionsComplete"));
}


void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    PrintString(FString("OnJoinSessionComplete"));
}


void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    PrintString(FString("OnDestroySessionComplete"));
}


void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
    PrintString(FString("OnStartSessionComplete"));
}
