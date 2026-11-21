file(GLOB RATEL_BASIC_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(ratel.basic SHARED ${RATEL_BASIC_SOURCES})
set_target_properties(ratel.basic PROPERTIES EXPORT_NAME basic)

target_link_libraries(ratel.basic
    PUBLIC ratel_build_settings
    PRIVATE spdlog::spdlog
)

if(WIN32)
    target_compile_definitions(ratel.basic PRIVATE RATEL_BASIC_EXPORTS)
endif()

file(GLOB RATEL_BASIC_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

install(TARGETS ratel.basic
    EXPORT ratelTargets
    RUNTIME DESTINATION "${RATEL_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${RATEL_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${RATEL_INSTALL_LIBDIR}"
)
install(FILES ${RATEL_BASIC_HEADERS}
    DESTINATION "${RATEL_INSTALL_INCLUDEDIR}/basic"
)
