#ifndef WALLE_DB_DB_H_
#define WALLE_DB_DB_H_
#include <walle/db/status.h>
#include <string>
#include <stddef.h>

using std::string;

namespace walledb {

class Option;
class WriteBatch;

class Snapshot {
public:
	virtual ~Snapshot();
};
class DB{
public:
	static Status open(const Option&op,
						 const string &path,
						 DB**db);
	virtual Status put(WriteOption*opt, const string & key,
				 const string &meta,
				 const string &value) = 0;

	virtual Status putMeta(WriteOption*opt,const string &key,
						const string &meta) = 0;
	
	virtual Status delete(WriteOption*opt, const string &key) = 0;

	virtual Status get(ReadOption *opt, const string &key,
				 string *value, string *meta) = 0;

	virtual Status getValue(ReadOption *opt, const string &key,
				  		string *value) = 0;

	virtual Status getMeta(ReadOption *opt, const string &key,
				  		string *meta) = 0;

	virtual Status puts(WriteOption*opt, WriteBatch *list) = 0;

	virtual Snapshot* getSnapshot() = 0;

	virtual Iterator* iterator(ReadOption *opt) = 0;

	virtual size_t getSize(const string &start, const string &end) = 0;

	virtual Status compact(const string &start, const string &end) = 0;

private:
	DB();
	DB(const DB&);
	DB& operator=(const DB&);
};

}

#endif