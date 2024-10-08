#include "utillllllssss.h"
#include <fstream>

// #include <iostream>

#include "Engine/Engine.h"
// #include "Editor.h"
// void ll(const FString& StringToLog)
// {
// 	if (!StringToLog.IsEmpty())
// 	{
// 		// Log to Unreal Engine console
// 		UE_LOG(LogTemp, Log, TEXT("%s"), *StringToLog);
//         
// 		// Optionally, display on screen
// 		if (GEngine)
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, StringToLog);
// 		}
// 	}
// }




void ClearLogFile() {

	std::string FilePath = R"(V:\UNREALLLLPROJECTSSSSSS\ue544_blankkkkkkkkkk\Saved\Logs\MyLogFile.txt)";

	std::ofstream LogFile(FilePath, std::ios::trunc); // Open in truncate mode to clear contents
	if (LogFile.is_open()) {
		LogFile.close();  // Just close the file, it's already empty
	} else {
		// std::cerr << "Failed to open log file for clearing." << std::endl;
	}
}





void lll(const FString& TextToWrite)
{
	bool nologgggggggg = false;

	if (nologgggggggg)
	{
		return;
	}
	else
	{
	}
	if (1)
	{
		// Define the path to the log file.
		std::string FilePath = R"(V:\UNREALLLLPROJECTSSSSSS\ue544_blankkkkkkkkkk\Saved\Logs\MyLogFile.txt)";
		// Change path accordingly

		
		std::ofstream LogFile(FilePath,
		                      std::ios::app
		                      // std::ios::out
		);
		if (LogFile.is_open())
		{
			// Write the text to the file and end with a new line.
			LogFile << TCHAR_TO_ANSI(*TextToWrite) << std::endl;

			// Close the file.
			LogFile.close();
		}
		else
		{
			// Log an error message if file opening failed.
			UE_LOG(LogTemp, Warning, TEXT("Failed to open log file."));
		}
	}
}


void ll(const FString& StringToLog, bool LOG, int SeverityLevel, const FString& Prefix)
{
	bool nologgggggggg = false;
	if (nologgggggggg)
	{
		return;
	}
	else
	{
	}

	if (!LOG)
	{
		return;
	}
	
	
	if (!StringToLog.IsEmpty())
	{
		FString LogMessage = Prefix + StringToLog; // Prepends a prefix to the original message
		// ELogVerbosity::Type LogLevel1;


		if (1)
		{
			// Map SeverityLevel to ELogVerbosity
			switch (SeverityLevel)
			{
			case 0:
				UE_LOG(LogTemp, Log, TEXT("%s"), *LogMessage);
				break;
			case 1:
				UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage);
				break;
			case 2:
				UE_LOG(LogTemp, Error, TEXT("%s"), *LogMessage);
				break;
			default:
				UE_LOG(LogTemp, Log, TEXT("%s"), *LogMessage);
				break;
			}
		}
		else
		{
		}

		if (0)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, LogMessage);
			}
		}
		else
		{
		}
	}


	if (1)
	{
		lll(StringToLog);
	}
	else
	{
	}
}

FVector Jiggle(const FVector& Vec, float Magnitude)
{
	if (0)
	{
		FVector RandomJitter;
		RandomJitter.X = FMath::RandRange(-0.5f, 0.5f) * Magnitude;
		RandomJitter.Y = FMath::RandRange(-0.5f, 0.5f) * Magnitude;
		RandomJitter.Z = FMath::RandRange(-0.5f, 0.5f) * Magnitude;

		return Vec + RandomJitter;
	}
	else
	{
		
		return Vec;
	}

}


void eeeee()
{
	// if (GEditor)
	// {
	// 	GEditor->EndPlayMap();
	// }
}
