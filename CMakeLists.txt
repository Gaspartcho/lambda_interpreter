cmake_minimum_required(VERSION 3.15)


project(
	lambda_interpreter
	VERSION 0.0.1
	LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g")


file(GLOB_RECURSE SRCS ${SOURCE_DIR}/*.c ${SOURCE_DIR}/*.h )

add_executable(${PROJECT_NAME} ${SRCS})


add_custom_target(valgrind
	COMMAND valgrind -s $<TARGET_FILE:${PROJECT_NAME}>
	DEPENDS ${PROJECT_NAME}
	COMMENT "Launching Valgrind"
)

add_custom_target(run
	COMMAND $<TARGET_FILE:${PROJECT_NAME}>
	DEPENDS ${PROJECT_NAME}
	COMMENT "Launching ${PROJECT_NAME}"
)
