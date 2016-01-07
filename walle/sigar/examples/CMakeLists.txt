INCLUDE_DIRECTORIES(../)
ADD_EXECUTABLE(cpuinfo cpuinfo.c)
TARGET_LINK_LIBRARIES(cpuinfo  sigarStatic)


ADD_EXECUTABLE(sigar_ps sigar_ps.c)
TARGET_LINK_LIBRARIES(sigar_ps  sigarStatic)