cmake_minimum_required(VERSION 3.25)
project(dx12-triangle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    common.cxx
    window.cxx
    main.cxx
)

set(HEADERS
    common.hxx
    window.hxx
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link against the DirectX 12 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE d3d12.lib dxgi.lib)