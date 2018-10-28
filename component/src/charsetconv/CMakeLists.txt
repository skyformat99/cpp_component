include_directories(
	${HEADER_DIR}
	include
)

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/charsetconv/charsetconv.h
)

add_library(charsetconv${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
