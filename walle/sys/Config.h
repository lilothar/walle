#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>
#include <cstring>
#include <vector>
#include <map>

using std::map;
using std::string;
using std::vector;

namespace walle {
namespace sys {
    
    typedef map<string, string> KeyValueMap;
    typedef KeyValueMap::iterator KeyValueMapItr;
     typedef KeyValueMap::const_iterator KeyValueMapCItr;
    typedef map<string, KeyValueMap*> SectionMap;
    typedef SectionMap::iterator SectionMapItr;
    typedef SectionMap::const_iterator SectionMapCItr;
    
   class Config {
   public:
        Config();
        ~Config();

        bool load(const string &filename);

        bool loadFromArray(const char* content, size_t len);

        string getString(const string &section, const string& key, const string 
        &d ="") const;

        vector<string> getStringList(const string &section,
        const string& key,const string &d = "") const;

        int getInt(char const *section, const std::string& key, int d = 0) const;

        vector<int> getIntList(const string &section, 
                                        const string& key) const;

        size_t getSectionKey(const string &section, 
                                    vector<string> &keys) const;

        size_t getSectionName(vector<string> &sections) const;

        string toString() const ;
         
    private:

            SectionMap _configMap;
            
    private:

        bool parseValue(char *str, string &key, string &val);
        string isSectionName(char *line);     
    };
}
}
#endif 
