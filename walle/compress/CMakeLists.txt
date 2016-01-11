
ADD_LIBRARY(quicklzShared SHARED quicklz.c)
ADD_LIBRARY(quicklzStatic STATIC quicklz.c)

set_target_properties(quicklzShared  PROPERTIES OUTPUT_NAME quicklz)
set_target_properties(quicklzStatic  PROPERTIES OUTPUT_NAME quicklz)

INSTALL(FILES quicklz.h DESTINATION include/walle/compress)

INSTALL(TARGETS quicklzShared quicklzStatic DESTINATION lib)

