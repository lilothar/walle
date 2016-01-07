file(GLOB HEADERS "*.h") 
install(FILES ${HEADERS} DESTINATION include/walle/algo)
install(DIRECTORY detail DESTINATION include/walle/algo)
#add_subdirectory(test) 