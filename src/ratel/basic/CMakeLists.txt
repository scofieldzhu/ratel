set(proj_name "ratel.basic")

project(${proj_name})

file(GLOB
	 SOURCE_FILES 
	 "*.cpp"
	 "*.h"
	 "*.hpp"
)
add_library(${proj_name} SHARED ${SOURCE_FILES})

set_target_properties(${proj_name} PROPERTIES
    DEBUG_POSTFIX _d
)

if(WIN32)
	target_compile_definitions(${proj_name} PRIVATE BASIC_EXPORTS)
endif()

target_link_libraries(${proj_name} PRIVATE
	spdlog::spdlog
)

install(TARGETS ${proj_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BIN})
install(TARGETS ${proj_name} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIB})
file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDE}/basic)