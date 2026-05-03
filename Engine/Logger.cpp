#include "pch.h"
#include "Logger.h"

LoggerRegistry& LoggerRegistry::getInstance() {
    static LoggerRegistry instance;
    return instance;
}