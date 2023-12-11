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

# configure_file(
#     ${license_file}
#     COPYONLY
# )
# add_custom_target(UpdateLicenseHeader
#     COMMAND LicenseHeaderManager -t ${license_file} -p ${CMAKE_SOURCE_DIR} -r
#     COMMENT "Updating license headers"
# )
# add_dependencies(${proj_name} UpdateLicenseHeader)

install(TARGETS ${proj_name} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS ${proj_name} ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${CMAKE_ISNTALL_INCLUDE}/${proj_name})



