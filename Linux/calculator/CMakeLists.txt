cmake_minimum_required(VERSION 3.10)
project(Calculator VERSION 1.0.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加库
add_library(calculator_lib src/calculator.cpp)
target_include_directories(calculator_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加主程序
add_executable(calculator_app src/main.cpp)
target_link_libraries(calculator_app PRIVATE calculator_lib)

# 添加测试程序
add_executable(calculator_test tests/test_calculator.cpp)
target_link_libraries(calculator_test PRIVATE calculator_lib)

# 安装规则
install(TARGETS calculator_app calculator_lib
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include/calculator
)

install(FILES README.md DESTINATION share/calculator/doc)

# 包含 CPack 模块
include(CPack)

# 设置 CPack 参数
set(CPACK_GENERATOR "TGZ;ZIP")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")