# 查找 GTest 库
find_package(GTest REQUIRED)
# GTest 的头文件
include_directories(${GTEST_INCLUDE_DIRS})
# GadgetNet 的头文件
include_directories(${PROJECT_SOURCE_DIR}/inc)

aux_source_directory(${PROJECT_SOURCE_DIR}/src IMP_SRC_LIST)
aux_source_directory(. TEST_SRC_LIST)

foreach(test_src ${TEST_SRC_LIST})
    string(REGEX MATCHALL "[0-9A-Za-z_]*.cpp" test_src_file_name ${test_src})
    string(REGEX REPLACE ".cpp" "" test_src_name ${test_src_file_name})
    message(${test_src_name})
    add_executable(${test_src_name} ${test_src} ${IMP_SRC_LIST})   
    # 链接测试库
    target_link_libraries(${test_src_name} ${GTEST_BOTH_LIBRARIES} pthread)
    # 添加到测试
    add_test(NAME ${test_src_name} COMMAND ${test_src_name})
endforeach()
