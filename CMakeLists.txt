cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(proj_name "ratel")

project (${proj_name})

set(trd_incl "${CMAKE_SOURCE_DIR}/3rd/include")
set(trd_bin "${CMAKE_SOURCE_DIR}/3rd/bin")
set(build_bin "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${build_bin})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${build_bin})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${build_bin})

include_directories(
	${CMAKE_SOURCE_DIR}
	${trd_incl}
)

link_directories(
	${trd_bin} 
	${ratel_bin}
)

add_compile_options(/wd4996 /wd4275 /wd4251)

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/${proj_name}" CACHE PATH "Installation prefix")

set(CMAKE_ISNTALL_INCLUDE "${CMAKE_INSTALL_PREFIX}/include/${proj_name}")

add_subdirectory(basic)

add_subdirectory(test)

file(GLOB HEADERS "${trd_bin}/*d.dll")
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
