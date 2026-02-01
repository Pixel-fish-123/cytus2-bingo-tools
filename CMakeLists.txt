cmake_minimum_required(VERSION 3.15)
project(ExcelProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================================================
# 1. 自动获取 OpenXLSX (替代原来的 add_subdirectory)
# =========================================================
include(FetchContent)
FetchContent_Declare(
  OpenXLSX
  GIT_REPOSITORY https://github.com/troldal/OpenXLSX.git
  GIT_TAG      master # 或者指定版本号如 v0.3.0
)
FetchContent_MakeAvailable(OpenXLSX)

# 2. 生成可执行文件
add_executable(working main.cpp function_realization.cpp)

# 3. 链接库 (依然维持原样)
target_link_libraries(working PRIVATE OpenXLSX::OpenXLSX)

# =========================================================
# 4. 【关键】静态链接魔法 (针对 Windows)
# =========================================================
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    target_compile_definitions(working PRIVATE OPENXLSX_STATIC_DEFINE)
endif()

# 5. 自动拷贝 Excel 文件
add_custom_command(TARGET working POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/song_package.xlsx"
        $<TARGET_FILE_DIR:working>
)
