
ADD_LIBRARY(quicklzShared SHARED quicklz.c)
ADD_LIBRARY(quicklzStatic STATIC quicklz.c)

set_target_properties(quicklzShared  PROPERTIES OUTPUT_NAME quicklz)
set_target_properties(quicklzStatic  PROPERTIES OUTPUT_NAME quicklz)

ADD_LIBRARY(snappyShared SHARED snappy.c)
ADD_LIBRARY(snappyStatic STATIC snappy.c)

set_target_properties(snappyShared  PROPERTIES OUTPUT_NAME snappy)
set_target_properties(snappyStatic  PROPERTIES OUTPUT_NAME snappy)

INSTALL(FILES quicklz.h snappy.h snappy_compact.h DESTINATION include/walle/compress)

INSTALL(TARGETS quicklzShared quicklzStatic snappyStatic snappyShared DESTINATION lib)

