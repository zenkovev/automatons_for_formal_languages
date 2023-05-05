#include "DKA.h"

/* After thinking for a while, I have made a decision that this function
will be more understandable, if all its logic will contain in one place,
not in different functions, because all its parts closely related */

DKA::DKA() {}

DKA::DKA(const std::string& alphabet_tmp, const NKA& machine) :
        alphabet(alphabet_tmp),
        reverse_alphabet(128, 0)
{
    for (size_t i = 0; i < alphabet.size(); ++i) {
        reverse_alphabet[alphabet[i]] = i;
    }

    std::vector<std::vector<bool>> subset_old_vertex;
    //vector-helper for bijection of new vertex and subset of old vertex

    //put new subset as void vertex in vector all_vertex
    //put his value in vector subset_old_vertex
    //keep number of last non-processed vertex-subset
    size_t last;

    {
        std::vector<bool> zero(machine.all_vertex.size(), false);
        //void subset
        zero[0] = true;
        //subset == {0}
        all_vertex.push_back(D_vertex());
        subset_old_vertex.push_back(std::move(zero));
        last = 0;
    }

    while (last < all_vertex.size()) {
        for (size_t char_counter = 0; char_counter < alphabet.size(); ++char_counter) {
            std::vector<bool> achievable_subset(machine.all_vertex.size(), false);
            //collect achievable subset from this subset by letter alphabet[char_counter]

            for (size_t i = 0; i < machine.all_vertex.size(); ++i) {
                if (subset_old_vertex[last][i]) {
                    for (const edge& e : machine.all_vertex[i].outgoing_edges) {
                        if (e.letter == alphabet[char_counter]) {
                            achievable_subset[e.to] = true;
                        }
                    }
                }
            }

            int number_of_achievable_subset = -1;
            for (size_t j = 0; j < subset_old_vertex.size(); ++j) {
                if (subset_old_vertex[j] == achievable_subset) {
                    number_of_achievable_subset = j;
                    break;
                }
            }

            if (number_of_achievable_subset != -1) {
                all_vertex[last].outgoing_edges.push_back(number_of_achievable_subset);
            } else {
                all_vertex[last].outgoing_edges.push_back(all_vertex.size());

                all_vertex.push_back(D_vertex());
                subset_old_vertex.push_back(std::move(achievable_subset));
            }
        }
        ++last;
    }

    //add terminal conditions
    for (size_t i = 0; i < all_vertex.size(); ++i) {
        for (size_t j = 0; j < subset_old_vertex[i].size(); ++j) {
            if (subset_old_vertex[i][j] && machine.all_vertex[j].is_terminal) {
                all_vertex[i].is_terminal = true;
            }
        }
    }
}

void DKA::make_complement() {
    for (size_t i = 0; i < all_vertex.size(); ++i) {
        all_vertex[i].is_terminal = !all_vertex[i].is_terminal;
    }
}

bool DKA::is_word_in_language(const std::string& word) {
    size_t v = 0;
    for (size_t c = 0; c < word.size(); ++c) {
        v = all_vertex[v].outgoing_edges[reverse_alphabet[word[c]]];
    }
    return all_vertex[v].is_terminal;
}

std::ostream& operator<<(std::ostream& os, const DKA& FIRST) {
    for (int i = 0; i < FIRST.all_vertex.size(); ++i) {
        if (FIRST.all_vertex[i].is_terminal) {
            os << "((" << i << "))";
        } else {
            os << "(" << i << ")";
        }
        if (i+1 < FIRST.all_vertex.size()) {
            os << ' ';
        } else {
            os << '\n';
        }
    }
    for (int i = 0; i < FIRST.all_vertex.size(); ++i) {
        for (int j = 0; j < FIRST.all_vertex[i].outgoing_edges.size(); ++j) {
            const std::vector<size_t>& edges = FIRST.all_vertex[i].outgoing_edges;
            char c = FIRST.alphabet[j];
            if (c == 0) {
                c = '_';
            }
            os << i << " ---" << c << "---> " << edges[j] << '\n';
        }
    }
    return os;
}





//MPDKA

DKA_make_minimal::DKA_make_minimal(DKA& machine) :
    alphabet(machine.alphabet),
    reverse_alphabet(machine.reverse_alphabet),
    all_vertex(machine.all_vertex),
    previous_EC(machine.all_vertex.size(), 0),
    previous_number_of_different_EC(1),
    current_EC(machine.all_vertex.size(), 0),
    current_number_of_different_EC(1)
{
    for (size_t i = 0; i < all_vertex.size(); ++i) {
        if (all_vertex[i].is_terminal) {
            current_EC[i] = 1;
            current_number_of_different_EC = 2;
        }
    }
}

//Continue minimization of DKA

void DKA_make_minimal::find_correct_final_current_EC() {
    while (previous_number_of_different_EC != current_number_of_different_EC) {
        previous_EC = current_EC;
        previous_number_of_different_EC = current_number_of_different_EC;
        update_current_EC();
    }
}

void DKA_make_minimal::update_current_EC() {
    std::vector< std::vector<size_t> > classes_by_letters(all_vertex.size());
    //for all vertexes keep numbers in previous_EC
    //which are achievable by this letter

    for (size_t v = 0; v < all_vertex.size(); ++v) {
        for (size_t c = 0; c < alphabet.size(); ++c) {
            classes_by_letters[v].push_back( previous_EC[all_vertex[v].outgoing_edges[c]] );
        }
    }
    //and words of smaller len also make difference between classes
    for (size_t v = 0; v < all_vertex.size(); ++v) {
        classes_by_letters[v].push_back(previous_EC[v]);
    }

    current_number_of_different_EC = 0;
    for (size_t v = 0; v < all_vertex.size(); ++v) {
        int number_of_similar_vector_of_previous_vertex = -1;
        for (int w = 0; w < v; ++w) {
            if (classes_by_letters[v] == classes_by_letters[w]) {
                number_of_similar_vector_of_previous_vertex = current_EC[w];
                break;
            }
        }

        if (number_of_similar_vector_of_previous_vertex == -1) {
            number_of_similar_vector_of_previous_vertex = current_number_of_different_EC;
            ++current_number_of_different_EC;
        }
        current_EC[v] = number_of_similar_vector_of_previous_vertex;
    }
}

void DKA_make_minimal::create_new_DKA_which_is_MPDKA() {
    //add new edges for new vertexes
    std::vector<D_vertex> new_all_vertex(current_number_of_different_EC);
    size_t v = 0;
    for (size_t new_v = 0; new_v < new_all_vertex.size(); ++new_v) {
        while (current_EC[v] < new_v) {
            ++v;
        }
        //current_EC[v] == new_v
        for (size_t c = 0; c < alphabet.size(); ++c) {
            new_all_vertex[new_v].outgoing_edges.push_back(
                    current_EC[ all_vertex[v].outgoing_edges[c] ]);
        }
    }

    //add terminal conditions
    for (size_t v = 0; v < all_vertex.size(); ++v) {
        if (all_vertex[v].is_terminal) {
            new_all_vertex[current_EC[v]].is_terminal = true;
        }
    }

    all_vertex = new_all_vertex;
}

void DKA_make_minimal::all_operations() {
    find_correct_final_current_EC();
    create_new_DKA_which_is_MPDKA();
}

//Done minimization of DKA

void DKA::make_minimal() {
    DKA_make_minimal current(*this);
    current.all_operations();
}





//RegExFromDKA

string_with_marker::string_with_marker(const std::string& str_tmp, char c_tmp) :
    str(str_tmp),
    c(c_tmp)
{}

string_with_marker RE_add(const string_with_marker& sm1, const string_with_marker& sm2) {
    if (!sm1.str.empty() && !sm2.str.empty()) {
        return string_with_marker(sm1.str + "+" + sm2.str, '+');
    }
    if (sm1.str.empty()) {
        return sm2;
    } else {
        return sm1;
    }
}

string_with_marker RE_mul(const string_with_marker& sm1, const string_with_marker& sm2) {
    if (sm1.str.empty() || sm2.str.empty()) {
        return string_with_marker(std::string(), 'n');
    }
    if (sm1.str == "_") {
        return sm2;
    }
    if (sm2.str == "_") {
        return sm1;
    }
    std::string str1copy, str2copy;
    if (sm1.c != '+') {
        str1copy = sm1.str;
    } else {
        str1copy = "(" + sm1.str + ")";
    }
    if (sm2.c != '+') {
        str2copy = sm2.str;
    } else {
        str2copy = "(" + sm2.str + ")";
    }
    return string_with_marker(str1copy + str2copy, '*');
}

string_with_marker RE_star(const string_with_marker& sm) {
    if (sm.str.empty() || sm.str == "_") {
        return string_with_marker("_", 'n');
    }
    if (sm.c == 'n') {
        return string_with_marker(sm.str + "*", 's');
    }
    return string_with_marker("(" + sm.str + ")*", 's');
}

All_R_Vertex::All_R_Vertex(size_t number_of_vertex_in_MPDKA) {
    all_vertex = std::vector<R_vertex>(number_of_vertex_in_MPDKA+1);
    //for only one last terminal vertex

    for (size_t w = 0; w < all_vertex.size(); ++w) {
        all_vertex[w].outgoing_edges = std::vector<std::string>(all_vertex.size());
        all_vertex[w].outgoing_edges_last_operation = std::vector<char>(all_vertex.size(), 'n');
    }
}

std::string& All_R_Vertex::edge(size_t w, size_t out) {
    return all_vertex[w].outgoing_edges[out];
}

char& All_R_Vertex::last_operation(size_t w, size_t out) {
    return all_vertex[w].outgoing_edges_last_operation[out];
}

size_t All_R_Vertex::size() {
    return all_vertex.size();
}

DKA_RegExFromDKA::DKA_RegExFromDKA(DKA& machine) :
    V(machine.all_vertex),
    alphabet(machine.alphabet),
    all_vertex(machine.all_vertex.size())
{}

void DKA_RegExFromDKA::fill_edges() {
    for (size_t v = 0; v < V.size(); ++v) {
        if (V[v].is_terminal) {
            all_vertex.edge(v, all_vertex.size()-1) = "_";
            all_vertex.last_operation(v, all_vertex.size()-1) = 'n';
        }

        for (size_t symbol = 0; symbol < V[v].outgoing_edges.size(); ++symbol) {
            size_t to = V[v].outgoing_edges[symbol];
            char letter = alphabet[symbol];

            if (all_vertex.edge(v, to).empty()) {
                all_vertex.edge(v, to) = letter;
                all_vertex.last_operation(v, to) = 'n';
            } else {
                all_vertex.edge(v, to).push_back('+');
                all_vertex.edge(v, to).push_back(letter);
                all_vertex.last_operation(v, to) = '+';
            }
        }
    }
}

void DKA_RegExFromDKA::delete_all_vertex_without_first_and_last() {
    for (size_t w = all_vertex.size()-2; w > 0; --w) {
        for (size_t in = 0; in < all_vertex.size(); ++in) {
            if (in == w || all_vertex.edge(in, w).empty()) {
                continue;
            }
            for (size_t out = 0; out < all_vertex.size(); ++out) {
                if (out == w || all_vertex.edge(w, out).empty()) {
                    continue;
                }
                add_new_part_of_reg_ex(w, in, out);
            }
        }
        //delete vertex - clean all its edges in other vertexes
        for (size_t i = 0; i < all_vertex.size(); ++i) {
            all_vertex.edge(w, i).clear();
            all_vertex.edge(i, w).clear();
        }
    }
}

void DKA_RegExFromDKA::add_new_part_of_reg_ex(size_t w, size_t in, size_t out) {
    string_with_marker old(all_vertex.edge(in, out), all_vertex.last_operation(in, out));
    string_with_marker left(all_vertex.edge(in, w), all_vertex.last_operation(in, w));
    string_with_marker centre(all_vertex.edge(w, w), all_vertex.last_operation(w, w));
    string_with_marker right(all_vertex.edge(w, out), all_vertex.last_operation(w, out));

    string_with_marker centre_star = RE_star(centre);
    string_with_marker left_centre_star = RE_mul(left, centre_star);
    string_with_marker left_centre_star_right = RE_mul(left_centre_star, right);
    string_with_marker result = RE_add(old, left_centre_star_right);

    all_vertex.edge(in, out) = result.str;
    all_vertex.last_operation(in, out) = result.c;
}

std::string DKA_RegExFromDKA::all_operations() {
    fill_edges();
    delete_all_vertex_without_first_and_last();
    
    string_with_marker cycle(all_vertex.edge(0, 0), all_vertex.last_operation(0, 0));
    string_with_marker way(all_vertex.edge(0, all_vertex.size()-1), all_vertex.last_operation(0, all_vertex.size()-1));
    string_with_marker cycle_star = RE_star(cycle);
    return RE_mul(cycle_star, way).str;
}

std::string DKA::RegExFromDKA() {
    DKA_RegExFromDKA current(*this);
    return current.all_operations();
}
