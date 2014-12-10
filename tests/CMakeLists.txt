cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/modules/")
include(${PROJECT_SOURCE_DIR}/../common.cmake)

enable_testing()

#####################################
######## EXTERNAL PACKAGES ##########
#####################################

find_package(Qt5Test REQUIRED)

#####################################
############# TEST CASES ############
#####################################

set(TESTS
    TestDataParser
    )

#####################################
############## COMPILE ##############
#####################################

foreach(TEST_CASE ${TESTS})

    include(${PROJECT_SOURCE_DIR}/${TEST_CASE}/testcase.cmake)

    add_executable(${TEST_CASE} ${SOURCES} ${MOC_SOURCES} ${UI_HEADERS})
    add_test(${TEST_CASE} ${TEST_CASE})
    
    target_link_libraries(${TEST_CASE} ${LINK_LIBS})
    qt5_use_modules(${TEST_CASE} Test)

endforeach()
