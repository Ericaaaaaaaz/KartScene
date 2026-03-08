#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RaceRecorderSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FRaceResultRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	FString Timestamp;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	FString LevelName;

	UPROPERTY(BlueprintReadWrite, Category = "User")
	FString UserID;

	UPROPERTY(BlueprintReadWrite, Category = "User")
	FString UserGender;

	UPROPERTY(BlueprintReadWrite, Category = "User")
	int32 UserAge = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	FString SceneName;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	int32 TargetFPS = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	int32 ResX = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	int32 ResY = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	float DurationSeconds = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	int32 PositiveHits = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	int32 NegativeHits = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Race")
	int32 FinalScore = 0;

};

UCLASS()
class KART_API URaceRecorderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Race|Record")
	void StartSession();

	// Call once when a run starts
	UFUNCTION(BlueprintCallable, Category = "Race|Record")
	void StartRun();

	UFUNCTION(BlueprintCallable, Category = "Race|Record")
	void SetUserInfo(const FString& InUserID, const FString& InUserGender, int32 InUserAge);

	// Set run settings chosen in menu
	UFUNCTION(BlueprintCallable, Category = "Race|Record")
	void SetRunSettings(const FString& InSceneName, int32 InTargetFPS, int32 InResX, int32 InResY);

	// Update values during play / at the end
	UFUNCTION(BlueprintCallable, Category = "Race|Record")
	void SetHits(int32 InPositiveHits, int32 InNegativeHits);

	UFUNCTION(BlueprintCallable, Category = "Race|Record")
	void SetFinalScore(int32 InFinalScore);

	UFUNCTION(BlueprintCallable, Category = "Race|Record")

	void FinishRun(float DurationSeconds);

	UFUNCTION(BlueprintCallable, Category = "Race|Export")
	FString GetSessionCSVPath() const { return SessionCSVPath; }

private:
	UPROPERTY()
	FRaceResultRow CurrentRun;

	UPROPERTY()
	FString SessionCSVPath;

	bool AppendLineToSessionCSV(const FString& Line) const;
	FString BuildCSVHeader() const;
	FString BuildCSVLine(const FRaceResultRow& Row) const;
};
