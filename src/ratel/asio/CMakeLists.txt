set(proj_name "ratel.asio")

project(${proj_name})

file(GLOB
	 SOURCE_FILES 
	 "*.cpp"
	 "*.h"
	 "*.hpp"
)
add_library(${proj_name} SHARED ${SOURCE_FILES})

find_package(Boost REQUIRED COMPONENTS system)
#target_link_libraries(ratel.asio PRIVATE Boost::boost Boost::system)

set_target_properties(${proj_name} PROPERTIES
    DEBUG_POSTFIX _d
)

target_compile_definitions(${proj_name} PRIVATE 
	SPDLOG_COMPILED_LIB
	SPDLOG_SHARED_LIB
	SPDLOG_USE_STD_FORMAT
)

if(WIN32)
	target_compile_definitions(${proj_name} PRIVATE RATEL_ASIO_EXPORTS -D_WIN32_WINNT=0x0601)
	#target_compile_definitions(${proj_name} PRIVATE GEOMETRY_EXPORTS PROTOBUF_USE_DLLS _CRT_SECURE_NO_WARNINGS NOMINMAX)
	#
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(deplibs spdlogd)
else()
	set(deplibs spdlog)
endif()
target_link_libraries(${proj_name} PRIVATE ${deplibs} ratel.basic Boost::boost Boost::system)

# if(WIN32)
# 	set_target_properties(${PROJECT_NAME} PROPERTIES
# 		PREFIX "ratel."
# 		SUFFIX ".dll"
# 	)
# endif()

# add_custom_target(UpdateLicenseHeader
#     COMMAND LicenseHeaderManager -t ${license_file} -p ${CMAKE_SOURCE_DIR} -r
#     COMMENT "Updating license headers"
# )
# add_dependencies(${proj_name} UpdateLicenseHeader)

install(TARGETS ${proj_name} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS ${proj_name} ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${CMAKE_ISNTALL_INCLUDE}/asio)
