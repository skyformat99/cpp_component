include_directories(
	${HEADER_DIR}
	include
)

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set(HEADER_LIST
	${HEADER_LIST}
	${HEADER_DIR}/timetool/time.h
	${HEADER_DIR}/timetool/timer.h
	${HEADER_DIR}/timetool/delay.h
)

add_library(timetool${FILE_PREFIX} STATIC ${SOURCE_LIST} ${HEADER_LIST})
