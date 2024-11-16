//	Description: Implementation of Logger class.
//
//	Author: Svante Drakenberg


#include "Logger.h"

//initalizer opens logfile
void ExhibitEngine::Logger::initalize(){
	outputLocation = fopen(LOGFILENAME, "a");
}

//Close logfile
void ExhibitEngine::Logger::shutDown(){
	fclose(outputLocation);
}

//Given a loglevel, returns its string representation 
const char* ExhibitEngine::Logger::getLogLevelString(LogLevel loglevel){
	switch (loglevel)
	{
	case LogLevel::DEBUG:
		return "[DBG]";
	case LogLevel::WARNING:
		return "[WRN]";
	case LogLevel::FATAL:
		return "[FTL]";
	default:
		return "[---]";
	}
}

//Update the timeBuffer buffer containing the current time
void ExhibitEngine::Logger::updateTimeBuffer(){
	auto now = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	std::tm localTime = *std::localtime(&currentTime);

	sprintf(timeBuffer, "%02d/%02d/%02d %02d:%02d:%02d.%03d",
		localTime.tm_year - 100,   // Year since 1900
		localTime.tm_mon + 1,       // Month [0-11] + 1
		localTime.tm_mday,          // Day of the month [1-31]
		localTime.tm_hour,          // Hours [0-23]
		localTime.tm_min,           // Minutes [0-59]
		localTime.tm_sec,           // Seconds [0-59]
		static_cast<int>(milliseconds.count())); // Milliseconds
}
