
#ifndef LOG_4_CPP
#define LOG_4_CPP

#include<log4cpp/Category.hh>
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/RollingFileAppender.hh>
#include<log4cpp/Priority.hh>
#include<iostream>
#include <string.h> //strrchr()函数所需头文件
#include <stdio.h>
#include <stdarg.h>
#define MAX_LOG_RECORD_LEN    2048

//日志优先级
enum Priority {
    ERROR_LOG,
    WARN_LOG,
    INFO_LOG
};

//用单例模式封装log4cpp
class Oeasylog {
 public: 
    static Oeasylog& getInstance();
	~Oeasylog();
    void destory();
	void setLogFilename( const std::string& fileName = "log4cpp.log");
    void setPriority(Priority priority);
	void Error(const char* filename, const int lineNo, const char* pchFormat, ...);
	void Info(const char* filename, const int lineNo, const char* pchFormat, ...);
	void Warn(const char* filename, const int lineNo, const char* pchFormat, ...);

 private:
    Oeasylog();  //单例模式：构造函数私有化

 private:
	std::string m_logfilename;
	log4cpp::Category &m_category_ref;
    static Oeasylog *m_plog;
	static char m_logBuf[MAX_LOG_RECORD_LEN];
	log4cpp::OstreamAppender *m_os_appender;
	log4cpp::FileAppender *m_file_appender;
};

#ifdef WIN32
#define FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#else
#define FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif

static Oeasylog &logInstance = Oeasylog::getInstance();

#define OEASYLOG_E(format, ...)		logInstance.Error(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)
#define OEASYLOG_W(format, ...)		logInstance.Warn(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)
#define OEASYLOG_I(format, ...)		logInstance.Info(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)

#endif
