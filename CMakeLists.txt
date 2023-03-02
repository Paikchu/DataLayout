cmake_minimum_required(VERSION 3.24)
project(A1)

set(CMAKE_CXX_STANDARD 17)

set(lib library.cpp library.h)

find_package(OpenMP REQUIRED)

add_executable(csv2heapfile csv2heapfile.cpp ${lib})
add_executable(scan scan.cpp ${lib})
add_executable(insert insert.cpp ${lib})
add_executable(update update.cpp ${lib})
add_executable(delete delete.cpp ${lib})
add_executable(read_fixed_len_page read_fixed_len_page.cpp ${lib})
add_executable(write_fixed_len_pages write_fixed_len_pages.cpp ${lib})
add_executable(select select.cpp ${lib})

add_executable(csv2colstore csv2colstore.cpp ${lib})
target_link_libraries(csv2colstore PUBLIC OpenMP::OpenMP_CXX)

add_executable(select2 select2.cpp ${lib})
add_executable(select3 select3.cpp ${lib})

# set(EXECUTABLE_OUTPUT_PATH exeFile)
