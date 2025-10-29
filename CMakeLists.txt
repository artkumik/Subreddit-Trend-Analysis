cmake_minimum_required(VERSION 3.30)
project(Subreddit_Trend_Analysis)

set(CMAKE_CXX_STANDARD 14)

add_executable(Subreddit_Trend_Analysis
        Heap.cpp
        Heap.h
        main.cpp
)

# Copy the data folder into the build/run directory
add_custom_command(TARGET Subreddit_Trend_Analysis PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/processed_data
        $<TARGET_FILE_DIR:Subreddit_Trend_Analysis>/processed_data
)