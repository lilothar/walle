include (InstallRequiredSystemLibraries)
#license file path

SET (CPACK_RESOURCE_FILE_LICENSE   "${PROJECT_SOURCE_DIR}/LICENSE")
SET (CPACK_RESOURCE_FILE_README    "${PROJECT_SOURCE_DIR}/README.md")
SET(CPACK_PACKAGE_CONTACT "liyb@fastweb.com.cn")
#centos red hat 
if(SYSTEM_PLATE MATCHES ubuntu)
    set (CPACK_GENERATOR "DEB")
elseif(SYSTEM_PLATE MATCHES centos)
    set (CPACK_GENERATOR "RPM")
else ()
    set (CPACK_GENERATOR "TAZ")
endif()

set (CPACK_SOURCE_GENERATOR ${CPACK_SOURCE_ZIP})
set (CPACK_SOURCE_PACKAGE_FILE_NAME walle-source)
#set (CPACK_PACKAGE_INSTALL_DIRECTORY /usr/local/walle)
#message(STATUS "package type ${PACK}")
set (CPACK_PACKAGING_INSTALL_PREFIX /usr/local/walle)

#ubuntu debian
#set (CPACK_GENERATOR "DEB")
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")  
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}") 

include (CPack) 

