project(1-DKA)

set(1-source-files "${PROJECT_SOURCE_DIR}/DKA.cpp" CACHE INTERNAL "")

add_library(
    1-DKA SHARED ${1-source-files} ${Automaton_SOURCE_DIR}/include/DKA.h
)
