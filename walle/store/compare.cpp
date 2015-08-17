#include <walle/store/compare.h>
#include <walle/store/myconf.h>

namespace walle {
namespace store {

/**
 * Prepared pointer of the comparator in the lexical order.
 */
LexicalComparator lexicalfunc;
LexicalComparator* const LEXICALCOMP = &lexicalfunc;


/**
 * Prepared pointer of the comparator in the lexical descending order.
 */
LexicalDescendingComparator lexicaldescfunc;
LexicalDescendingComparator* const LEXICALDESCCOMP = &lexicaldescfunc;


/**
 * Prepared pointer of the comparator in the decimal order.
 */
DecimalComparator decimalfunc;
DecimalComparator* const DECIMALCOMP = &decimalfunc;


/**
 * Prepared pointer of the comparator in the decimal descending order.
 */
DecimalDescendingComparator decimaldescfunc;
DecimalDescendingComparator* const DECIMALDESCCOMP = &decimaldescfunc;


}                                        // common namespace
}
// END OF FILE
