#include "pch.h"
#include "Logger.h"

LoggerRegistry& LoggerRegistry::getInstance() {
    static LoggerRegistry instance;
    return instance;
}

LoggerRegistry::LoggerRegistry() = default;
LoggerRegistry::~LoggerRegistry() = default;

std::shared_ptr<Logger> LoggerRegistry::getLogger(const std::string& name) {
    std::lock_guard<std::mutex> lock(registryMutex);
    auto it = loggers.find(name);
    if (it != loggers.end()) {
        return it->second;
    }
    return defaultLogger;
}

void LoggerRegistry::setDefaultLogger(std::shared_ptr<Logger> logger) {
    defaultLogger = logger;
}

void LoggerRegistry::registerLogger(const std::string& name,
                                    std::shared_ptr<Logger> logger) {
    std::lock_guard<std::mutex> lock(registryMutex);
    loggers[name] = logger;
}
