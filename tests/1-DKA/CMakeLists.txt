set(TARGET_NAME "1-DKA")
set(COVERAGE_NAME "1-coverage")
set(COVERAGE_TARGET "1-coverage-DKA")
set(SOURCE_DIR_MAIN "${1-DKA_SOURCE_DIR}")
set(SOURCES ${0-source-files} ${1-source-files})

message(INFO ${SOURCES})

set(TEST_SOURCES TestDKA.h TestDKA.cpp)
run_coverage(
    "${TEST_SOURCES}"
    "${SOURCES}"
    "${COVERAGE_TARGET}"
    "${TARGET_NAME}"
    "${COVERAGE_NAME}"
)
