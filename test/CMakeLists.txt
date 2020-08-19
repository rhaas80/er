###############
# Build tests
###############
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

ADD_EXECUTABLE(test_config test_config.c)

TARGET_LINK_LIBRARIES(test_config er)

################
# Add tests to ctest
################

ADD_TEST(NAME test_config COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 1 ./test_config)

####################
# make a verbose "test" target named "check"
####################

ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
