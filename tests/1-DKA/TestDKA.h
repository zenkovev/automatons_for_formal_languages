#pragma once
#include <gtest/gtest.h>
#include "NKA.h"
#include "DKA.h"

class TestDKA: public ::testing::Test{

};

class TestDKAWithLanguage : public ::testing::Test {
public:
    NKA NKA_machine;

    void SetUp();
    void TearDown();
    void CheckCorrectnessLanguage(DKA& machine);
    void CheckCorrectnessLanguageComplement(DKA& machine);
};

class TestDKAMakeMinimalBySteps : public ::testing::Test {
public:
    DKA machine;

    void SetUp();
    void TearDown();
    void TestConstructWorker(const DKA_make_minimal& worker);
    void TestFindCorrectFinalCurrentEC(const DKA_make_minimal& worker);
    void CheckCorrectnessLanguage(const DKA_make_minimal& worker);
};
