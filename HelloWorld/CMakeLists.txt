cmake_minimum_required(VERSION 3.8)

project(HelloWorld)

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
	"src/HelloWorld.cpp"
	"src/Test.cpp"
)


target_link_libraries(
	${PROJECT_NAME}
	${OpenCV_LIBS}
)

#Copy data needed by program to the build directory
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

