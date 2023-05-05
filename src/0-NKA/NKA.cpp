#include "NKA.h"

edge::edge(size_t to_tmp, char letter_tmp) {
    to = to_tmp;
    letter = letter_tmp;
}

struct {
    bool operator()(const edge& e1, const edge& e2) {
        if (e1.to != e2.to) {
            return e1.to < e2.to;
        }
        return e1.letter < e2.letter;
    }
} edge_less;
//comparsion by to, than by letter

struct {
    bool operator()(const edge& e1, const edge& e2) {
        return e1.to == e2.to && e1.letter == e2.letter;
    }
} edge_equal;
//checking to equality

vertex::vertex(bool is_terminal_tmp) {
    is_terminal = is_terminal_tmp;
}

vertex::vertex(bool is_terminal_tmp, size_t to, char letter) {
    is_terminal = is_terminal_tmp;
    edge edge_tmp(to, letter);
    outgoing_edges.push_back(std::move(edge_tmp));
}

void vertex::push_edge_quick(size_t to, char letter) {
    edge edge_tmp(to, letter);
    outgoing_edges.push_back(std::move(edge_tmp));
}

void vertex::push_edge(size_t to, char letter) {
    for (edge edge_tmp: outgoing_edges) {
        if (edge_tmp.to == to && edge_tmp.letter == letter) {
            return;
        }
    }
    push_edge_quick(to, letter);
}

NKA::NKA() {
    vertex tmp(false);
    all_vertex.push_back(std::move(tmp));
}

NKA::NKA(char letter) {
    if (letter == 0) {
        vertex tmp(true);
        all_vertex.push_back(std::move(tmp));
    } else {
        vertex start(false, 1, letter);
        vertex finish(true);
        all_vertex.push_back(std::move(start));
        all_vertex.push_back(std::move(finish));
    }
}

NKA::NKA(const std::string& str) {
    if (str.empty()) {
        throw "Initialization of NKA by empty string";
    }
    vertex tmp(false);
    all_vertex.push_back(tmp);
    for (size_t i = 0; i < str.size(); ++i) {
        size_t last = all_vertex.size()-1;
        all_vertex[last].push_edge_quick(last+1, str[i]);
        all_vertex.push_back(tmp);
    }
    all_vertex[all_vertex.size()-1].is_terminal = true;
}

void NKA::push_other_NKA_in_end_without_links(const NKA& other) {
    int shift = all_vertex.size();
    for (size_t i = 0; i < other.all_vertex.size(); ++i) {
        all_vertex.push_back(other.all_vertex[i]);
        vertex& last = all_vertex[all_vertex.size()-1];
        for (size_t j = 0; j < last.outgoing_edges.size(); ++j) {
            last.outgoing_edges[j].to += shift;
        }
    }
}

void NKA::delete_epsilon_transitions() {
    NKA_delete_epsilon_transitions current(*this);
    current.all_operations();
}

NKA_delete_epsilon_transitions::NKA_delete_epsilon_transitions(NKA& current) :
    V(current.all_vertex),
    epsilon_achievable(current.all_vertex.size()),
    achievable(current.all_vertex.size(), false)
{
    //now epsilon_achievable is empty
    //fill epsilon_achievable

    //epsilon_achievable with bigger numbers
    for (int i = V.size()-1; i >= 0; --i) {
        epsilon_achievable[i].insert(i);
        std::vector<edge>& E = V[i].outgoing_edges;
        for (size_t j = 0; j < E.size(); ++j) {
            if (E[j].letter == 0 && E[j].to > i) {
                std::set<size_t> new_elements = epsilon_achievable[E[j].to];
                epsilon_achievable[i].merge(new_elements);
            }
        }
    }

    //all epsilon_achievable
    for (size_t i = 0; i < V.size(); ++i) {
        std::vector<edge>& E = V[i].outgoing_edges;
        for (size_t j = 0; j < E.size(); ++j) {
            if (E[j].letter == 0 && E[j].to < i) {
                std::set<size_t> new_elements = epsilon_achievable[E[j].to];
                epsilon_achievable[i].merge(new_elements);
            }
        }
    }
}

void NKA_delete_epsilon_transitions::add_edges_and_terminal_conditions() {
    for (size_t i = 0; i < V.size(); ++i) {
        for (size_t j : epsilon_achievable[i]) {
            if (V[j].is_terminal) {
                V[i].is_terminal = true;
            }
            for (edge& e : V[j].outgoing_edges) {
                if (e.letter != 0) {
                    V[i].push_edge_quick(e.to, e.letter);
                }
            }
        }
    }
}

void NKA_delete_epsilon_transitions::delete_epsilon_edges() {
    for (size_t i = 0; i < V.size(); ++i) {
        std::vector<edge>& E = V[i].outgoing_edges;

        for (size_t j = 0; j < E.size(); ++j) {
            if (E[j].letter == 0) {
                E[j].to = 0;
            }
        }
        std::sort(E.begin(), E.end(), edge_less);
        size_t start = 0;
        while (start < E.size() && E[start].to == 0 && E[start].letter == 0) {
            ++start;
        }
        if (start >= E.size()) {
            E.clear();
        } else {
            while (start > 0) {
                E[start-1].to = E[start].to;
                E[start-1].letter = E[start].letter;
                --start;
            }
        }
    }
}

void NKA_delete_epsilon_transitions::delete_requrring_edges() {
    for (size_t i = 0; i < V.size(); ++i) {
        std::vector<edge>& E = V[i].outgoing_edges;
        auto new_vector_end = std::unique(E.begin(), E.end(), edge_equal);
        while (new_vector_end < E.end()) {
            E.pop_back();
        }
    }
}

void NKA_delete_epsilon_transitions::find_achievable_vertexes_from_start(size_t i) {
    //first calling: i == 0
    if (achievable[i]) {
        return;
    }
    achievable[i] = true;
    for (edge& e : V[i].outgoing_edges) {
        find_achievable_vertexes_from_start(e.to);
    }
}

void NKA_delete_epsilon_transitions::delete_non_achievable_vertexes_from_start() {
    std::vector<size_t> new_numbers(V.size(), 0);
    size_t shift = 0;
    size_t last_achievable = 0;
    for (size_t i = 0; i < V.size(); ++i) {
        if (achievable[i]) {
            V[i-shift] = V[i];
            new_numbers[i] = i-shift;
            last_achievable = i-shift;
        } else {
            ++shift;
        }
    }
    while (last_achievable+1 < V.size()) {
        V.pop_back();
    }
    for (size_t i = 0; i < V.size(); ++i) {
        for (edge& e : V[i].outgoing_edges) {
            e.to = new_numbers[e.to];
        }
    }
}

void NKA_delete_epsilon_transitions::delete_vertexes_with_no_way_to_terminal_vertex() {
    //TODO: find vertexes which do not have way in terminal vertex and delete them
    //Remark: if we created NKA from regular expression,
    //there are no these vertexes
}

void NKA_delete_epsilon_transitions::all_operations() {
    add_edges_and_terminal_conditions();
    delete_epsilon_edges();
    delete_requrring_edges();
    find_achievable_vertexes_from_start(0);
    delete_non_achievable_vertexes_from_start();
    delete_vertexes_with_no_way_to_terminal_vertex();
}

bool NKA::is_word_in_language(const std::string& word) {
    NKA_is_word_in_language current(*this, word);
    return current.is_suffix_in_NKA(0, 0);
}

NKA_is_word_in_language::NKA_is_word_in_language(const NKA& current, const std::string& word_tmp) :
    V(current.all_vertex),
    word(word_tmp),
    symbol_numbers_of_launch(current.all_vertex.size())
{}

bool NKA_is_word_in_language::is_suffix_in_NKA(size_t first_symbol, size_t current_vertex) {
    {
        size_t size = symbol_numbers_of_launch[current_vertex].size();
        if (size != 0 && symbol_numbers_of_launch[current_vertex][size-1] == first_symbol) {
            //went by epsilon cycle
            return false;
        }
        symbol_numbers_of_launch[current_vertex].push_back(first_symbol);
    }
    if (first_symbol == word.size() && V[current_vertex].is_terminal) {
        symbol_numbers_of_launch[current_vertex].pop_back();
        return true;
    }
    for (const edge& e: V[current_vertex].outgoing_edges) {
        if (first_symbol != word.size() && e.letter == word[first_symbol]) {
            if (is_suffix_in_NKA(first_symbol+1, e.to)) {
                symbol_numbers_of_launch[current_vertex].pop_back();
                return true;
            }
        }
        if (e.letter == 0) {
            if (is_suffix_in_NKA(first_symbol, e.to)) {
                symbol_numbers_of_launch[current_vertex].pop_back();
                return true;
            }
        }
    }
    symbol_numbers_of_launch[current_vertex].pop_back();
    return false;
}

NKA operator+(const NKA& FIRST, const NKA& SECOND) {
    if (FIRST.all_vertex.empty() || SECOND.all_vertex.empty()) {
        throw "Regular expression operation with empty NKA";
    }

    NKA RESULT;
    //one non-terminal vertex

    RESULT.all_vertex[0].push_edge_quick(RESULT.all_vertex.size(), 0);
    RESULT.push_other_NKA_in_end_without_links(FIRST);
    //add FIRST

    RESULT.all_vertex[0].push_edge_quick(RESULT.all_vertex.size(), 0);
    RESULT.push_other_NKA_in_end_without_links(SECOND);
    //add SECOND

    return RESULT;
}

NKA operator*(const NKA& FIRST, const NKA& SECOND) {
    if (FIRST.all_vertex.empty() || SECOND.all_vertex.empty()) {
        throw "Regular expression operation with empty NKA";
    }

    NKA RESULT(FIRST);
    for (size_t i = 0; i < RESULT.all_vertex.size(); ++i) {
        if (RESULT.all_vertex[i].is_terminal) {
            RESULT.all_vertex[i].is_terminal = false;
            RESULT.all_vertex[i].push_edge_quick(RESULT.all_vertex.size(), 0);
        }
    }
    RESULT.push_other_NKA_in_end_without_links(SECOND);

    return RESULT;
}

NKA KleeneStar(const NKA& FIRST) {
    if (FIRST.all_vertex.empty()) {
        throw "Regular expression operation with empty NKA";
    }

    NKA RESULT;
    //one non-terminal vertex
    RESULT.all_vertex[0].push_edge_quick(1, 0);
    RESULT.push_other_NKA_in_end_without_links(FIRST);

    RESULT.all_vertex[0].is_terminal = true;
    for (size_t i = 1; i < RESULT.all_vertex.size(); ++i) {
        if (RESULT.all_vertex[i].is_terminal) {
            RESULT.all_vertex[i].is_terminal = false;
            RESULT.all_vertex[i].push_edge_quick(0, 0);
        }
    }

    return RESULT;
}

NKA KleenePlus(const NKA& FIRST) {
    return FIRST*KleeneStar(FIRST);
}

std::ostream& operator<<(std::ostream& os, const NKA& FIRST) {
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
            const std::vector<edge>& edges = FIRST.all_vertex[i].outgoing_edges;
            char c = (edges[j].letter == 0) ? '_' : edges[j].letter;
            os << i << " ---" << c << "---> " << edges[j].to << '\n';
        }
    }
    return os;
}
