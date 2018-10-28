include_directories(
	${HEADER_DIR}
	${THIRD_HEADER_DIR}
	include
)

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/configtool/configbase.h
	${HEADER_DIR}/configtool/jsonconfig.h
	${HEADER_DIR}/configtool/kvjsonconfig.h
)

add_library(configtool${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
