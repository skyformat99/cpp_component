include_directories (
	${HEADER_DIR}
	${THIRD_HEADER_DIR}
	include
)

set(TARGET_NAME ws_server${FILE_PREFIX})

aux_source_directory (source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set (HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/ws_server/ws_server.h
)

add_library (${TARGET_NAME} STATIC ${SOURCE_LIST} ${HEADER_LIST})
