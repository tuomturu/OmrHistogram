cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME OkrHistogram)

project(${PROJECT_NAME})

#####################################
######## COMPILER SETTINGS ##########
#####################################

#set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} /usr/local/lib)
#set(CMAKE_CXX_COMPILER "g++")
#set(CMAKE_C_COMPILER "gcc")

# Error messages single line for Eclipse
if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCC)
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -g")

#####################################
######## EXTERNAL PACKAGES ##########
#####################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/modules/")

# find packets
find_package(Qt4 REQUIRED)
find_package(Qwt REQUIRED)

include(${QT_USE_FILE})

# add includes
set(EXTERNAL_INCLUDE_DIRS
	${Qt4_INCLUDE_DIR}
	${QWT_INCLUDE_DIR}
	)

# add libraries
set(EXTERNAL_LIBS
	${QT_LIBRARIES}
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
	)

#####################################
############## QT STUFF #############
#####################################

#QT4_WRAP_UI(UI_SOURCES ${CMAKE_SOURCE_DIR}/gui/GUI.ui)
#QT4_WRAP_CPP(MOC_SOURCES ${CMAKE_SOURCE_DIR}/inc/DataValidator.h)

#####################################
############# INCLUDES ##############
#####################################


set(INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/../inc
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
############## COMPILE ##############
#####################################

add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
