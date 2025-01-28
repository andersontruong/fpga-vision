file(GLOB_RECURSE SOURCES *.cpp *.h)

message(STATUS "${SOURCES}")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)
