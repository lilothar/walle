file(GLOB HEADERS "*.h") 
install(FILES ${HEADERS} DESTINATION include/eva/sys)
add_subdirectory(test) 