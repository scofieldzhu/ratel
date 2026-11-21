file(GLOB RATEL_GEOMETRY_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(ratel.geometry SHARED ${RATEL_GEOMETRY_SOURCES})
set_target_properties(ratel.geometry PROPERTIES EXPORT_NAME geometry)

target_link_libraries(ratel.geometry
    PUBLIC ratel_build_settings
    PRIVATE spdlog::spdlog
)

if(WIN32)
    target_compile_definitions(ratel.geometry PRIVATE RATEL_GEOMETRY_EXPORTS)
endif()

file(GLOB RATEL_GEOMETRY_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

install(TARGETS ratel.geometry
    EXPORT ratelTargets
    RUNTIME DESTINATION "${RATEL_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${RATEL_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${RATEL_INSTALL_LIBDIR}"
)
install(FILES ${RATEL_GEOMETRY_HEADERS}
    DESTINATION "${RATEL_INSTALL_INCLUDEDIR}/geometry"
)
