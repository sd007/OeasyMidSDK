
#ifndef LOG_4_CPP
#define LOG_4_CPP

#include<log4cpp/Category.hh>
#include<iostream>
#include <string.h> //strrchr()��������ͷ�ļ�
#include <stdio.h>
#include <stdarg.h>

#define MAX_LOG_RECORD_LEN    2048

//��־���ȼ�
enum Priority {
    ERROR_LOG,
    WARN_LOG,
    INFO_LOG
};

//�õ���ģʽ��װlog4cpp
class Oeasylog {
 public: 
    static Oeasylog& getInstance();
    void destory();
	void setLogFilename( const std::string& fileName = "log4cpp.log");
    void setPriority(Priority priority);
	void Error(const char* filename, const int lineNo, const char* pchFormat, ...);
	void Info(const char* filename, const int lineNo, const char* pchFormat, ...);
	void Warn(const char* filename, const int lineNo, const char* pchFormat, ...);

 private:
    Oeasylog();  //����ģʽ�����캯��˽�л�

 private:
	std::string m_logfilename;
	log4cpp::Category &m_category_ref;
    static Oeasylog *m_plog;
	static char m_logBuf[MAX_LOG_RECORD_LEN];
};

#ifdef WIN32
#define FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#else
#define FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif

static Oeasylog &logInstance = Oeasylog::getInstance();

#define OEASY_E(format, ...)		logInstance.Error(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)
#define OEASY_W(format, ...)		logInstance.Warn(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)
#define OEASY_I(format, ...)		logInstance.Info(FILENAME(__FILE__), __LINE__, format, ##__VA_ARGS__)

#endif