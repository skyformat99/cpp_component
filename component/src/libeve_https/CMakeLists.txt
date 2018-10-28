include_directories(
	${HEADER_DIR}
	${HEADER_DIR}/libeve_https
	${THIRD_HEADER_DIR}
	include
)

IF (${os} STREQUAL "linux")
	INCLUDE_DIRECTORIES (
		${THIRD_HEADER_DIR}/libevent_linux
	)
ELSE ()
	INCLUDE_DIRECTORIES (
		${THIRD_HEADER_DIR}/libevent
	)
ENDIF ()

set(TARGET_NAME libeve_https${FILE_PREFIX})

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "./include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/libeve_https/http_server.h
	${HEADER_DIR}/libeve_https/libeve_https.h
)

add_library(${TARGET_NAME} STATIC ${HEADER_LIST} ${SOURCE_LIST})
