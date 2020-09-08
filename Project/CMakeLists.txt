cmake_minimum_required(VERSION 3.8)

project(Project)

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
	"src/Project.cpp"
	"src/TemplateMatching.cpp"
	"src/CannyDetector.cpp"
	"src/BestResults.cpp"
	"src/GammaTransform.cpp"
 "include/Menu.hpp" "src/Menu.cpp")

target_link_libraries(
	${PROJECT_NAME}
	${OpenCV_LIBS}
)
