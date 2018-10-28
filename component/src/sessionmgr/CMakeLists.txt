include_directories(
	${HEADER_DIR}
	${HEADER_DIR}/sessionmgr/
	${THIRD_HEADER_DIR}
	${THIRD_HEADER_DIR}/sqlite3
	include
)

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/sessionmgr/sessionmgr.h
)

add_library(sessionmgr${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
