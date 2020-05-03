cmake_minimum_required(VERSION 3.8)

project(Lab4)

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
	"src/Lab4.cpp"
	"src/CannyDetector.cpp"
	"src/StreetDetector.cpp" 
	"src/HoughTransform.cpp"
)


target_link_libraries(
	${PROJECT_NAME}
	${OpenCV_LIBS}
)

#Copy data needed by program to the build directory
file(COPY Images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "Build")
#set(EXECUTABLE_OUTPUT_PATH "build")