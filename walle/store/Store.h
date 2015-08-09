#ifndef WALLE_STORE_H_
#define WALLE_STORE_H_
#include <string>
#include <map>
using std::map;
using std::string;

namespace walle {
namespace store {

typedef map<string, string> StrMap;
typedef map<string, StrMap > RowMap;
typedef pair<string, string> ResPair;
typedef vector<ResPair >     ResVec;
typedef vector<string>       StrVec;
class StoreImpl;

class Store {
	public:
		Store(const char *conf);
		~Store();

		bool open();
		void close();

		//ddl
		bool createTable(const string & tablename);
		bool deleteTable(const string &tablename);
		bool tableExist(const string & tablename);

		//dml

		bool putRow(const string &tablename, 
					const string &rowkey,
					const string &column,
					const string &value);

		bool putRowWithCloumn(const string &tablename,
					 		  const string &rowkey,
					 		  const StrMap &columns);
		bool putRows(const string & tablename
						 const RowMap& rows);

		bool getRow(const string & tablname,
					const string & row,  
					ResVec& result);

		bool getRowWithCloumn(const string & tablname,
					const string & row,
					const StrVec &column, 	  
					ResVec& result);

		bool getRows(const string &tablename,
						 const StrVec &row,
						 ResVec& result
						);
	   bool getRowsWithCloumn(const string & tablname,
					const StrVec& row,
					const StrVec &column, 	  
					ResVec& result);
	   bool delRow(const string &tablename,
	   				   const string &row
	   				);
	   bool delRowWithColumn(const string &tablename,
	   				         const string &row,
	   				         const string &column
	   				         );
	   bool delRowWithColumns(const string &tablename,
	   						  const string &row,
	   						  const StrVec &columna
	   						  );
	   bool scan(const string &table,
	   				const string &startrow,
	   				const StrVec &columns,
	   				ResVec& result );
	    bool scanWithStop(const string &table,
	   				const string &startrow,
	   				const string &stoprow,
	   				const StrVec &columns,
	   				ResVec& result );
	private:
		StoreImpl *_impl;
};

}

}
#endif
