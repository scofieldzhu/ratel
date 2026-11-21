file(GLOB RATEL_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_executable(test ${RATEL_TEST_SOURCES})

# VS 生成器下 CMAKE_DEBUG_POSTFIX 不会作用于该可执行目标，显式保留历史命名。
set_target_properties(test PROPERTIES DEBUG_POSTFIX "_d")

target_link_libraries(test
    PRIVATE
        ratel_build_settings
        spdlog::spdlog
        ratel.basic
        ratel.geometry
        ratel.asio
        ratel.system
)

install(TARGETS test
    RUNTIME DESTINATION "${RATEL_INSTALL_BINDIR}"
)
