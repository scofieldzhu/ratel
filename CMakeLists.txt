cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(proj_name "Ratel")

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

add_subdirectory(basic)

add_subdirectory(test)

#install(TARGETS ${proj_name} DESTINATION bin)
#install(FILES "${PROJECT_BINARY_DIR}/MyLibraryConfig.h" 
   #     DESTINATION include)

# TODO: 如有需要，请添加测试并安装目标。
