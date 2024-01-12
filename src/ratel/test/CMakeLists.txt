
set(proj_name "test")

project(${proj_name})

file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/*.cpp")
add_executable(${proj_name} ${SOURCE_FILES})

set_target_properties(${proj_name} PROPERTIES
    DEBUG_POSTFIX _d
)

target_compile_definitions(${proj_name} PRIVATE 
	SPDLOG_COMPILED_LIB
	SPDLOG_SHARED_LIB
	SPDLOG_USE_STD_FORMAT
)

target_link_libraries(${proj_name} ratel.basic ratel.geometry ratel.asio ratel.system)

set(deplibs_dbg spdlogd)
set(deplibs_opt spdlog)
target_link_libraries(${proj_name} 
                      debug ${deplibs_dbg} 
                      optimized ${deplibs_opt})

install(TARGETS ${proj_name} DESTINATION ${CMAKE_INSTALL_BIN})