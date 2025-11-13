cmake_minimum_required(VERSION 3.31)

project(labyrinth)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# Set properties that will be used while building TGUI
set(BUILD_SHARED_LIBS OFF)      # Determines whether TGUI build a static or dynamic/shared library
set(TGUI_BACKEND SFML_GRAPHICS)  # Sets which backend TGUI will use


include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
#   DOWNLOAD_EXTRACT_TIMESTAMP TRUE
# )
# FetchContent_MakeAvailable(googletest)

FetchContent_Declare(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.x
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.x
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  TGUI
  GIT_REPOSITORY https://github.com/texus/TGUI
  GIT_TAG v1.10.0   # Change this to wanted version
)
FetchContent_MakeAvailable(TGUI)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
# file(GLOB TEST_SOURCES "tests/*.cpp")

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
# add_executable(testing ${TEST_SOURCES} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# set_target_properties(testing PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics TGUI::TGUI spdlog::spdlog)
# target_link_libraries(testing GTest::gtest_main)

execute_process(
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/update_tag.py
  RESULT_VARIABLE exit_code
)
execute_process(
  COMMAND git describe --abbrev=0 --tags
  OUTPUT_VARIABLE tag
)

string(LENGTH ${tag} tag_len)
math(EXPR new_tag_len "${tag_len} - 1")
string(SUBSTRING "${tag}" 0 ${new_tag_len} clear_tag)
string(REPLACE "." ";" list_tag "${clear_tag}")
list(GET list_tag 0 MAJOR_VERSION)
list(GET list_tag 1 MINOR_VERSION)
list(GET list_tag 2 PATCH_VERSION)

set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_CONTACT elochkaan@gmail.com)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
    /W4
  )
  # target_compile_options(testing PRIVATE
  #   /W4
  # )
else ()
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -pedantic
  )
  # target_compile_options(testing PRIVATE
  #   -Wall -Wextra -pedantic
  # )
endif()

if (MSVC)
    set(CPACK_GENERATOR ZIP)
else ()
    set(CPACK_GENERATOR DEB)
endif()

# enable_testing()
# include(GoogleTest)
include(CPack)
# gtest_discover_tests(testing)cmake_minimum_required(VERSION 3.28)