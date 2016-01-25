#ifndef WALLE_DB_ITERATOR_H_
#define WALLE_DB_ITERAOTR_H_
#include <walle/sys/StringPice.h>
#include <walle/algo/funnctional.h>
#include <walle/db/status.h>

using walle::sys::StringPice;

namespace walledb {

class Iterator {
public:
	Iterator();
	virtual ~Iterator();

	virtual bool valid() = 0;

	virtual Status status() = 0;

	virtual bool next() = 0;

	virtual bool prev() = 0;

	virtual bool seekFirst() = 0;

	virtual bool seekEnd() = 0;

	virtual bool seek(const StringPice& targret) = 0;

	virtual StringPice key() = 0;

	virtual StringPice value() = 0;

	virtual StringPice  meta()  = 0;
public:
	typedef std::function<void()> CleanupallBack;

	void registerCleanup(const CleanupallBack&cb);
protected;
	std::vector<CleanupallBack> _callbacks;

};

extern Iterator* NewEmptyIterator();
// Return an empty iterator with the specified status.
extern Iterator* NewErrorIterator(const Status& status);

}
#endif
