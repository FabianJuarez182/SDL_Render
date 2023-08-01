cmake_minimum_required(VERSION 3.16)

project(EXEC VERSION 1.0)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

# Find SDL2 library
find_package(SDL2 REQUIRED)

target_link_libraries(${PROJECT_NAME}
  ${SDL2_LIBRARIES}
)
