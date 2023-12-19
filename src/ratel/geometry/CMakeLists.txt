set(proj_name "ratel.geometry")

project(${proj_name})

# set(Protobuf_INCLUDE_DIR ${trd_incl})
# set(Protobuf_LIBRARIES ${trd_lib})

# set(Protobuf_PROTOC_EXECUTABLE ${trd_bin}/protoc.exe)

# find_package(Protobuf REQUIRED)

# set(msg_proto_dir ${CMAKE_CURRENT_SOURCE_DIR}/protos)
# file(GLOB msg_protos "${msg_proto_dir}/*.proto")
# set(proto_files "")
# foreach(full_path ${msg_protos})
# 	get_filename_component(filename ${full_path} NAME)
# 	list(APPEND proto_files ${filename})
# endforeach()

# if(WIN32)
# 	execute_process(
# 		COMMAND ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=dllexport_decl=RATEL_GEOMETRY_API:. -I ${msg_proto_dir} ${proto_files}
# 		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
# 	)
# else()
# 	execute_process(
# 		COMMAND ${Protobuf_PROTOC_EXECUTABLE} --cpp_out=. -I ${msg_proto_dir} ${proto_files}
# 		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
# 	)
# endif()

file(GLOB
	 SOURCE_FILES 
	 "*.cpp"
	 "*.h"
	 "*.hpp"
	 #"*.cc"
)
add_library(${proj_name} SHARED ${SOURCE_FILES})

set_target_properties(${proj_name} PROPERTIES
    DEBUG_POSTFIX _d
)

target_compile_definitions(${proj_name} PRIVATE 
	SPDLOG_COMPILED_LIB
	SPDLOG_SHARED_LIB
	SPDLOG_USE_STD_FORMAT
)

if(WIN32)
	target_compile_definitions(${proj_name} PRIVATE GEOMETRY_EXPORTS)
	#target_compile_definitions(${proj_name} PRIVATE GEOMETRY_EXPORTS PROTOBUF_USE_DLLS _CRT_SECURE_NO_WARNINGS NOMINMAX)
	#
endif()

set(deplibs_dbg spdlogd)
set(deplibs_opt spdlog)
target_link_libraries(${proj_name} debug ${deplibs_dbg} optimized ${deplibs_opt})

# if(WIN32)
# 	set_target_properties(${PROJECT_NAME} PROPERTIES
# 		PREFIX "ratel."
# 		SUFFIX ".dll"
# 	)
# endif()

# configure_file(
#     ${license_file}
#     COPYONLY
# )
# add_custom_target(UpdateLicenseHeader
#     COMMAND LicenseHeaderManager -t ${license_file} -p ${CMAKE_SOURCE_DIR} -r
#     COMMENT "Updating license headers"
# )
# add_dependencies(${proj_name} UpdateLicenseHeader)

install(TARGETS ${proj_name} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS ${proj_name} ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

file(GLOB HEADERS "*.h" "*.hpp")
install(FILES ${HEADERS} DESTINATION ${CMAKE_ISNTALL_INCLUDE}/basic)
