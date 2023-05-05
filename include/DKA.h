#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "NKA.h"

/*********************************************************************************
Realization of deterministic finite automaton (DFA, DKA in Russian)
Realization is always full DFA (PDKA in Russian)
Can construct from NKA with one-letter edges
Can converse into his addition
Can minimize number of vertexes
Can get regular expression of its language
*********************************************************************************/

struct D_vertex {
//determinate vertex
    std::vector<size_t> outgoing_edges;
    //for all letters we keep vertex which is achievable by this letter
    bool is_terminal = false;
};

struct DKA {
    std::string alphabet;
    //all letters in correct order
    //UB if alphabet contains two same letters
    std::vector<int> reverse_alphabet;
    //contains number of letter (as char) in alphabet
    std::vector<D_vertex> all_vertex;
    //all vertex
    
    DKA();
    
    DKA(const std::string& alphabet_tmp, const NKA& machine);
    //alphabet_tmp is alphabet of language of machine
    //order of chars installs order of letters in alphabet
    //machine must contain only one-letter edges
    //usually it is result of delete_epsilon_transitions
    
    void make_complement();
    //make from this DKA DKA of addititon of language of this DKA
    
    void make_minimal();
    //decrease number of vertex with saving language
    //use struct DKA_make_minimal
    
    bool is_word_in_language(const std::string& word);
    
    std::string RegExFromDKA();
    //convert DKA into regular expression
    //use struct DKA_RegExFromDKA
};

std::ostream& operator<<(std::ostream& os, const DKA& FIRST);

//MPDKA

struct DKA_make_minimal {
    std::string& alphabet;
    std::vector<int>& reverse_alphabet;
    std::vector<D_vertex>& all_vertex;
    //from DKA
    
    std::vector<size_t> previous_EC;
    size_t previous_number_of_different_EC;
    std::vector<size_t> current_EC;
    size_t current_number_of_different_EC;
    //equivalence class == EC
    //EC is from algorithm of creation MPDKA
    //in the beginning previous_EC contains only one class
    //current_EC contains two classes: terminal and non-terminal
    //number_of_different_EC is number of different classes
    
    DKA_make_minimal() = delete;
    DKA_make_minimal(DKA& machine);
    
    //Some functions as parts of algorithm
    void find_correct_final_current_EC();
    void update_current_EC();
    void create_new_DKA_which_is_MPDKA();
    void all_operations();
};

//RegExFromDKA

struct R_vertex {
//vertex for regular expressions
    std::vector<std::string> outgoing_edges;
    //for all vertexes we keep regular expressions in all other vertexes
    //REGEX=0: std::string.empty() (edge is not exist)
    //REGEX=1: std::string == '_' (for last terminal vertex)

    std::vector<char> outgoing_edges_last_operation;
    //last operation in regular expression
    //can be '+', '*', 's' as star
    //can be 'n' as none (for basic regular expressions)
    
    //we will keep only one last terminal vertex
    //than flag is_terminal is not necessary
};

struct string_with_marker {
    std::string str;
    char c;
    //with meaning that we pointed in R_vertex
    string_with_marker(const std::string& str_tmp, char c_tmp);
};

//Regular expression operations
string_with_marker RE_add(const string_with_marker& sm1, const string_with_marker& sm2);
string_with_marker RE_mul(const string_with_marker& sm1, const string_with_marker& sm2);
string_with_marker RE_star(const string_with_marker& sm);

struct All_R_Vertex {
//convenient struct for keeping NKA with reg-ex-edges
    std::vector<R_vertex> all_vertex;
    
    All_R_Vertex(size_t number_of_vertex_in_MPDKA);
    //create vectors of correct size, but empty
    //zero vertex is start
    //last and only last vertex is terminal
    
    std::string& edge(size_t w, size_t out);
    char& last_operation(size_t w, size_t out);
    size_t size();
};

struct DKA_RegExFromDKA {
    std::vector<D_vertex>& V;
    std::string& alphabet;
    All_R_Vertex all_vertex;
    
    DKA_RegExFromDKA() = delete;
    DKA_RegExFromDKA(DKA& machine);
    
    //Some functions as parts of algorithm
    void fill_edges();
    void delete_all_vertex_without_first_and_last();
    void add_new_part_of_reg_ex(size_t w, size_t in, size_t out);
    std::string all_operations();
};
