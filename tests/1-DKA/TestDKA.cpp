#include "TestDKA.h"

void TestDKAWithLanguage::SetUp() {
    NKA_machine = NKA('a')*( KleeneStar(NKA("abc")+NKA("cde")) + KleenePlus(NKA('f'))*NKA('f') );
    NKA_machine.delete_epsilon_transitions();
}

void TestDKAWithLanguage::TearDown() {}

void TestDKAWithLanguage::CheckCorrectnessLanguage(DKA& machine) {
    EXPECT_FALSE(machine.is_word_in_language(""));
    EXPECT_TRUE(machine.is_word_in_language("a"));
    EXPECT_FALSE(machine.is_word_in_language("af"));
    EXPECT_TRUE(machine.is_word_in_language("aff"));
    EXPECT_TRUE(machine.is_word_in_language("afffff"));

    EXPECT_FALSE(machine.is_word_in_language("aaa"));
    EXPECT_TRUE(machine.is_word_in_language("aabc"));
    EXPECT_TRUE(machine.is_word_in_language("acde"));
    EXPECT_TRUE(machine.is_word_in_language("aabcabcabc"));
    EXPECT_TRUE(machine.is_word_in_language("acdecde"));
    EXPECT_TRUE(machine.is_word_in_language("acdeabcabcabccdecde"));

    EXPECT_FALSE(machine.is_word_in_language("aabcabccccabc"));
    EXPECT_FALSE(machine.is_word_in_language("aabcfffabc"));
    EXPECT_FALSE(machine.is_word_in_language("acdefffcde"));
}

void TestDKAWithLanguage::CheckCorrectnessLanguageComplement(DKA& machine) {
    EXPECT_TRUE(machine.is_word_in_language(""));
    EXPECT_FALSE(machine.is_word_in_language("a"));
    EXPECT_TRUE(machine.is_word_in_language("af"));
    EXPECT_FALSE(machine.is_word_in_language("aff"));
    EXPECT_FALSE(machine.is_word_in_language("afffff"));

    EXPECT_TRUE(machine.is_word_in_language("aaa"));
    EXPECT_FALSE(machine.is_word_in_language("aabc"));
    EXPECT_FALSE(machine.is_word_in_language("acde"));
    EXPECT_FALSE(machine.is_word_in_language("aabcabcabc"));
    EXPECT_FALSE(machine.is_word_in_language("acdecde"));
    EXPECT_FALSE(machine.is_word_in_language("acdeabcabcabccdecde"));

    EXPECT_TRUE(machine.is_word_in_language("aabcabccccabc"));
    EXPECT_TRUE(machine.is_word_in_language("aabcfffabc"));
    EXPECT_TRUE(machine.is_word_in_language("acdefffcde"));
}

TEST_F(TestDKA, Test1TestOfTest) {
    EXPECT_TRUE(true);
}

TEST_F(TestDKAWithLanguage, Test2MakeDKAAndCheckWords) {
    DKA machine("abcdef", NKA_machine);
    CheckCorrectnessLanguage(machine);
}

TEST_F(TestDKAWithLanguage, Test3MakeComplementOfDKA) {
    DKA machine("abcdef", NKA_machine);
    machine.make_complement();
    CheckCorrectnessLanguageComplement(machine);
}

void TestDKAMakeMinimalBySteps::SetUp() {
    //Example from lecture
    machine.alphabet = "ab";
    machine.reverse_alphabet = std::vector<int>(128, 0);
    for (size_t i = 0; i < machine.alphabet.size(); ++i) {
        machine.reverse_alphabet[machine.alphabet[i]] = i;
    }

    for (int i = 0; i < 4; ++i) {
        machine.all_vertex.push_back(D_vertex());
    }
    machine.all_vertex[0].outgoing_edges.push_back(3);
    machine.all_vertex[0].outgoing_edges.push_back(1);
    machine.all_vertex[1].outgoing_edges.push_back(1);
    machine.all_vertex[1].outgoing_edges.push_back(2);
    machine.all_vertex[2].outgoing_edges.push_back(3);
    machine.all_vertex[2].outgoing_edges.push_back(1);
    machine.all_vertex[3].outgoing_edges.push_back(3);
    machine.all_vertex[3].outgoing_edges.push_back(3);
    machine.all_vertex[3].is_terminal = true;
}

void TestDKAMakeMinimalBySteps::TearDown() {}

void TestDKAMakeMinimalBySteps::TestConstructWorker(const DKA_make_minimal& worker) {
    //Check equivalence classes
    std::vector<size_t> variant1 = {0, 0, 0, 1};
    std::vector<size_t> variant2 = {1, 1, 1, 0};
    //can be one of this variants
    //but any of this variants is correct
    EXPECT_TRUE(worker.current_EC == variant1 || worker.current_EC == variant2);
    EXPECT_EQ(worker.current_number_of_different_EC, 2);
}

void TestDKAMakeMinimalBySteps::TestFindCorrectFinalCurrentEC(const DKA_make_minimal& worker) {
    //Check equivalence classes
    std::vector<size_t> variant1 = {0, 1, 0, 2};
    std::vector<size_t> variant2 = {0, 2, 0, 1};
    std::vector<size_t> variant3 = {1, 0, 1, 2};
    std::vector<size_t> variant4 = {1, 2, 1, 0};
    std::vector<size_t> variant5 = {2, 0, 2, 1};
    std::vector<size_t> variant6 = {2, 1, 2, 0};
    //can be one of this variants
    //but any of this variants is correct
    EXPECT_TRUE(worker.current_EC == variant1
            || worker.current_EC == variant2
            || worker.current_EC == variant3
            || worker.current_EC == variant4
            || worker.current_EC == variant5
            || worker.current_EC == variant6);
    EXPECT_EQ(worker.current_number_of_different_EC, 3);
}

void TestDKAMakeMinimalBySteps::CheckCorrectnessLanguage(const DKA_make_minimal& worker) {
    EXPECT_EQ(machine.all_vertex.size(), 3);
    EXPECT_TRUE(machine.is_word_in_language("bbaaaaabbbba"));
    EXPECT_FALSE(machine.is_word_in_language("bbb"));
}

TEST_F(TestDKAMakeMinimalBySteps, Test4MakeMinimalBySteps_Part1TestConstructWorker) {
    DKA_make_minimal worker(machine);
    TestConstructWorker(worker);
}

TEST_F(TestDKAMakeMinimalBySteps, Test4MakeMinimalBySteps_Part2TestFindCorrectFinalCurrentEC) {
    DKA_make_minimal worker(machine);
    worker.find_correct_final_current_EC();
    TestFindCorrectFinalCurrentEC(worker);
}

TEST_F(TestDKAMakeMinimalBySteps, Test4MakeMinimalBySteps_Part3CheckCorrectnessLanguage) {
    DKA_make_minimal worker(machine);
    worker.find_correct_final_current_EC();
    worker.create_new_DKA_which_is_MPDKA();
    CheckCorrectnessLanguage(worker);
}

TEST_F(TestDKAWithLanguage, Test5MakeMinimal) {
    DKA machine("abcdef", NKA_machine);
    CheckCorrectnessLanguage(machine);
    size_t size1 = machine.all_vertex.size();

    machine.make_minimal();
    CheckCorrectnessLanguage(machine);
    size_t size2 = machine.all_vertex.size();

    machine.make_minimal();
    CheckCorrectnessLanguage(machine);
    size_t size3 = machine.all_vertex.size();

    EXPECT_LE(size2, size1);
    EXPECT_EQ(size3, size2);
}

TEST_F(TestDKA, Test6StringWithMarker) {
    string_with_marker str_m("abc", '*');
    EXPECT_EQ(str_m.str, "abc");
    EXPECT_EQ(str_m.c, '*');

    string_with_marker a("a", 'n');
    string_with_marker b("b+cd", '+');
    string_with_marker c("(e+f)g", '*');
    string_with_marker d("(h+kl)*", 's');
    string_with_marker e("", 'n');
    string_with_marker f("_", 'n');

    str_m = RE_add(a, b);
    EXPECT_EQ(str_m.str, "a+b+cd");
    EXPECT_EQ(str_m.c, '+');

    str_m = RE_add(c, d);
    EXPECT_EQ(str_m.str, "(e+f)g+(h+kl)*");
    EXPECT_EQ(str_m.c, '+');

    str_m = RE_add(a, e);
    EXPECT_EQ(str_m.str, a.str);
    EXPECT_EQ(str_m.c, a.c);
    str_m = RE_add(e, c);
    EXPECT_EQ(str_m.str, c.str);
    EXPECT_EQ(str_m.c, c.c);

    str_m = RE_mul(c, d);
    EXPECT_EQ(str_m.str, "(e+f)g(h+kl)*");
    EXPECT_EQ(str_m.c, '*');

    str_m = RE_mul(b, c);
    EXPECT_EQ(str_m.str, "(b+cd)(e+f)g");
    EXPECT_EQ(str_m.c, '*');

    str_m = RE_mul(c, b);
    EXPECT_EQ(str_m.str, "(e+f)g(b+cd)");
    EXPECT_EQ(str_m.c, '*');

    str_m = RE_mul(b, e);
    EXPECT_EQ(str_m.str, e.str);
    EXPECT_EQ(str_m.c, e.c);

    str_m = RE_mul(b, f);
    EXPECT_EQ(str_m.str, b.str);
    EXPECT_EQ(str_m.c, b.c);
    str_m = RE_mul(f, b);
    EXPECT_EQ(str_m.str, b.str);
    EXPECT_EQ(str_m.c, b.c);

    str_m = RE_star(e);
    EXPECT_EQ(str_m.str, f.str);
    EXPECT_EQ(str_m.c, f.c);

    str_m = RE_star(f);
    EXPECT_EQ(str_m.str, f.str);
    EXPECT_EQ(str_m.c, f.c);

    str_m = RE_star(a);
    EXPECT_EQ(str_m.str, "a*");
    EXPECT_EQ(str_m.c, 's');

    str_m = RE_star(c);
    EXPECT_EQ(str_m.str, "((e+f)g)*");
    EXPECT_EQ(str_m.c, 's');
}

TEST_F(TestDKA, Test7AllRVertex) {
    All_R_Vertex V(2);
    EXPECT_EQ(V.all_vertex.size(), 3);

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_TRUE(V.all_vertex[i].outgoing_edges[j].empty());
            EXPECT_EQ(V.all_vertex[i].outgoing_edges_last_operation[j], 'n');
        }
    }

    V.all_vertex[1].outgoing_edges[0] = "abc";
    V.all_vertex[1].outgoing_edges_last_operation[0] = '*';
    EXPECT_EQ(V.edge(1, 0), "abc");
    EXPECT_EQ(V.last_operation(1, 0), '*');
    V.edge(1, 0) = "a+b+c";
    V.last_operation(1, 0) = '+';
    EXPECT_EQ(V.edge(1, 0), "a+b+c");
    EXPECT_EQ(V.last_operation(1, 0), '+');
    EXPECT_EQ(V.size(), 3);
}
