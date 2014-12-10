
#####################################
########### SOURCE FILES ############
#####################################

set(SOURCE_DIR
	${CMAKE_SOURCE_DIR}/../src
	)
	
set(SOURCES
    ${SOURCE_DIR}/DataParser.cpp
	${SOURCE_DIR}/DataParser_test.cpp
	)

#####################################
############# INCLUDES ##############
#####################################

set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/../src
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
    )
