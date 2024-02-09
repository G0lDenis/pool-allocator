set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(DefaultNAlloc default_n_allocation.cpp)
add_executable(MyNAlloc my_alloc_n_allocation.cpp)

target_link_libraries(DefaultNAlloc pool_allocator)
target_link_libraries(MyNAlloc pool_allocator)
target_include_directories(DefaultNAlloc PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(MyNAlloc PUBLIC ${PROJECT_SOURCE_DIR})