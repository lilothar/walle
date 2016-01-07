SET(SIGAR_SRC ${SIGAR_SRC}
  sigar.c
  sigar_cache.c
  sigar_fileinfo.c
  sigar_format.c
  sigar_getline.c
  sigar_ptql.c
  sigar_signal.c
  sigar_util.c
  linux_sigar.c
)

ADD_LIBRARY(sigarShared SHARED ${SIGAR_SRC})
ADD_LIBRARY(sigarStatic STATIC ${SIGAR_SRC})
set_target_properties(sigarShared PROPERTIES OUTPUT_NAME sigar)
set_target_properties(sigarStatic PROPERTIES OUTPUT_NAME sigar)
ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(examples)
INSTALL(FILES sigar.h 
	sigar_fileinfo.h 
	sigar_format.h 
	sigar_getline.h 
	sigar_log.h 
	sigar_private.h 
	sigar_ptql.h 
	sigar_util.h 
	DESTINATION include/walle/sigar
	)
INSTALL(TARGETS sigarShared sigarStatic
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	)

