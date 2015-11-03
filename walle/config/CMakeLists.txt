
set(SYSTEM_NAME ${CMAKE_SYSTEM_NAME})

configure_file (
  "config.h.in"
  "config.h"
  )

install (FILES "${PROJECT_BINARY_DIR}/walle/config/config.h" DESTINATION include/walle/config)

   
