cmake_minimum_required(VERSION 3.8)

project(Lab2)

if(WIN32)
	set(OpenCV_dir $ENV{OPENCV_DIR})
endif ()

if(MSVC)
	add_compile_options(/W4 /permissive-)
else ()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(OpenCV REQUIRED)


include_directories(
	include
	${OpenCV_INCLUDE_DIRS}
)

add_executable(
	${PROJECT_NAME}
	"src/Lab2.cpp"
	"src/Calibration.cpp")


target_link_libraries(
	${PROJECT_NAME}
	${OpenCV_LIBS}
)

#Copy data needed by program to the build directory
if (WIN32 AND MSVC)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}" NATIVE_BIN_DIR)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data" NATIVE_DATA_DIR)
  add_custom_command(
    TARGET "${PROJECT_NAME}" POST_BUILD
    COMMAND xcopy "${NATIVE_DATA_DIR}" "${NATIVE_BIN_DIR}\\data" /I /C /K /E /R /Y 
  )
endif (WIN32 AND MSVC)
