#include <walle/sys/Config.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <walle/sys/Stream.h>
#include <walle/sys/Logging.h>
#include <walle/sys/StringUtil.h>


using namespace std;
namespace walle {
namespace sys {
	static Config _config;
	
	Config::Config()
	{
	}
    Config::~Config()
    {
        for(SectionMapItr it=_configMap.begin(); it!=_configMap.end(); ++it) {
            delete it->second;
        }
    }
		
	bool Config::parseValue(char *str, string &key, string &val)
    {
        char           *p, *p1, *name, *value;
    
        if (str == NULL)
            return true;
    
        p = str;
        //trim pro space
        while ((*p) == ' ' || (*p) == '\t' || (*p) == '\r' || (*p) == '\n') p++;
        p1 = p + strlen(p);
        //trim tail space
        while(p1 > p) {
            p1 --;
            if (*p1 == ' ' || *p1 == '\t' || *p1 == '\r' || *p1 == '\n') 
				continue;
            p1 ++;
            break;
        }
        (*p1) = '\0';
        //
        if (*p == '#' || *p == '\0') {
			return true;
        }
		
        p1 = strchr(str, '=');
        if (p1 == NULL) {
			return false;
        }
		
        name = p;
        value = p1 + 1;
		
        while ((*(p1 - 1)) == ' ') p1--;
        (*p1) = '\0';
    
        while ((*value) == ' ') value++;
        p = strchr(value, '#');
		
        if (p == NULL) {
			p = value + strlen(value);
        }
		
        while ((*(p - 1)) <= ' ') p--;
        (*p) = '\0';
		
        if (name[0] == '\0')
            return false;
    
        key = name;
        val = value;
    
        return true;
    }
	string Config::isSectionName(char *str) 
	{
		string result;
        if (str == NULL || strlen(str) <= 2 || (*str) != '[') 
            return result;
            
        char *p = str + strlen(str);
        while ((*(p-1)) == ' ' || (*(p-1)) == '\t' || (*(p-1)) == '\r' || 
			(*(p-1)) == '\n') p--;
        if (*(p-1) != ']') return result;
        *(p-1) = '\0';
        
        p = str + 1;
        while(*p) {
            if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || 
				(*p >= '0' && *p <= '9') || (*p == '_')) {
            } else {
                return result;
            }
            p ++;
        }
		result = str+1;
        return result;
    }
	bool Config::loadFromArray(const char* content, size_t len)
	{
			if( !content || len < 1) {
				LOG_ERROR<<"no config content for input";
				return false;
			}
			char			data[4096];
			int 			line = 0;
			bool            ret;
			string          key;
			string          value;
			size_t          readsize;
			BytesStream     s(content, len);

			KeyValueMap *m = NULL;
			while ((readsize = s.readLine(data, (size_t)4096)) >= 1) {
				data[readsize] = '\0';
				line ++;
				string sName = isSectionName(data);
				if (!sName.empty()) {
					SectionMapItr it = _configMap.find(sName);
					if (it == _configMap.end()) {
						m = new KeyValueMap();
						_configMap.insert(SectionMap::value_type(sName, m));
					} else {
						m = it->second;
					}
					continue;
				}
				ret = parseValue(data, key, value);
				if (ret == false) {
					LOG_ERROR<<"parse config error on line: "<<line;
					return false;
				}
				if (key.empty()) {
					continue;
				}
				if (m == NULL) {
					LOG_ERROR<<"parse config error on line: "<<line;
					return false;
				}			 

				KeyValueMapItr it1 = m->find(key);
				if (it1 != m->end()) {
					it1->second += '\0';
					it1->second += value;
				} else {
					m->insert(KeyValueMap::value_type(key, value));
					key.clear();
					value.clear();
				}
			}
			return true;
	}
	bool Config::load(const string &filename)
	{
			FILE		   *fp;
			char			data[4096];
			int 			line = 0;
			bool            ret;
			string          key;
			string          value;
			
			if ((fp = fopen(filename.c_str(), "rb")) == NULL) {
				LOG_ERROR<<"parse config error, open config file error";
				return false;
			}
			
			KeyValueMap *m = NULL;
			while (fgets(data, 4096, fp)) {
				line ++;
				string sName = isSectionName(data);
				if (!sName.empty()) {
					SectionMapItr it = _configMap.find(sName);
					if (it == _configMap.end()) {
						m = new KeyValueMap();
						_configMap.insert(SectionMap::value_type(sName, m));
					} else {
						m = it->second;
					}
					continue;
				}
				ret = parseValue(data, key, value);
				if (ret == false) {
					LOG_ERROR<<"parse config error on line: "<<line;	
					fclose(fp);
					return false;
				}
				if (key.empty()) {
					continue;
				}
				if (m == NULL) {
					LOG_ERROR<<"parse config error on line: "<<line;	
					fclose(fp);
					return false;
				}			 

				KeyValueMapItr it1 = m->find(key);
				if (it1 != m->end()) {
					it1->second += '\0';
					it1->second += value;
				} else {
					m->insert(KeyValueMap::value_type(key, value));
					key.clear();
					value.clear();
				}
			}
			fclose(fp);
			return true;
		}
	
		string Config::getString(const string &section, const string& key, 
											const string &d) const
		{
			SectionMapCItr it = _configMap.find(section);
			if (it == _configMap.end()) {
				return string(d);
			 }
			KeyValueMapCItr it1 = it->second->find(key);
			if (it1 == it->second->end()) {
				return d;
			}
			return it1->second;
		}
		

		vector<string> Config::getStringList(const string &section,
									const string& key, const string &d) const
		{
			(void)d;
			vector<string> ret;
			SectionMapCItr it = _configMap.find(section);
			if (it == _configMap.end()) {
				return ret;
			}
			KeyValueMapCItr it1 = it->second->find(key);
			if (it1 == it->second->end()) {
				return ret;
			}
			const char *data = it1->second.data();
			const char *p = data;
			for(size_t i=0; i<it1->second.size(); i++) {
				if (data[i] == '\0') {
					ret.push_back(p);
					p = data+i+1;
				}
			}
			ret.push_back(p);
			return ret;
		}
	

		int Config::getInt(const char *section, const string& key, int d) const 
		{
			string str = getString(section, key);
			if(str.empty()) {
				return d;
			}
			d = StringUtil::atoix(str);
			return d;
		}
		
		
		vector<int> Config::getIntList(const string &section, const string& key) const
		{
			vector<int> ret;
			SectionMapCItr it = _configMap.find(section);
			if (it == _configMap.end()) {
				return ret;
			}
			KeyValueMapCItr it1 = it->second->find(key);
			if (it1 == it->second->end()) {
				return ret;
			}
			const char *data = it1->second.data();
			const char *p = data;
			for(size_t i=0; i<it1->second.size(); i++) {
				if (data[i] == '\0') {
					ret.push_back(StringUtil::atoix(p));
					p = data+i+1;
				}
			}
			ret.push_back(atoi(p));
			return ret;
		}
		

		size_t Config::getSectionKey(const string &section, vector<string> &keys) const 
		{
			SectionMapCItr it = _configMap.find(section);
			if (it == _configMap.end()) {
				return 0;
			}
			KeyValueMapCItr it1;
			for(it1=it->second->begin(); it1!=it->second->end(); ++it1) {
				keys.push_back(it1->first);
			}
			return keys.size();
		}
				 

		size_t Config::getSectionName(vector<string> &sections) const
		{
			SectionMapCItr it;
			for(it=_configMap.begin(); it!=_configMap.end(); ++it)
			{
				sections.push_back(it->first);
			}
			return sections.size();
		}
	
		// toString
		string Config::toString() const 
		{
			string result;
			SectionMapCItr it;
			KeyValueMapCItr it1;
			for(it = _configMap.begin(); it != _configMap.end(); ++it) {
				result += "[" + it->first + "]\n";
				for(it1=it->second->begin(); it1!=it->second->end(); ++it1) {
					string s = it1->second.c_str();
					result += "    " + it1->first + " = " + s + "\n";
					if (s.size() != it1->second.size()) {
						char *data = const_cast<char*>(it1->second.data());
						char *p = NULL;
						for(size_t i=0; i<it1->second.size(); i++) {
							if (data[i] != '\0') continue;
							if (p) result += "	  " + it1->first + " = " + p + "\n";
							p = data+i+1;
						}
						if (p) result += "	  " + it1->first + " = " + p + "\n";
					}
				}
			}
			result += "\n";    
			return result;
		}
}
}

