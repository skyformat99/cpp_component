include_directories(
	${HEADER_DIR}
	include
)

link_directories (
	${LINK_LIBRARIES_DIR}
)

aux_source_directory(source SOURCE_LIST)
FILE (GLOB HEADER_LIST "include/*.h")

set (HEADER_LIST
	${HEADER_LIST}
)
IF (${os} STREQUAL "windows") 

add_executable (libeve_https_test${FILE_PREFIX} ${SOURCE_LIST} ${HEADER_LIST})
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries (libeve_https_test${FILE_PREFIX}
		libeve_https${FILE_PREFIX}
		ospathtool${FILE_PREFIX}
		randtool${FILE_PREFIX}
		ws2_32
		libevent libevent_core libevent_extras libevent_openssl
		libcrypto libssl libeay32 ssleay32
		debug http_server_d optimized http_server
	)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries (libeve_https_test${FILE_PREFIX}
		libeve_https${FILE_PREFIX}
		ospathtool${FILE_PREFIX}
		randtool${FILE_PREFIX}
	)

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

endif ()

ENDIF()
