cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(proj_name "ratel")

project (${proj_name})

set(trd_incl "${CMAKE_SOURCE_DIR}/3rd/include")
set(trd_bin "${CMAKE_SOURCE_DIR}/3rd/bin")
set(trd_lib "${CMAKE_SOURCE_DIR}/3rd/lib")
set(build_bin "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${build_bin})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${build_bin})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${build_bin})

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${trd_incl}
)

link_directories(
	${trd_lib} 
)

add_compile_options(/wd4996 /wd4275 /wd4251)

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/${proj_name}" CACHE PATH "Installation prefix")

set(CMAKE_ISNTALL_INCLUDE "${CMAKE_INSTALL_PREFIX}/include/${proj_name}")

set(license_file "${CMAKE_CURRENT_SOURCE_DIR}/doc/license_template.txt" )

add_subdirectory(src/ratel/basic)

add_subdirectory(src/ratel/geometry)

add_subdirectory(src/ratel/asio)

add_subdirectory(src/ratel/system)

add_subdirectory(src/ratel/test)

# install third-party libraries
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	install(FILES ${trd_bin}/spdlogd.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
else()
	install(FILES ${trd_bin}/spdlog.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()
