
set(proj_name "test")

project(${proj_name})

file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/*.cpp")
add_executable(${proj_name} ${SOURCE_FILES})

set_target_properties(${proj_name} PROPERTIES
    DEBUG_POSTFIX _d
)

target_link_libraries(${proj_name} PRIVATE
    spdlog::spdlog
    ratel.basic 
    ratel.geometry 
    ratel.asio 
    ratel.system
)

install(TARGETS ${proj_name} DESTINATION ${CMAKE_INSTALL_BIN})