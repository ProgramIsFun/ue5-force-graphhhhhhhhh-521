#pragma once

#include <string>

#include "CoreMinimal.h"

// #ifndef ENABLE_LOGGING
// #define ENABLE_LOGGING 0
// #endif
//
//
//
// #if ENABLE_LOGGING
// #define ll(StringToLog, ...) lll2(StringToLog, ##__VA_ARGS__)
// #else
// #define ll(StringToLog, ...) // Define as empty to disable logging
// #endif




// Declare the function
// void ll(const FString& StringToLog);
// void ll3(const FString& StringToLog, int SeverityLevel = 0, const FString& Prefix = TEXT("[Info]"));
// void lll(const std::string& LogMessage);


void ClearLogFile();
// void lll(const std::string& text);
void lll(const FString& TextToWrite);


void ll(const FString& StringToLog, bool LOG=false, int SeverityLevel = 0, const FString& Prefix = TEXT("[Info]"));


FVector Jiggle(const FVector& Vec, float Magnitude);

void eeeee();