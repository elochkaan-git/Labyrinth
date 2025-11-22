cmake_minimum_required(VERSION 3.30)

project(labyrinth)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

message(STATUS "Searching libraries")
find_package(SFML COMPONENTS graphics)
find_package(spdlog)
find_package(Boost CONFIG COMPONENTS program_options)
include(FetchContent)

if (SFML_FOUND)
  message(STATUS "SFML found!")
else ()
  message(STATUS "SFML not found! Downloading")
  FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
  FetchContent_MakeAvailable(SFML)
endif()

if (spdlog_FOUND)
  message(STATUS "spdlog found!")
else ()
  message(STATUS "spdlog not found! Downloading")
  FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.x
  )
  FetchContent_MakeAvailable(spdlog)
endif()

if (Boost_FOUND)
  message(STATUS "Boost.Program_options found!")
endif()

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)

if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
    /W4
  )
else ()
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -pedantic
  )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
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
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_PACKAGE_CONTACT elochkaan@gmail.com)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(MSVC)
  set(CPACK_GENERATOR ZIP)
else ()
  set(CPACK_GENERATOR TGZ)
endif()
include(CPack)

