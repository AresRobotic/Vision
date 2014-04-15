cmake_minimum_required(VERSION 2.8) 
set( NAME_PROJECT
color_tracking)
PROJECT (${NAME_PROJECT})
find_package(OpenCV REQUIRED )

set( NAME_SRC
    	${NAME_PROJECT}.cpp
	../myfunctions.cpp
        
)

set( NAME_HEADERS       
	../myfunctions.hpp
	
)

add_executable( ${NAME_PROJECT} ${NAME_SRC} ${NAME_HEADERS} )

target_link_libraries( ${NAME_PROJECT} ${OpenCV_LIBS} )
