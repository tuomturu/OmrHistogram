cmake_minimum_required(VERSION 2.8.11)
set(PROJECT_NAME OkrHistogram)

project(${PROJECT_NAME})

#####################################
######## COMPILER SETTINGS ##########
#####################################

# Mocced stuff is in build directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Error messages in single line for Eclipse
if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCC)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -g")

#####################################
######## EXTERNAL PACKAGES ##########
#####################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/modules/")

# find packages
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(QT_INCLUDE_DIR /usr/local/qwt-6.1.2-svn-qt-5.3.2/include)
set(QT_LIBRARY_DIR /usr/local/qwt-6.1.2-svn-qt-5.3.2/lib)
find_package(Qwt REQUIRED)

# add includes
set(EXTERNAL_INCLUDE_DIRS
	${QWT_INCLUDE_DIR}
	)

# add libraries
set(EXTERNAL_LIBS
	${QWT_LIBRARIES}
	)

#####################################
########### SOURCE FILES ############
#####################################

set(SOURCE_DIR
	${CMAKE_SOURCE_DIR}
	)
	
set(SOURCES
	${SOURCE_DIR}/Main.cpp
	${SOURCE_DIR}/CommandLineParser.cpp
	${SOURCE_DIR}/Histogram.cpp
	)

#####################################
############# INCLUDES ##############
#####################################

set(INCLUDE_DIR
	${CMAKE_SOURCE_DIR}/../inc
	)

set(INCLUDE_DIRS
    ${INCLUDE_DIR}
	)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${INCLUDE_DIRS}
	${EXTERNAL_INCLUDE_DIRS}
	)

#####################################
############### LIBS ################
#####################################

set(LINK_LIBS
    ${EXTERNAL_LIBS}
    )

#####################################
############## QT STUFF #############
#####################################

set(UI_DIR
    ${CMAKE_SOURCE_DIR}/../ui
	)

#set(CMAKE_AUTOMOC ON)

QT5_WRAP_UI(UI_HEADERS
    ${UI_DIR}/Histogram.ui
    )
    
QT5_WRAP_CPP(MOC_SOURCES
    ${INCLUDE_DIR}/CommandLineParser.hpp
    ${INCLUDE_DIR}/Histogram.hpp
    )

#####################################
############## COMPILE ##############
#####################################

add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

qt5_use_modules(${PROJECT_NAME} Widgets)