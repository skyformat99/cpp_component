include_directories (
	${HEADER_DIR}
	${HEADER_DIR}/stringtool
)

aux_source_directory (source/ SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")
set (HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/stringtool/stringtool.h
)

add_library (stringtool${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
