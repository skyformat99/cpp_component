include_directories(
	${HEADER_DIR}
	${HEADER_DIR}/curl_client
	${THIRD_HEADER_DIR}
	include
)

set(TARGET_NAME curlclient${FILE_PREFIX})

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "./include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/curl_client/curl_param.h
	${HEADER_DIR}/curl_client/curl_client.h
)

add_library(${TARGET_NAME} STATIC ${SOURCE_LIST} ${HEADER_LIST})
