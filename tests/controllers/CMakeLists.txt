########################################################################
# unit test suite
########################################################################


########################################################################
# Add includes and dependencies
########################################################################
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

########################################################################
# Build executables, register as tests, and install
########################################################################
# Build each test and link to libraries
SET(test_sources
	
)

#turn each test cpp file into an executable with an int main() function
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

#for each source: build an executable, register it as a test, and install
FOREACH(test_source ${test_sources})
    GET_FILENAME_COMPONENT(test_name ${test_source} NAME_WE)
    ADD_EXECUTABLE(${test_name} ${test_source})
    ADD_TEST(${test_name} ${test_name})
    INSTALL(TARGETS ${test_name} RUNTIME DESTINATION ${DATA_DIR}/tests/controllers)
ENDFOREACH(test_source)

