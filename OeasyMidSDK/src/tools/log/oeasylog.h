#ifndef OEASY_LOG_H
#define OEASY_LOG_H
#include "spdlog/spdlog.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <memory>
using namespace std;

#define MAX_LOG_RECORD_LEN    2048*2

//日志优先级
enum LogLevel {
	INFO_LOG,
	WARN_LOG,
	ERROR_LOG
};
//log显示类型
enum LogType {
	Console_LOG,
	Rotating_LOG,
	Daily_LOG
};

class OeasyLog
{
public:
	~OeasyLog();
	static OeasyLog& getInstance();
	static void setLogLevel(LogLevel level = INFO_LOG);
	std::shared_ptr<spdlog::logger> CreateLogInstance(string filepath, LogType  type = Daily_LOG);
	void Error(const char* filename, const int lineNo, const char* pchFormat, ...);
	void Info(const char* filename, const int lineNo, const char* pchFormat, ...);
	void Warn(const char* filename, const int lineNo, const char* pchFormat, ...);
private:
	OeasyLog();
private:
	std::shared_ptr<spdlog::logger> m_cur_logger;
	std::shared_ptr<spdlog::logger> m_consol_logger;
	std::shared_ptr<spdlog::logger> m_rotating_logger;
	std::shared_ptr<spdlog::logger> m_Daily_logger;
	static LogLevel m_logLevel;
	static string m_filepath;
	static OeasyLog *m_plog;
	static char m_logBuf[MAX_LOG_RECORD_LEN];
};	

#ifdef WIN32
#define FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#else
#define FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif

static OeasyLog &logInstance = OeasyLog::getInstance();

#define OEASYLOG_E(format, ...)		logInstance.Error(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)
#define OEASYLOG_W(format, ...)		logInstance.Warn(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)
#define OEASYLOG_I(format, ...)		logInstance.Info(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)

#endif