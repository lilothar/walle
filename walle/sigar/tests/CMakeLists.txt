MACRO(SIGAR_TEST name)
	ADD_EXECUTABLE(${name} ${name}.c)
	TARGET_LINK_LIBRARIES(${name}  sigarStatic)
	ADD_TEST(${name} ${name})
ENDMACRO(SIGAR_TEST name)

INCLUDE_DIRECTORIES(../)

ADD_DEFINITIONS(-DSIGAR_TEST_OS_LINUX)



SIGAR_TEST(t_sigar_cpu)
SIGAR_TEST(t_sigar_fs)
SIGAR_TEST(t_sigar_loadavg)
SIGAR_TEST(t_sigar_mem)
SIGAR_TEST(t_sigar_netconn)
SIGAR_TEST(t_sigar_netif)
SIGAR_TEST(t_sigar_pid)
SIGAR_TEST(t_sigar_proc)
SIGAR_TEST(t_sigar_reslimit)
SIGAR_TEST(t_sigar_swap)
SIGAR_TEST(t_sigar_sysinfo)
SIGAR_TEST(t_sigar_uptime)
# SIGAR_TEST(t_sigar_version)
