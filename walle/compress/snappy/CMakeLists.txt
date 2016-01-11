
ADD_LIBRARY(snappy_static STATIC
    snappy-c.cc  snappy-sinksource.cc  snappy-stubs-internal.cc  snappy.cc)

ADD_LIBRARY(snappy SHARED
    snappy-c.cc  snappy-sinksource.cc  snappy-stubs-internal.cc  snappy.cc)

INSTALL(FILES snappy.h snappy-stubs-public.h
        DESTINATION include/walle/compress)
INSTALL(TARGETS snappy snappy_static DESTINATION lib)
