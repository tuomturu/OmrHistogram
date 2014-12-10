cmake_minimum_required(VERSION 2.8.11)

set(PROJECT_NAME OkrHistogram)
project(${PROJECT_NAME})

include(${PROJECT_SOURCE_DIR}/../common.cmake)

#####################################
######## EXTERNAL PACKAGES ##########
#####################################


#####################################
########### SOURCE FILES ############
#####################################

set(SOURCE_DIR
	${CMAKE_SOURCE_DIR}
	)
	
set(SOURCES
	${SOURCE_DIR}/CommandLineParser.cpp
	${SOURCE_DIR}/DataParser.cpp
	${SOURCE_DIR}/Histogram.cpp
	${SOURCE_DIR}/Main.cpp
	)

#####################################
############# INCLUDES ##############
#####################################

set(INCLUDE_DIRS
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

set(CMAKE_AUTOMOC ON)

set(UI_DIR
    ${CMAKE_SOURCE_DIR}/../ui
	)

QT5_WRAP_UI(UI_HEADERS
    ${UI_DIR}/Histogram.ui
    )

#####################################
############## COMPILE ##############
#####################################

add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

qt5_use_modules(${PROJECT_NAME} Widgets)
