set(proj_name "basic")

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

target_compile_definitions(${proj_name} PRIVATE 
	SPDLOG_COMPILED_LIB
	SPDLOG_SHARED_LIB
	SPDLOG_USE_STD_FORMAT
)

if(WIN32)
	target_compile_definitions(${proj_name} PRIVATE BASIC_EXPORTS)
endif()

set(deplibs_dbg spdlogd)
set(deplibs_opt spdlog)
target_link_libraries(${proj_name} debug ${deplibs_dbg} optimized ${deplibs_opt})

install(TARGETS ${proj_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${proj_name})
