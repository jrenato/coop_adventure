// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include <Online/OnlineSessionNames.h>


void PrintString(const FString& Str)
{
    if (!GEngine) return;
    GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, Str);
}


UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() {
    //PrintString("MSS Constructor");

    MySessionName = FName("Co-Op Adventure Session Name");

    CreateServerAfterDestroy = false;
    DestroyServerName = "";
    ServerNameToFind = "";
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
    SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
    SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
    SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
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

    FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(MySessionName);
    if (ExistingSession)
    {
        FString Msg = FString::Printf(TEXT("Session already exists: %s, destroying it"),
            *MySessionName.ToString());
        PrintString(Msg);
        CreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
        SessionInterface->DestroySession(MySessionName);
        return;
    }
    
    FOnlineSessionSettings SessionSettings;

    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bIsDedicated = false;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.NumPublicConnections = 2;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bAllowJoinViaPresence = true;

    SessionSettings.bIsLANMatch = false;

    SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
    {
        SessionSettings.bIsLANMatch = true;
    }

    SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}


void UMultiplayerSessionsSubsystem::FindServer(FString ServerName)
{
    if (ServerName.IsEmpty())
	{
		PrintString(FString("Server name cannot be empty"));
		return;
	}

    ServerNameToFind = ServerName;
    bool bIsLAN = IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL";

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = bIsLAN;
    SessionSearch->MaxSearchResults = 9999;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}


void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %s, %d"), *SessionName.ToString(), bWasSuccessful));

    if (bWasSuccessful)
    {
        GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
    }
}


void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    FString Msg = FString::Printf(TEXT("OnDestroySessionComplete: %s, %d"), *SessionName.ToString(), bWasSuccessful);
    PrintString(Msg);

    if (CreateServerAfterDestroy)
	{
		CreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}


void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (!bWasSuccessful) return;
    if (ServerNameToFind.IsEmpty()) return;

    TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
    FOnlineSessionSearchResult* CorrectSearchResult = 0;

    if (SearchResults.Num() > 0)
	{
		PrintString(FString::Printf(TEXT("%d sessions found"), SearchResults.Num()));

        for (FOnlineSessionSearchResult SearchResult : SearchResults)
        {
            if (!SearchResult.IsValid()) continue;

            FString ServerName = "UNNAMED";
            SearchResult.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

            if (ServerName.Equals(ServerNameToFind))
			{
                CorrectSearchResult = &SearchResult;
			    PrintString(FString::Printf(TEXT("Found Server with name is: %s"), *ServerName));
                break;
			}
        }
	}
	else
	{
		PrintString(FString("No sessions found"));
	}

    if (CorrectSearchResult)
	{
        SessionInterface->JoinSession(0, MySessionName, *CorrectSearchResult);
	}
	else
	{
		PrintString(FString("Could not find server with name: " + ServerNameToFind));
		ServerNameToFind = "";
	}
}


void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        PrintString(FString("Failed to join session"));
		return;
    }

    PrintString(FString::Printf(TEXT("Successfully joined session: %s"), *SessionName.ToString()));

    FString AddressString;
    bool Success = SessionInterface->GetResolvedConnectString(MySessionName, AddressString);

    if (!Success)
	{
		PrintString(FString("Failed to get Address String"));
		return;
	}

	PrintString(FString::Printf(TEXT("Address string: %s"), *AddressString));
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

    if (!PlayerController)
	{
		PrintString(FString("Failed to get player controller"));
		return;
	}

	PlayerController->ClientTravel(AddressString, ETravelType::TRAVEL_Absolute);
}


void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
    PrintString(FString("OnStartSessionComplete"));
}
