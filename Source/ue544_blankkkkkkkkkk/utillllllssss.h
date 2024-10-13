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


///////////////////////////////////////////////////////////////////////////////////////////
// #include <iostream>
// #include <chrono>
//
// // Function template to measure the execution time of any function
// template<typename Func, typename... Args>
// void measureExecutionTime(Func func, Args&&... args) {
// 	using namespace std::chrono;
//     
// 	// Start time
// 	auto start = high_resolution_clock::now();
//     
// 	// Call the function with given arguments
// 	func(std::forward<Args>(args)...);
//     
// 	// End time
// 	auto end = high_resolution_clock::now();
//     
// 	// Calculate and print the elapsed time
// 	auto duration = duration_cast<microseconds>(end - start).count();
// 	std::cout << "Execution time: " << duration << " microseconds" << std::endl;
// }


//
// #include <chrono>
// #define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
// #define timeNow() std::chrono::high_resolution_clock::now()
//
// typedef std::chrono::high_resolution_clock::time_point TimeVar;
//
//
// template<typename F, typename... Args>
// double funcTime(F func, Args&&... args){
// 	TimeVar t1=timeNow();
// 	func(std::forward<Args>(args)...);
// 	return duration(timeNow()-t1);
// }



    