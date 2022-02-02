cmake_minimum_required (VERSION 3.5)
project(NaiveLinalg-tests VERSION 0.2.0 LANGUAGES C CXX)


# add the executable
add_executable(naivelinalg_test "test.c")
target_link_libraries(naivelinalg_test naivelinalg)

add_executable(naivelinalg_testcpp "testcpp.cpp")
target_link_libraries(naivelinalg_testcpp naivelinalg)

add_test(testinc naivelinalg_test)
add_test(testincpp naivelinalg_testcpp)


#Now with Google Test
#enable_testing()
#find_package(GTest REQUIRED)

#add_executable(foo foo.cc)
#target_link_libraries(foo GTest::gtest GTest::gtest_main)
#add_test(AllTestsInFoo foo)
