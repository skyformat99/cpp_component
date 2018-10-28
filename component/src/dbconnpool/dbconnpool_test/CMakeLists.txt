INCLUDE_DIRECTORIES(
	${HEADER_DIR}
	${HEADER_DIR}/cbb11commonlogic
	${THIRD_HEADER_DIR}
	include
)

LINK_DIRECTORIES (
	${LINK_LIBRARIES_DIR}
)

AUX_SOURCE_DIRECTORY(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

SET (HEADER_LIST
	${HEADER_LIST}
)

SET (TARGET_NAME
	dbconnpool_test${FILE_PREFIX}
)

IF (${os} STREQUAL "windows")
		ADD_EXECUTABLE (${TARGET_NAME} ${SOURCE_LIST} ${HEADER_LIST})
		TARGET_LINK_LIBRARIES (${TARGET_NAME}
			dbconnpool${FILE_PREFIX}
			debug mysql_wrap_d optimized mysql_wrap
			libmysql
		)
ELSEIF (${os} STREQUAL "linux")	
		IF (${PLATFORM_TYPE} MATCHES "arm")
			MESSAGE(STATUS "varianttool_test arm not compile")
		ELSE()
				ADD_EXECUTABLE (${TARGET_NAME} ${SOURCE_LIST} ${HEADER_LIST})
				TARGET_LINK_LIBRARIES (${TARGET_NAME}
					dbconnpool${FILE_PREFIX}
					debug mysql_wrap_d optimized mysql_wrap
					libmysql
				)
		
				SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
				SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
		
		
				IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")	
						ADD_CUSTOM_COMMAND(TARGET ${TARGET_NAME} POST_BUILD COMMAND echo "strip"
									COMMAND ${STRIP} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${TARGET_NAME}
									COMMAND cp ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${TARGET_NAME} ~/nfs/${PLATFORM_NAME} -f
					)
				ENDIF()	
		ENDIF()
ENDIF()