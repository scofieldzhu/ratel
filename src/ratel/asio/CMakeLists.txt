file(GLOB RATEL_ASIO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(ratel.asio SHARED ${RATEL_ASIO_SOURCES})
set_target_properties(ratel.asio PROPERTIES EXPORT_NAME asio)

target_link_libraries(ratel.asio
    PUBLIC ratel_build_settings
    PRIVATE
        spdlog::spdlog
        ratel.basic
        Boost::headers
        Boost::system
)

if(WIN32)
    target_compile_definitions(ratel.asio PRIVATE
        RATEL_ASIO_EXPORTS
        _WIN32_WINNT=0x0601
    )
endif()

file(GLOB RATEL_ASIO_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

install(TARGETS ratel.asio
    EXPORT ratelTargets
    RUNTIME DESTINATION "${RATEL_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${RATEL_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${RATEL_INSTALL_LIBDIR}"
)
install(FILES ${RATEL_ASIO_HEADERS}
    DESTINATION "${RATEL_INSTALL_INCLUDEDIR}/asio"
)
