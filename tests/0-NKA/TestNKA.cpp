#include "TestNKA.h"
#include "NKA.h"

TEST_F(TestNKA, Test1TestOfTest) {
    EXPECT_TRUE(true);
}

TEST_F(TestNKA, Test2Edge) {
    edge e(5, 'a');
    EXPECT_EQ(e.to, 5);
    EXPECT_EQ(e.letter, 'a');
}

TEST_F(TestNKA, Test3Vertex) {
    vertex v1(true);
    EXPECT_TRUE(v1.outgoing_edges.empty());
    EXPECT_EQ(v1.is_terminal, true);

    vertex v2(false, 5, 'a');
    EXPECT_EQ(v2.outgoing_edges.size(), 1);
    EXPECT_EQ(v2.outgoing_edges[0].to, 5);
    EXPECT_EQ(v2.outgoing_edges[0].letter, 'a');
    EXPECT_EQ(v2.is_terminal, false);
}

TEST_F(TestNKA, Test4VertexPushEdge) {
    vertex v1(false);
    v1.push_edge_quick(5, 'a');
    v1.push_edge_quick(5, 'a');

    EXPECT_EQ(v1.outgoing_edges.size(), 2);
    EXPECT_EQ(v1.outgoing_edges[0].to, 5);
    EXPECT_EQ(v1.outgoing_edges[0].letter, 'a');
    EXPECT_EQ(v1.outgoing_edges[1].to, 5);
    EXPECT_EQ(v1.outgoing_edges[1].letter, 'a');

    vertex v2(false);
    v2.push_edge(5, 'a');
    v2.push_edge(5, 'a');

    EXPECT_EQ(v2.outgoing_edges.size(), 1);
    EXPECT_EQ(v2.outgoing_edges[0].to, 5);
    EXPECT_EQ(v2.outgoing_edges[0].letter, 'a');
}

TEST_F(TestNKA, Test5IsWordInLanguage) {
    NKA machine1("abcde");
    EXPECT_TRUE(machine1.is_word_in_language("abcde"));
    EXPECT_FALSE(machine1.is_word_in_language(""));
    EXPECT_FALSE(machine1.is_word_in_language("acbde"));
    EXPECT_FALSE(machine1.is_word_in_language("abcd"));
    EXPECT_FALSE(machine1.is_word_in_language("bcde"));

    NKA machine2 = KleeneStar(NKA(0)+NKA("abc"));
    EXPECT_TRUE(machine2.is_word_in_language(""));
    EXPECT_TRUE(machine2.is_word_in_language("abc"));
    EXPECT_TRUE(machine2.is_word_in_language("abcabc"));
    EXPECT_TRUE(machine2.is_word_in_language("abcabcabc"));
    EXPECT_FALSE(machine2.is_word_in_language("abcde"));
}

TEST_F(TestNKA, Test6SimpleNKAConstructors) {
    NKA machine1;
    EXPECT_EQ(machine1.all_vertex.size(), 1);
    EXPECT_FALSE(machine1.all_vertex[0].is_terminal);

    NKA machine2(0);
    EXPECT_TRUE(machine2.is_word_in_language(""));
    EXPECT_FALSE(machine2.is_word_in_language("a"));
    EXPECT_FALSE(machine2.is_word_in_language("aaaaaaaaaa"));

    NKA machine3('a');
    EXPECT_FALSE(machine3.is_word_in_language(""));
    EXPECT_TRUE(machine3.is_word_in_language("a"));
    EXPECT_FALSE(machine3.is_word_in_language("b"));
    EXPECT_FALSE(machine3.is_word_in_language("aa"));
    EXPECT_FALSE(machine3.is_word_in_language("aaaaaaaaaa"));

    EXPECT_ANY_THROW(NKA machine4(""));

    NKA machine5("abcde");
    EXPECT_FALSE(machine5.is_word_in_language(""));
    EXPECT_FALSE(machine5.is_word_in_language("a"));
    EXPECT_FALSE(machine5.is_word_in_language("abc"));
    EXPECT_FALSE(machine5.is_word_in_language("bcde"));
    EXPECT_TRUE(machine5.is_word_in_language("abcde"));
}

TEST_F(TestNKA, Test7PushOtherNKAInEndWithoutLinks) {
    NKA machine1("abcde");
    NKA machine2("abcf");
    size_t m1size = machine1.all_vertex.size();
    size_t m2size = machine2.all_vertex.size();

    machine1.push_other_NKA_in_end_without_links(machine2);

    EXPECT_EQ(machine1.all_vertex.size(), m1size+m2size);
    EXPECT_EQ(machine2.all_vertex.size(), m2size);

    EXPECT_TRUE(machine1.is_word_in_language("abcde"));
    EXPECT_FALSE(machine1.is_word_in_language("abcf"));
    EXPECT_FALSE(machine1.is_word_in_language("abcdeabcf"));
}

TEST_F(TestNKA, Test8SumOfNKA) {
    NKA machine1("abcde");
    NKA machine2("abcf");
    NKA machine = machine1 + machine2;

    EXPECT_TRUE(machine.is_word_in_language("abcde"));
    EXPECT_TRUE(machine.is_word_in_language("abcf"));
    EXPECT_FALSE(machine.is_word_in_language("abcdeabcf"));

    EXPECT_FALSE(machine.is_word_in_language(""));

    NKA machine3;
    machine3.all_vertex.clear();
    EXPECT_ANY_THROW(machine1 + machine3);
}

TEST_F(TestNKA, Test9ProductOfNKA) {
    NKA machine1("abcde");
    NKA machine2("abcf");
    NKA machine = machine1 * machine2;

    EXPECT_FALSE(machine.is_word_in_language("abcde"));
    EXPECT_FALSE(machine.is_word_in_language("abcf"));
    EXPECT_TRUE(machine.is_word_in_language("abcdeabcf"));

    EXPECT_FALSE(machine.is_word_in_language(""));

    NKA machine3;
    machine3.all_vertex.clear();
    EXPECT_ANY_THROW(machine1 * machine3);
}

TEST_F(TestNKA, Test10KleeneOperations) {
    NKA machine("abcde");

    NKA machine_star = KleeneStar(machine);
    EXPECT_TRUE(machine_star.is_word_in_language(""));
    EXPECT_TRUE(machine_star.is_word_in_language("abcde"));
    EXPECT_TRUE(machine_star.is_word_in_language("abcdeabcde"));
    EXPECT_TRUE(machine_star.is_word_in_language("abcdeabcdeabcde"));
    EXPECT_FALSE(machine_star.is_word_in_language("abcdeabc"));

    NKA machine_plus = KleenePlus(machine);
    EXPECT_FALSE(machine_plus.is_word_in_language(""));
    EXPECT_TRUE(machine_plus.is_word_in_language("abcde"));
    EXPECT_TRUE(machine_plus.is_word_in_language("abcdeabcde"));
    EXPECT_TRUE(machine_plus.is_word_in_language("abcdeabcdeabcde"));
    EXPECT_FALSE(machine_plus.is_word_in_language("abcdeabc"));

    NKA machine_void;
    machine_void.all_vertex.clear();
    EXPECT_ANY_THROW(KleeneStar(machine_void));
    EXPECT_ANY_THROW(KleenePlus(machine_void));
}

TEST_F(TestNKA, Test11DifferentOperationsCombineTest) {
    NKA machine = NKA('a')*( KleeneStar(NKA("abc")+NKA("cde")) + KleenePlus(NKA('f'))*NKA('f') );

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

//Possibly, test of next function is not very good
//But there are two factors:
//1) Functions-helpers do not have own value without basic function
//2) Functions-helpers linked with each other a lot

//Also I use one example of language for two tests
//But this tests are independent from each other
//And it is not very big copy to think about their union

TEST_F(TestNKA, Test12DeleteEpsilonTransitionsBySteps) {
    NKA machine = KleenePlus(NKA("abc"));
    NKA_delete_epsilon_transitions worker(machine);

    for (size_t i = 0; i < machine.all_vertex.size(); ++i) {
        for (edge e: machine.all_vertex[i].outgoing_edges) {
            if (e.letter == 0) {
                EXPECT_TRUE(worker.epsilon_achievable[i].count(e.to) != 0);
            }
        }
    }

    worker.add_edges_and_terminal_conditions();

    EXPECT_FALSE(machine.is_word_in_language(""));
    EXPECT_TRUE(machine.is_word_in_language("abc"));
    EXPECT_TRUE(machine.is_word_in_language("abcabcabc"));
    EXPECT_FALSE(machine.is_word_in_language("abcabcab"));

    worker.delete_epsilon_edges();

    for (size_t i = 0; i < machine.all_vertex.size(); ++i) {
        for (edge e: machine.all_vertex[i].outgoing_edges) {
            EXPECT_NE(e.letter, 0);
        }
    }

    EXPECT_FALSE(machine.is_word_in_language(""));
    EXPECT_TRUE(machine.is_word_in_language("abc"));
    EXPECT_TRUE(machine.is_word_in_language("abcabcabc"));
    EXPECT_FALSE(machine.is_word_in_language("abcabcab"));

    worker.delete_requrring_edges();

    EXPECT_FALSE(machine.is_word_in_language(""));
    EXPECT_TRUE(machine.is_word_in_language("abc"));
    EXPECT_TRUE(machine.is_word_in_language("abcabcabc"));
    EXPECT_FALSE(machine.is_word_in_language("abcabcab"));

    worker.find_achievable_vertexes_from_start(0);

    EXPECT_TRUE(worker.achievable[0]);

    worker.delete_non_achievable_vertexes_from_start();

    EXPECT_FALSE(machine.is_word_in_language(""));
    EXPECT_TRUE(machine.is_word_in_language("abc"));
    EXPECT_TRUE(machine.is_word_in_language("abcabcabc"));
    EXPECT_FALSE(machine.is_word_in_language("abcabcab"));
}

TEST_F(TestNKA, Test13DeleteEpsilonTransitions) {
    NKA machine = NKA('a')*( KleeneStar(NKA("abc")+NKA("cde")) + KleenePlus(NKA('f'))*NKA('f') );
    machine.delete_epsilon_transitions();

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
