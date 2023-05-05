set(TARGET_NAME "0-NKA")
set(COVERAGE_NAME "0-coverage")
set(COVERAGE_TARGET "0-coverage-NKA")
set(SOURCE_DIR_MAIN "${0-NKA_SOURCE_DIR}")
set(SOURCES ${0-source-files})

message(INFO ${SOURCES})

set(TEST_SOURCES TestNKA.h TestNKA.cpp)
run_coverage(
    "${TEST_SOURCES}"
    "${SOURCES}"
    "${COVERAGE_TARGET}"
    "${TARGET_NAME}"
    "${COVERAGE_NAME}"
)
