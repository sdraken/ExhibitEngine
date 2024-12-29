/* Potential future optimization:
    -Use a logging queue and a background thread to process logs asynchronously.
    - avoiding dynamic allocations made by std::format/vformat/ofstream
*/
#include <iostream>
#include <fstream>
#include <mutex>
#include <format>


//Define macros to be used by other components
#define LOGDEBUG(format, ...) Logger::getInstance().log(SeverityLevel::DEBUG, format, ##__VA_ARGS__)
#define LOGFATAL(format, ...) Logger::getInstance().log(SeverityLevel::FATAL, format, ##__VA_ARGS__)

enum class SeverityLevel {
    DEBUG,
    FATAL
};

class Logger {
public:
    // Deleted copy constructor and assignment operator to enforce singleton
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Static method to get the single instance of Logger
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    // Logging method
    template <typename... Args>
    void log(SeverityLevel level, const std::string& formatString, Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex);
        std::string message = std::vformat(formatString, std::make_format_args(std::forward<Args>(args)...));
        std::string logEntry = formatLogEntry(level, message);

        // Log to console
        std::cout << logEntry << std::endl;

        // Log to file
        if (fileStream.is_open()) {
            fileStream << logEntry << std::endl;
        }
    }

private:
    Logger(){
        initialize("logfile");
    }
    ~Logger() {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }
        // Method to initialize the logger with a file
    void initialize(const std::string& logFilePath) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!fileStream.is_open()) {
            fileStream.open(logFilePath, std::ios::out | std::ios::app);
            if (!fileStream) {
                throw std::runtime_error("Failed to open log file: " + logFilePath);
            }
        }
    }


    std::string formatLogEntry(SeverityLevel level, const std::string& message) {
        std::string levelStr = (level == SeverityLevel::DEBUG) ? "DEBUG" : "FATAL";
        return std::format("[{}]: {}", levelStr, message);
    }

    std::ofstream fileStream;
    std::mutex mutex;
};
