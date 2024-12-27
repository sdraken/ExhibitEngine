//	Description: The Logger class is used to log useful/crucial information to stdout and/or a logfile.
//				 To be used by ExhibitEngine's other components for assertions, debugging, etc. The logging 
//				 method needs to be implemented directly in the header file since it uses the template keyword 
//				 to define behavior  with an arbitrary number of format modifiers in the message (like, %d, %s, etc).
// 
// 
// log(LogLevel logLevel, const char* format, Ts... args) since it 
//
//
//	Author: Svante Drakenberg

#pragma once
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#define LOGFILENAME "logs.txt"

namespace ExhibitEngine{

	enum class LogLevel {
		DEBUG,
		WARNING,
		FATAL,
	};

	class Logger {
	public:
		Logger();
		~Logger();

		template<typename... Ts>
		void log(LogLevel logLevel, const char* format, Ts... args);

	private:
		char outputBuffer[1000];
		char timeBuffer[30];
		FILE* outputLocation = fopen(LOGFILENAME, "a");

		const char* getLogLevelString(LogLevel loglevel);

		void updateTimeBuffer();

	};

	// Combine the loglevel, currenttime and log message and output it to stdout and logfile. 
	template<typename ...Ts>
	void Logger::log(LogLevel logLevel, const char* format, Ts ...args)
	{
		updateTimeBuffer();

		sprintf(outputBuffer, " %s %s - %s\n", getLogLevelString(logLevel), timeBuffer, format);
		
		printf(outputBuffer, args...);

		//write log message to logfile if it has been created/opened
		if (outputLocation != NULL) {
			fprintf(outputLocation, outputBuffer, args...);
		}
		//You should not log without a logfile. terminate if no logfile is found.
		else {
			sprintf(outputBuffer, " %s %s - %s\n", getLogLevelString(LogLevel::FATAL), timeBuffer, "Attempted logging without logfile");
			printf(outputBuffer);
			std::exit(EXIT_FAILURE);
		}

		if (logLevel >= LogLevel::FATAL) {
			std::exit(EXIT_FAILURE);
		}
	}

	//Extern allows usage of Logger defined in Application.h by components that need to use the logger
	extern Logger logger;


}

//Define macros to be used by other components
#define LOGDEBUG(format, ...) ExhibitEngine::logger.log(ExhibitEngine::LogLevel::DEBUG, format, ##__VA_ARGS__)
#define LOGWARNING(format, ...) ExhibitEngine::logger.log(ExhibitEngine::LogLevel::WARNING, format, ##__VA_ARGS__)
#define LOGFATAL(format, ...) ExhibitEngine::logger.log(ExhibitEngine::LogLevel::FATAL, format, ##__VA_ARGS__)