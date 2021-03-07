cmake_minimum_required(VERSION 3.14)
project(Game)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE "Release")
include_directories("${PROJECT_SOURCE_DIR}/Library/include")
include_directories("${PROJECT_SOURCE_DIR}/Thirdparty/Bullet")
link_directories(${PROJECT_SOURCE_DIR}/Library)
add_subdirectory(App)
add_subdirectory(Component)
add_subdirectory(GameObject)
add_subdirectory(Library)

set(MATH_SRC
        Math/Vec.h
        Math/Matrix.h Math/DXMath.h)
set(BASE_SRC
        BaseApp.h
        WindowsTimer.h
        WindowsTimer.cpp ModelImporter.cpp ModelImporter.h TestAnimManager.h)


set(EXECUTABLE_OUTPUT_PATH e:/project/${PROJECT_NAME})

add_executable(Game WIN32 main.cpp ${MATH_SRC} ${BASE_SRC} TestApp.h)

target_link_libraries(Game Library D3DApp Component GameObject dxgi dxguid d3d11 d3dcompiler)