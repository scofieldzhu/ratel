file(GLOB RATEL_SYSTEM_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(ratel.system SHARED ${RATEL_SYSTEM_SOURCES})
set_target_properties(ratel.system PROPERTIES EXPORT_NAME system)

target_link_libraries(ratel.system
    PUBLIC ratel_build_settings
    PRIVATE spdlog::spdlog
)

if(WIN32)
    target_compile_definitions(ratel.system PRIVATE RATEL_SYSTEM_EXPORTS)
endif()

file(GLOB RATEL_SYSTEM_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

install(TARGETS ratel.system
    EXPORT ratelTargets
    RUNTIME DESTINATION "${RATEL_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${RATEL_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${RATEL_INSTALL_LIBDIR}"
)
install(FILES ${RATEL_SYSTEM_HEADERS}
    DESTINATION "${RATEL_INSTALL_INCLUDEDIR}/system"
)
