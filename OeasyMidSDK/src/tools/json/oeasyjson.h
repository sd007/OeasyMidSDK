#ifndef OEASY_JSON
#define OEASY_JSON

#include "BaseType.h"
#include <fstream>
//#include "json/json.h"
#include <string>
#include <vector>
using namespace std;
class OeasyJson{
public:
	OeasyJson();
	~OeasyJson();
	string getString(string str, string key);
	int getInt(string str, string key);
	double getDouble(string str, string key);

	//string writeJson(vector<pair<string, string>>  valueVector);
private:
	int parseString(string str);
private:
	//Json::Reader m_reader;
	//Json::Value m_readRoot;
	//Json::FastWriter m_writer;
	//Json::Value m_writeRoot;
};

#endif //OEASY_JSON

