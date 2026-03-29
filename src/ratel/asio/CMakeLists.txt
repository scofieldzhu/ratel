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

set_target_properties(${proj_name} PROPERTIES
    DEBUG_POSTFIX _d
)

if(WIN32)
	target_compile_definitions(${proj_name} PRIVATE RATEL_ASIO_EXPORTS -D_WIN32_WINNT=0x0601)
endif()

target_link_libraries(${proj_name} PRIVATE 
	spdlog::spdlog 
	ratel.basic 
	Boost::boost 
	Boost::system
)

install(TARGETS ${proj_name} RUNTIME DESTINATION ${CMAKE_INSTALL_BIN})
install(TARGETS ${proj_name} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIB})

file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDE}/asio)
