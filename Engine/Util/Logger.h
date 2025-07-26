#ifndef EN_LOGGER_H
#define EN_LOGGER_H

#include <mutex>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdarg>

enum class ELogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class ILogger {
public:
    virtual void Write(const std::string &Message) = 0;
    virtual ~ILogger() {}
};

class CConsoleLogger : public ILogger {
public:
    void Write(const std::string &Message) override {
        std::cout << Message << std::endl;
    }
};

class CFileLogger : public ILogger {
protected:
    std::ofstream MFile;
public:
    CFileLogger(const std::string &Filename) : MFile(Filename) {}
    void Write(const std::string &Message) override {
        if (MFile.is_open()) {
            MFile << Message << std::endl;
        }
    }
};

class CLogger {
protected:
    std::mutex MMutex;
    ELogLevel MCurrentLevel;
    std::vector<ILogger *> MOutputs;
protected:
    CLogger() : MCurrentLevel(ELogLevel::Info) {}

    std::string FormatMessage(ELogLevel Level, const std::string &Message, const std::string &File, int Line)
    {
        std::string LevelStr = LevelToString(Level);
        std::string Timestamp = GetTimestamp();
        return "[" + Timestamp + "] [" + LevelStr + "] [" + File + ":" + std::to_string(Line) + "] " + Message;
    }

    std::string LevelToString(ELogLevel Level)
    {
        switch (Level) {
            case ELogLevel::Debug: return "DEBUG";
            case ELogLevel::Info: return "INFO";
            case ELogLevel::Warning: return "WARNING";
            case ELogLevel::Error: return "ERROR";
            case ELogLevel::Critical: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }

    std::string GetTimestamp()
    {
        auto Now = std::chrono::system_clock::now();
        auto NowTimeT = std::chrono::system_clock::to_time_t(Now);
        std::tm *NowTm = std::localtime(&NowTimeT);
        std::ostringstream Oss;
        Oss << std::put_time(NowTm, "%Y-%m-%d %H:%M:%S");
        return Oss.str();
    }
public:
    static CLogger &GetInstance()
    {
        static CLogger Instance;
        return Instance;
    }

    void SetLevel(ELogLevel Level)
    {
        std::lock_guard<std::mutex> Lock(MMutex);
        MCurrentLevel = Level;
    }

    void AddOutput(ILogger *Output)
    {
        std::lock_guard<std::mutex> Lock(MMutex);
        MOutputs.push_back(Output);
    }

    void Log(ELogLevel Level, const std::string &Message, const std::string &File, int Line)
    {
        std::lock_guard<std::mutex> Lock(MMutex);
        if (Level >= MCurrentLevel) {
            std::string Formatted = FormatMessage(Level, Message, File, Line);
            for (auto Output : MOutputs) {
                Output->Write(Formatted);
            }
        }
    }

    void LogFormat(ELogLevel Level, const std::string &File, int Line, const char *Format, ...)
    {
        std::lock_guard<std::mutex> Lock(MMutex);
        if (Level >= MCurrentLevel) {
            va_list Args;
            va_start(Args, Format);
            std::string Message = FormatString(Format, Args);
            va_end(Args);
            std::string Formatted = FormatMessage(Level, Message, File, Line);
            for (auto Output : MOutputs) {
                Output->Write(Formatted);
            }
        }
    }

    std::string FormatString(const char *Format, va_list Args)
    {
        va_list ArgsCopy;
        va_copy(ArgsCopy, Args);
        int Length = vsnprintf(nullptr, 0, Format, ArgsCopy);
        va_end(ArgsCopy);

        std::vector<char> Buffer(Length + 1);
        vsnprintf(Buffer.data(), Length + 1, Format, Args);
        return std::string(Buffer.data(), Length);
    }
};

#define LOG_DEBUG(Message) CLogger::GetInstance().Log(ELogLevel::Debug, Message, __FILE__, __LINE__)
#define LOG_INFO(Message) CLogger::GetInstance().Log(ELogLevel::Info, Message, __FILE__, __LINE__)
#define LOG_WARNING(Message) CLogger::GetInstance().Log(ELogLevel::Warning, Message, __FILE__, __LINE__)
#define LOG_ERROR(Message) CLogger::GetInstance().Log(ELogLevel::Error, Message, __FILE__, __LINE__)
#define LOG_CRITICAL(Message) CLogger::GetInstance().Log(ELogLevel::Critical, Message, __FILE__, __LINE__)

#define LOG_DEBUG_FORMAT(Format, ...) CLogger::GetInstance().LogFormat(ELogLevel::Debug, __FILE__, __LINE__, Format, ##__VA_ARGS__)
#define LOG_INFO_FORMAT(Format, ...) CLogger::GetInstance().LogFormat(ELogLevel::Info, __FILE__, __LINE__, Format, ##__VA_ARGS__)
#define LOG_WARNING_FORMAT(Format, ...) CLogger::GetInstance().LogFormat(ELogLevel::Warning, __FILE__, __LINE__, Format, ##__VA_ARGS__)
#define LOG_ERROR_FORMAT(Format, ...) CLogger::GetInstance().LogFormat(ELogLevel::Error, __FILE__, __LINE__, Format, ##__VA_ARGS__)
#define LOG_CRITICAL_FORMAT(Format, ...) CLogger::GetInstance().LogFormat(ELogLevel::Critical, __FILE__, __LINE__, Format, ##__VA_ARGS__)

#endif