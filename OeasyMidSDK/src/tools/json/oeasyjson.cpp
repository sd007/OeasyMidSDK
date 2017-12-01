#include "oeasyjson.h"
#include "oeasylog.h"


OeasyJson::OeasyJson()
{

}

OeasyJson::~OeasyJson()
{

}

int OeasyJson::parseString( string str )
{
	//if (!m_reader.parse(str, m_readRoot, false))
	//{
	//	OEASYLOG_E("parseString %s, error!", str.c_str());
	//	return -1;
	//}
	return 0;
}

std::string OeasyJson::getString( string str, string key )
{
	//int ret = parseString(str);
	//if (ret == 0 )
	//{
	//	return  m_readRoot[key].asString();
	//}
	return "";
}

int OeasyJson::getInt( string str, string key )
{
	//int ret = parseString(str);
	//if (ret == 0 )
	//{
	//	return  m_readRoot[key].asInt();
	//}
	return -1;
}

double OeasyJson::getDouble( string str, string key )
{
	//int ret = parseString(str);
	//if (ret == 0 )
	//{
	//	return  m_readRoot[key].asDouble();
	//}
	return -1;
}

//std::string OeasyJson::writeJson(vector<pair<string, string>> valueVector)
//{
//	Json::Value tempJsonValue;
//
//	for(int i =0; i < valueVector.size(); i++)
//	{
//		pair<string, string> value = valueVector.at(i);
//		tempJsonValue[value.first] = value.second;
//	}
//	m_writeRoot.append(tempJsonValue);
//	std::string json_str= m_writer.write(m_writeRoot);
//	return json_str;
//}
