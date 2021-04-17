set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE COMMON_SOURCES main/*.cpp)

file(GLOB_RECURSE GRAPHIC_INTERFACE SFMLInterface/*.cpp)

list(APPEND GRAPHIC_INTERFACE ${GRAPHIC_INTERFACE})
list(APPEND COMMON_SOURCES ${COMMON_SOURCES})

list(FILTER COMMON_SOURCES EXCLUDE REGEX main.cpp$)
# message(${COMMON_SOURCES})

add_executable(${BINARY}_run ${COMMON_SOURCES} main/main.cpp)
add_executable(graphicInterface_run ${GRAPHIC_INTERFACE} ${COMMON_SOURCES})

add_library(${BINARY}_lib STATIC ${COMMON_SOURCES})

target_link_libraries(${BINARY}_run -lpthread)
target_link_libraries(graphicInterface_run -lsfml-graphics -lsfml-window -lsfml-system -lpthread)