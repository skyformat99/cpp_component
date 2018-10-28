include_directories (
	${HEADER_DIR}
	${HEADER_DIR}/ospathtool
)

aux_source_directory (source/ SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")
set (HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/ospathtool/ospathtool.h
)

add_library (ospathtool${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
