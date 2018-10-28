include_directories (
	${HEADER_DIR}
	${THIRD_HEADER_DIR}
	include
)

set(TARGET_NAME ws_common${FILE_PREFIX})

aux_source_directory (source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set (HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/ws_common/ws_const.h
	${HEADER_DIR}/ws_common/ws_request.h
	${HEADER_DIR}/ws_common/ws_response.h
	${HEADER_DIR}/ws_common/ws_http_parser.h
	${HEADER_DIR}/ws_common/ws_msg_parser.h
	${HEADER_DIR}/ws_common/ws_string_tools.h
)

add_library (${TARGET_NAME} STATIC ${SOURCE_LIST} ${HEADER_LIST})
