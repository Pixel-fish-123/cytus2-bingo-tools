cmake_minimum_required(VERSION 3.15)
project(ExcelProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 包含本地的 OpenXLSX (确保你已经按上一个教程把源码放好了)
add_subdirectory(third_party/OpenXLSX)

# 2. 生成可执行文件
add_executable(working main.cpp function_realization.cpp)

# 3. 链接库
target_link_libraries(working PRIVATE OpenXLSX::OpenXLSX)

# =========================================================
# 4. 【关键】静态链接魔法 (针对 Windows)
# =========================================================
if(WIN32)
    # -static: 强制所有系统库静态链接
    # -static-libgcc -static-libstdc++: 防止缺少 C++ 运行库
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

    # 某些版本的 OpenXLSX 需要手动指定不需要 DLL
    target_compile_definitions(working PRIVATE OPENXLSX_STATIC_DEFINE)
endif()

# 5. 自动拷贝 Excel 文件到构建目录 (方便你自己调试)
add_custom_command(TARGET working POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/song_package.xlsx"
        $<TARGET_FILE_DIR:working>
)