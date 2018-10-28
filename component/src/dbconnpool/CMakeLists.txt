include_directories(
	${HEADER_DIR}
	${THIRD_HEADER_DIR}/mysql
	${THIRD_HEADER_DIR}/mysqlwrap
	include
)

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/dbconnpool/mysqlwrapconnpool.h
)

add_library(dbconnpool${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
