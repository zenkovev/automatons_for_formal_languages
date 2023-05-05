project(0-NKA)


set(0-source-files "${PROJECT_SOURCE_DIR}/NKA.cpp" CACHE INTERNAL "")
 
add_library(
    0-NKA SHARED ${0-source-files} ${Automaton_SOURCE_DIR}/include/NKA.h
)
