include_directories(
	${HEADER_DIR}
	include
)

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/varianttool/any.h
)

add_library(varianttool${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
