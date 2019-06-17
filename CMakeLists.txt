#1. cmake version 
cmake_minimum_required(VERSION 3.10.2)

#2. project name
PROJECT(tinyServer)

#3. header file path
INCLUDE_DIRECTORIES(

)

#4. source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

#5. 设置环境变量
SET(TEST_MATH
    ${DIR_SRCS}
 )
 
#6.add executable file，添加要编译的可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH})

#7.add link library，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)