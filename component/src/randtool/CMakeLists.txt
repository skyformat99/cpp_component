include_directories(
	${HEADER_DIR}
	${THIRD_HEADER_DIR}
	include
)

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/randtool/uuid.h
)

add_library(randtool${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
