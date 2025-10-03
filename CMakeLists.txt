cmake_minimum_required(VERSION 3.15)


project(
 lambda_interpreter
 VERSION 0.0.1
 LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SUBMODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g")


file(GLOB_RECURSE SRCS ${SOURCE_DIR}/*.c)

add_executable(${PROJECT_NAME} ${SRCS})


add_custom_target(proof
 COMMAND ivette -wp-rte -wp-smoke-tests -wp-counter-examples
    -wp-prover "Z3,cvc5,Alt-Ergo"
    ${SRCS}
 COMMENT "Lauching Ivette"
)
