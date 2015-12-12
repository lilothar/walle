#include <iostream>
#include <fstream>
#include <cstdlib>
#include <walle/sys/Properties.h>
#include <walle/sys/Logging.h>

using namespace walle::sys;

Properties::Properties()
{
    propmap = new std::map<std::string, std::string>;
}

Properties::Properties(const Properties &p)
{
    propmap = new std::map<std::string, std::string>(*(p.propmap));
}

Properties::~Properties()
{
    delete propmap;
}

int Properties::loadProperties(const char* fileName, char delimiter, bool verbose, bool multiline /*=false*/)
{
    std::ifstream input(fileName);
    std::string line;

    if(!input.is_open()) 
    {
        LOG_ERROR<<  "Properties::loadProperties() Could not open the file:" << fileName ;
        return(-1);
    }
    loadProperties(input, delimiter, verbose, multiline);
    input.close();
    return 0;
}

int Properties::loadProperties(std::istream &ist, char delimiter, bool verbose, bool multiline /*=false*/)
{
    std::string line;

    while(ist)
    {
        getline(ist, line);                       //read one line at a time
        if  (line.find('#') == 0)
            continue;                               //ignore comments
        std::string::size_type pos =
            line.find(delimiter);                   //find the delimiter
        
        if( pos == line.npos )
            continue;                               //ignore if no delimiter is found
        std::string key = line.substr(0,pos);       // get the key
        key = removeLTSpaces(key);
        std::string value = line.substr(pos+1);     //get the value
        value = removeLTSpaces(value);

		if (multiline)
        	(*propmap)[key] += value;					// allow properties to be spread across multiple lines
        else
        	(*propmap)[key] = value;
        
        if( verbose)
            LOG_INFO << "Loading key " << key  << " with value " << (*propmap)[key] ;
    }
    return 0;
}



void Properties::setValue(const std::string key, const std::string value) {
    (*propmap)[key] = value;
    return;
}

std::string Properties::removeLTSpaces(std::string str){

    char const* delims = " \t\r\n";

    // trim leading whitespace
    std::string::size_type  notwhite = str.find_first_not_of(delims);
    str.erase(0,notwhite);

   // trim trailing whitespace
   notwhite = str.find_last_not_of(delims);
   str.erase(notwhite+1);
   return(str);
}

std::string Properties::getValue(std::string key, std::string def) const
{
    if(propmap->find(key) == propmap->end()) return def;
        return (*propmap)[key];
}
const char* Properties::getValue(std::string key, const char* def) const
{
    if(propmap->find(key) == propmap->end()) return def;
        return (((*propmap)[key]).c_str());
}

int Properties::getValue(std::string key, int def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atoi(((*propmap)[key]).c_str()));
}
long Properties::getValue(std::string key, long def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atoll(((*propmap)[key]).c_str()));
}

long long Properties::getValue(std::string key, long long def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atoll(((*propmap)[key]).c_str()));
}

uint64_t Properties::getValue(std::string key, uint64_t def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atoll(((*propmap)[key]).c_str()));
}

double Properties::getValue(std::string key, double def)
{
    if(propmap->find(key) == propmap->end()) return def;
        return (atof(((*propmap)[key]).c_str()));
}

void Properties::getList(std::string &outBuf, std::string linePrefix) const {
  std::map<std::string, std::string>::iterator iter;

  for (iter = propmap->begin(); iter != propmap->end(); iter++) {
    if ((*iter).first.size() > 0) {
      outBuf += linePrefix;
      outBuf += (*iter).first;
      outBuf += '=';
      outBuf += (*iter).second;
      outBuf += '\n';
    }
  }

  return;
}
