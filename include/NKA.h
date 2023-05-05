#pragma once
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>

/*********************************************************************************
Realization of nondeterministic finite automaton (NFA, NKA in Russian)
Realization can contain only <=one-letter edges
Can construct from letter, string and can do operations of regular expressions
Can remove epsilon edges
*********************************************************************************/

//Struct-helpers for main struct

struct edge {
    //keep in vertex
    size_t to;
    //vertex, which is end of edge
    char letter;
    //0 is empty word
    //in char 0 is NUL than it is convinient

    edge(size_t to_tmp, char letter_tmp);
};

struct vertex {
    std::vector<edge> outgoing_edges;
    bool is_terminal;

    vertex(bool is_terminal_tmp);
    //construct vertex without outgoing edges

    vertex(bool is_terminal_tmp, size_t to, char letter);
    //construct vertex with one outgoing edge(to, letter)

    void push_edge_quick(size_t to, char letter);
    //push new edge as last outgoing edge
    //if this edge exist we push copy of egde

    void push_edge(size_t to, char letter);
    //push new edge as last outgoing edge
    //if this edge exist we do not push copy of egde
    //checking of this statement is not quick
};

//Main struct

struct NKA {
    std::vector<vertex> all_vertex;
    //all_vertex[0] is starting vertex

    NKA();
    //construct NKA == void set
    //NKA become one non-terminal vertex

    NKA(char letter);
    //construct NKA == {letter}

    NKA(const std::string& str);
    //construct NKA == {str}

    void push_other_NKA_in_end_without_links(const NKA& other);
    //push all vertexes of other NKA in end of current NKA
    //save edges of other NKA
    //do not add edges between current NKA and other NKA

    void delete_epsilon_transitions();
    //all edges become one-letter edges
    //call struct-helper NKA_delete_epsilon_transitions
    
    bool is_word_in_language(const std::string& word);
};

struct NKA_delete_epsilon_transitions {
    std::vector<vertex>& V;
    //all_vertex from NKA
    std::vector< std::set<size_t> > epsilon_achievable;
    //epsilon-achievable from this vertex
    std::vector<bool> achievable;
    //edge-achievable from start vertex
    //it is necessary in some moment
    //in this moment it will be fill
    //earlier it is filled in by default

    NKA_delete_epsilon_transitions() = delete;

    NKA_delete_epsilon_transitions(NKA& current);
    void add_edges_and_terminal_conditions();
    void delete_epsilon_edges();
    void delete_requrring_edges();
    void find_achievable_vertexes_from_start(size_t i);
    void delete_non_achievable_vertexes_from_start();
    void delete_vertexes_with_no_way_to_terminal_vertex();
    void all_operations();
};

struct NKA_is_word_in_language {
    const std::vector<vertex>& V;
    //all_vertex from NKA
    const std::string& word;
    std::vector<std::vector<size_t>> symbol_numbers_of_launch;
    //for all vertexes in recursion stack keep all numbers of first_symbol of launch
    //it is solution of problem of epsilon cycle
    
    NKA_is_word_in_language() = delete;
    
    NKA_is_word_in_language(const NKA& current, const std::string& word_tmp);
    bool is_suffix_in_NKA(size_t first_symbol, size_t current_vertex);
    //suffix of word from first_symbol from current_vertex
};

//Regular expression operations

NKA operator+(const NKA& FIRST, const NKA& SECOND);
NKA operator*(const NKA& FIRST, const NKA& SECOND);
NKA KleeneStar(const NKA& FIRST);
NKA KleenePlus(const NKA& FIRST);

//Print NKA

std::ostream& operator<<(std::ostream& os, const NKA& FIRST);
