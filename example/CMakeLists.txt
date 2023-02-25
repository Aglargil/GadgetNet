# GadgetNet 的头文件
include_directories(${PROJECT_SOURCE_DIR}/inc)

aux_source_directory(${PROJECT_SOURCE_DIR}/src IMP_SRC_LIST)
aux_source_directory(. SRC_LIST)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/TcpServer TCPSERVER_SRC_LIST)

foreach(src ${SRC_LIST})
    string(REGEX MATCHALL "[0-9A-Za-z_]*.cpp" src_file_name ${src})
    string(REGEX REPLACE ".cpp" "" src_name ${src_file_name})
    message(${src_name})
    add_executable(${src_name} ${src} ${IMP_SRC_LIST} ${TCPSERVER_SRC_LIST})   
    target_link_libraries(${src_name} pthread)
endforeach()
