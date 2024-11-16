#pragma once
#include <chrono>

#define LOGFILENAME "logs.txt"

namespace ExhibitEngine{

	enum class LogLevel {
		DEBUG,
		WARNING,
		FATAL,
	};

	class Logger {
	public:
		void initalize();
		void shutDown();

		template<typename... Ts>
		void log(LogLevel logLevel, const char* format, Ts... args);

	private:
		char outputBuffer[10000];
		char timeBuffer[30];
		FILE* outputLocation = NULL;

		const char* getLogLevelString(LogLevel loglevel);

		void updateTimeBuffer();

	};

	template<typename ...Ts>
	void Logger::log(LogLevel logLevel, const char* format, Ts ...args)
	{
		updateTimeBuffer();

		sprintf(outputBuffer, " %s %s - %s\n", getLogLevelString(logLevel), timeBuffer, format);
		
		printf(outputBuffer, args...);

		if (outputLocation != NULL) {
			fprintf(outputLocation, outputBuffer, args...);
		}
		else {
			sprintf(outputBuffer, " %s %s - %s\n", getLogLevelString(LogLevel::FATAL), timeBuffer, "Attempted logging without logfile");
			printf(outputBuffer);
			std::exit(EXIT_FAILURE);
		}

		if (logLevel >= LogLevel::FATAL) {
			std::exit(EXIT_FAILURE);
		}
	}


	extern Logger logger;


}

#define LOGDEBUG(format, ...) ExhibitEngine::logger.log(ExhibitEngine::LogLevel::DEBUG, format, ##__VA_ARGS__)
#define LOGWARNING(format, ...) ExhibitEngine::logger.log(ExhibitEngine::LogLevel::WARNING, format, ##__VA_ARGS__)
#define LOGFATAL(format, ...) ExhibitEngine::logger.log(ExhibitEngine::LogLevel::FATAL, format, ##__VA_ARGS__)