#include "RaceRecorderSubsystem.h"

#include "HAL/FileManager.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void URaceRecorderSubsystem::StartSession()
{
	const FString Dir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Results"));
	IFileManager::Get().MakeDirectory(*Dir, true);

	const FDateTime Now = FDateTime::Now();
	const FString SessionStamp = Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
	SessionCSVPath = FPaths::Combine(Dir, FString::Printf(TEXT("Session_%s.csv"), *SessionStamp));

	const FString Header = BuildCSVHeader();
	FFileHelper::SaveStringToFile(Header, *SessionCSVPath);
}

void URaceRecorderSubsystem::StartRun()
{
	CurrentRun = FRaceResultRow{};

	// Level name
	if (UWorld* World = GetWorld())
	{
		CurrentRun.LevelName = World->GetMapName();
	}

	// Timestamp
	const FDateTime Now = FDateTime::Now();
	CurrentRun.Timestamp = Now.ToString(TEXT("%Y-%m-%d_%H-%M-%S"));
}

void URaceRecorderSubsystem::SetUserInfo(const FString& InUserID, const FString& InUserGender, int32 InUserAge)
{
	CurrentRun.UserID = InUserID;
	CurrentRun.UserGender = InUserGender;
	CurrentRun.UserAge = InUserAge;
}

void URaceRecorderSubsystem::SetRunSettings(const FString& InSceneName, int32 InTargetFPS, int32 InResX, int32 InResY)
{
	CurrentRun.SceneName = InSceneName;
	CurrentRun.TargetFPS = InTargetFPS;
	CurrentRun.ResX = InResX;
	CurrentRun.ResY = InResY;
}

void URaceRecorderSubsystem::SetHits(int32 InPositiveHits, int32 InNegativeHits)
{
	CurrentRun.PositiveHits = InPositiveHits;
	CurrentRun.NegativeHits = InNegativeHits;
}

void URaceRecorderSubsystem::SetFinalScore(int32 InFinalScore)
{
	CurrentRun.FinalScore = InFinalScore;
}


void URaceRecorderSubsystem::FinishRun(float DurationSeconds)
{
	CurrentRun.DurationSeconds = DurationSeconds;

	if (SessionCSVPath.IsEmpty())
	{
		StartSession();
	}

	const FString Line = BuildCSVLine(CurrentRun);
	AppendLineToSessionCSV(Line);
}


FString URaceRecorderSubsystem::BuildCSVHeader() const
{
	return TEXT("Timestamp,UserID,UserGender,UserAge,LevelName,SceneName,TargetFPS,ResX,ResY,DurationSeconds,PositiveHits,NegativeHits,FinalScore\n");
}

FString URaceRecorderSubsystem::BuildCSVLine(const FRaceResultRow& Row) const
{
	const FString UserIDEscaped = FString::Printf(TEXT("\"%s\""), *Row.UserID);
	const FString GenderEscaped = FString::Printf(TEXT("\"%s\""), *Row.UserGender);
	const FString LevelEscaped = FString::Printf(TEXT("\"%s\""), *Row.LevelName);
	const FString SceneEscaped = FString::Printf(TEXT("\"%s\""), *Row.SceneName);


	return FString::Printf(
		TEXT("%s,%s,%s,%d,%s,%s,%d,%d,%d,%.3f,%d,%d,%d\n"),
		*Row.Timestamp,
		*UserIDEscaped,
		*GenderEscaped,
		Row.UserAge,
		*LevelEscaped,
		*SceneEscaped,
		Row.TargetFPS,
		Row.ResX,
		Row.ResY,
		Row.DurationSeconds,
		Row.PositiveHits,
		Row.NegativeHits,
		Row.FinalScore
	);
}

bool URaceRecorderSubsystem::AppendLineToSessionCSV(const FString& Line) const
{
	return FFileHelper::SaveStringToFile(
		Line,
		*SessionCSVPath,
		FFileHelper::EEncodingOptions::AutoDetect,
		&IFileManager::Get(),
		FILEWRITE_Append
	);
}
