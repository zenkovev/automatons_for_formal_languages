/*****
  Даны alpha и слово u из {a, b, c}*.
  Найти длину самого длинного подслова u, принадлежащего L.

  С учётом реализации НКА и ДКА алгоритм совсем простой: строим НКА, по нему строим МПДКА, затем
  считываем слово u и все его суффиксы, каждый раз, когда попадаем в терминальную вершину,
  сравниваем длину полученной подстроки из языка с длиной ранее найденной, помним самую длинную.
  Асимптотика: O(|u|^2) на запрос.

  alpha - регулярное выражение в обратной польской нотации
  u - слово алфавита

  Пример:
  input:
    ab+*
    acbcaabbbcabbcabca
  output:
    aabbb
*****/

#include <NKA.h>
#include <DKA.h>

NKA NKA_from_symbol(char c) {
    if (c == '1') {
        return NKA(0);
    }
    if (c >= 'a' && c <= 'c') {
        return NKA(c);
    }
    if (c == '.') {
        return NKA('a')+NKA('b')+NKA('c');
    }
    throw "Regular expression is not correct: a, b, c, 1, . expected";
}

class NKA_builder {
public:
    NKA& machine;
    const std::string& regex;

    NKA_builder() = delete;

    NKA_builder(NKA& machine_tmp, const std::string& regex_tmp) :
        machine(machine_tmp),
        regex(regex_tmp)
    {}

private:
    void machine_first_building(size_t& regex_pos) {
        machine = NKA_from_symbol(regex[regex_pos]);
        ++regex_pos;
    }

    void machine_current_building_only_operation_star(size_t& regex_pos) {
        //read next operation star
        //do this operation
        machine = KleeneStar(machine);
        ++regex_pos;
    }

    void machine_current_building_operand_and_operation(size_t& regex_pos) {
        //read next operand and next operation
        //do this operation
        NKA new_machine = NKA_from_symbol(regex[regex_pos]);
        ++regex_pos;
        if (regex_pos >= regex.size()) {
            throw "Regular expression is not correct: +, * expected";
        }

        if (regex[regex_pos] != '+' && regex[regex_pos] != '*') {
            throw "Regular expression is not correct: +, * expected";
        }
        if (regex[regex_pos] == '+') {
            machine = machine + new_machine;
        }
        if (regex[regex_pos] == '*') {
            machine = machine*new_machine;
        }
        ++regex_pos;
    }

    void machine_current_building(size_t& regex_pos) {
        //read next operand and/or next operation
        //do this operation
        if (regex[regex_pos] == '*') {
            machine_current_building_only_operation_star(regex_pos);
        } else {
            machine_current_building_operand_and_operation(regex_pos);
        }
    }

public:
    void build() {
        size_t regex_pos = 0;
        if (regex_pos < regex.size()) {
            machine_first_building(regex_pos);
        }
        while (regex_pos < regex.size()) {
            machine_current_building(regex_pos);
        }
    }
};

class DKA_finder {
public:
    const DKA& DKA_machine;
    const std::string& word;
    int l; //borders of subword
    int r; //borders of subword
    int len = -1; //len of subword

    DKA_finder() = delete;

    DKA_finder(const DKA& DKA_machine_tmp, const std::string& word_tmp) :
        DKA_machine(DKA_machine_tmp),
        word(word_tmp)
    {}

    void find() {
        int word_size = word.size();

        for (int first_symbol = 0; first_symbol < word_size; ++first_symbol) {
            int end_symbol = first_symbol-1;
            int vertex = 0;
            while (end_symbol < word_size) {
                if (DKA_machine.all_vertex[vertex].is_terminal) {
                    if (end_symbol-first_symbol+1 > len) {
                        l = first_symbol;
                        r = end_symbol;
                        len = end_symbol - first_symbol + 1;
                    }
                }
                ++end_symbol;
                if (end_symbol != word_size) {
                    vertex = DKA_machine.all_vertex[vertex].outgoing_edges[word[end_symbol]-'a'];
                }
            }
        }
    }
};

int main() {
    NKA machine;

    std::string regex;
    std::cin >> regex;

    NKA_builder builder(machine, regex);
    builder.build();

    machine.delete_epsilon_transitions();
    DKA DKA_machine("abc", machine);
    DKA_machine.make_minimal();

    std::string word;
    std::cin >> word;

    DKA_finder finder(DKA_machine, word);
    finder.find();
    int l = finder.l;
    int r = finder.r;
    int len = finder.len;

    if (len == -1) {
        std::cout << "INF\n";
    } else {
        for (int i = l; i <= r; ++i) {
            std::cout << word[i];
        }
        std::cout << '\n';
    }
}
