#include "oeasylog.h"

OeasyLog* OeasyLog::m_plog = NULL;
string OeasyLog::m_filepath = "logs/OeasyMidSDKlog";
LogLevel OeasyLog::m_logLevel = INFO_LOG;

char OeasyLog::m_logBuf[MAX_LOG_RECORD_LEN] = { 0 };

OeasyLog::OeasyLog()
{
	
}
OeasyLog::~OeasyLog()
{

}

void OeasyLog::setLogLevel(LogLevel level)
{
	m_logLevel = level;
}

#include <io.h>  
#include <direct.h>  
#define PATH_DELIMITER '/' 

static bool createDirectory(const std::string folder) {
	std::string folder_builder;
	std::string sub;
	sub.reserve(folder.size());
	for (auto it = folder.begin(); it != folder.end(); ++it)
	{
		//cout << *(folder.end()-1) << endl;  
		const char c = *it;
		sub.push_back(c);
		if (c == PATH_DELIMITER || it == folder.end() - 1)
		{
			folder_builder.append(sub);
			if (0 != ::_access(folder_builder.c_str(), 0))
			{
				// this folder not exist  
				if (0 != ::_mkdir(folder_builder.c_str()))
				{
					// create failed  
					return false;
				}
			}
			sub.clear();
		}
	}
	return true;
}

OeasyLog& OeasyLog::getInstance()
{
	if (m_plog == NULL) {
		m_plog = new OeasyLog;
		bool ret = createDirectory("MidSDKlogs");
		if (ret)
		{
			m_plog->CreateLogInstance("MidSDKlogs/sdklog");
		}
		else 
		{
			m_plog->CreateLogInstance("sdklog");
		}
	}
	return *m_plog;
}

std::shared_ptr<spdlog::logger> OeasyLog::CreateLogInstance(string filepath, LogType  type)
{
	m_consol_logger = spdlog::stdout_color_mt("console");
	std::shared_ptr<spdlog::logger> ret = m_consol_logger;
	try
	{
		switch (type)
		{
		case Console_LOG:
			ret = spdlog::stdout_color_mt("console");
			break;
		case Rotating_LOG:
			// Create a file rotating logger with 5mb size max and 5 rotated files
			ret = spdlog::rotating_logger_mt("rotating", filepath, 1024 * 1024 * 5, 5);
			break;
		case Daily_LOG:
			// Create a daily logger - a new file is created every day on 2:30am
			ret = spdlog::daily_logger_mt("daily", filepath, 2, 30);
			break;
		default:
			break;
		}
	}
	catch (const spdlog::spdlog_ex& ex)
	{
		std::cout << "Log init failed: " << ex.what() << std::endl;
	}
	m_cur_logger = ret;
	return ret;
}

void OeasyLog::Error(const char* filename, const int lineNo, const char* pchFormat, ...)
{
	memset(m_logBuf, 0, MAX_LOG_RECORD_LEN);
	int strLen = 0;
	// 日志信息
	va_list list;
	va_start(list, pchFormat);
	strLen += vsprintf(m_logBuf + strLen, pchFormat, list);
	va_end(list);
	strLen += sprintf(m_logBuf + strLen, " (%s:%d)", filename, lineNo);

	if (m_consol_logger != m_cur_logger)
	{
		m_cur_logger->error("{}", m_logBuf);
		m_cur_logger->flush();
	}
	m_consol_logger->error("{}", m_logBuf);
	m_consol_logger->flush();

}

void OeasyLog::Info(const char* filename, const int lineNo, const char* pchFormat, ...)
{
	memset(m_logBuf, 0, MAX_LOG_RECORD_LEN);
	int strLen = 0;
	// 日志信息
	va_list list;
	va_start(list, pchFormat);
	strLen += vsprintf(m_logBuf + strLen, pchFormat, list);
	va_end(list);
	strLen += sprintf(m_logBuf + strLen, "  (%s:%d)", filename, lineNo);
	if (m_consol_logger != m_cur_logger)
	{
		m_cur_logger->info("{}", m_logBuf);
		m_cur_logger->flush();
	}
	m_consol_logger->info("{}", m_logBuf);
	m_consol_logger->flush();
}

void OeasyLog::Warn(const char* filename, const int lineNo, const char* pchFormat, ...)
{
	memset(m_logBuf, 0, MAX_LOG_RECORD_LEN);
	int strLen = 0;
	// 日志信息
	va_list list;
	va_start(list, pchFormat);
	strLen += vsprintf(m_logBuf + strLen, pchFormat, list);
	va_end(list);
	strLen += sprintf(m_logBuf + strLen, "  (%s:%d)", filename, lineNo);
	if (m_consol_logger != m_cur_logger)
	{
		m_cur_logger->warn("{}", m_logBuf);
		m_cur_logger->flush();
	}
	m_consol_logger->warn("{}", m_logBuf);
	m_consol_logger->flush();
}
