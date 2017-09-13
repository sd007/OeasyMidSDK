#include<iostream>
#include "log4cpp.h"

using namespace std;

#define MAX_LOGSIZE 1024*1024
#define LOGFILE_SIZE 50

char Oeasylog::m_logBuf[MAX_LOG_RECORD_LEN] = {0};

Oeasylog* Oeasylog::m_plog = NULL;

//获取log指针
Oeasylog& Oeasylog::getInstance() {
	if ( m_plog == NULL ) {
		m_plog = new Oeasylog;
	}

	return *m_plog;
}

//销毁
Oeasylog::~Oeasylog() {

}


//销毁
void Oeasylog::destory() {
	if (m_plog) {
		m_plog->m_category_ref.info("log4cpp destroy");
		m_plog->m_category_ref.shutdown();
		delete m_plog;
		m_plog = NULL;
	}
}


//构造函数
Oeasylog::Oeasylog(): 
m_category_ref(log4cpp::Category::getRoot()) ,
m_logfilename (std::string("log4cpp.log"))
{
	//自定义输出格式
	log4cpp::PatternLayout *pattern_one =
		new log4cpp::PatternLayout;
	pattern_one->setConversionPattern("%d: %p %c %x:%m%n");

	log4cpp::PatternLayout *pattern_two =
		new log4cpp::PatternLayout;
	pattern_two->setConversionPattern("%d: %p %c %x:%m%n");

	//获取屏幕输出
	log4cpp::OstreamAppender *os_appender = 
		new log4cpp::OstreamAppender("osAppender",&std::cout);
	os_appender->setLayout(pattern_one);

	m_os_appender = os_appender;
	//获取文件日志输出 
	//http://blog.csdn.net/wyb19890515/article/details/7187057
	
	log4cpp::FileAppender *file_appender = 
		new log4cpp::RollingFileAppender("RollingFileAppender", m_logfilename, MAX_LOGSIZE, LOGFILE_SIZE);
	file_appender->setLayout(pattern_two);
	m_file_appender = file_appender;

	m_category_ref.setPriority(log4cpp::Priority::DEBUG);
	m_category_ref.addAppender(os_appender);
	m_category_ref.addAppender(file_appender);

	m_category_ref.info("log4cpp created!");
}


//设置优先级
void Oeasylog::setPriority(Priority priority) {
	switch (priority) {
	case (ERROR_LOG):
		m_category_ref.setPriority(log4cpp::Priority::ERROR);
		break;

	case (WARN_LOG):
		m_category_ref.setPriority(log4cpp::Priority::WARN);
		break;

	case (INFO_LOG):
		m_category_ref.setPriority(log4cpp::Priority::INFO);
		break;
	default:
		m_category_ref.setPriority(log4cpp::Priority::DEBUG);
		break;    
	}
}

void Oeasylog::setLogFilename( const std::string& fileName /*= "log4cpp.log"*/ )
{
	m_logfilename = fileName;
}


void Oeasylog::Error( const char* filename, const int lineNo, const char* pchFormat, ... )
{
	memset(m_logBuf, 0 , MAX_LOG_RECORD_LEN);
	int strLen = 0;
	// 日志信息
	va_list list;
	va_start(list, pchFormat);
	strLen += vsprintf(m_logBuf + strLen, pchFormat, list);
	va_end(list);
	strLen += sprintf(m_logBuf + strLen, "  (%s:%d)", filename, lineNo);
	m_category_ref.error(m_logBuf);
}

void Oeasylog::Info( const char* filename, const int lineNo, const char* pchFormat, ... )
{
	memset(m_logBuf, 0 , MAX_LOG_RECORD_LEN);
	int strLen = 0;
	// 日志信息
	va_list list;
	va_start(list, pchFormat);
	strLen += vsprintf(m_logBuf + strLen, pchFormat, list);
	va_end(list);
	strLen += sprintf(m_logBuf + strLen, "  (%s:%d)", filename, lineNo);
	m_category_ref.info(m_logBuf);
}

void Oeasylog::Warn( const char* filename, const int lineNo, const char* pchFormat, ... )
{
	memset(m_logBuf, 0 , MAX_LOG_RECORD_LEN);
	int strLen = 0;
	// 日志信息
	va_list list;
	va_start(list, pchFormat);
	strLen += vsprintf(m_logBuf + strLen, pchFormat, list);
	va_end(list);
	strLen += sprintf(m_logBuf + strLen, "  (%s:%d)", filename, lineNo);
	m_category_ref.warn(m_logBuf);
}

