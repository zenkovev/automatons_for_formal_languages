// Примеры построения полного ДКА по регулярному выражению

#include <NKA.h>
#include <DKA.h>

int main() {
    std::cout << "(ab)*b*+((a+b)^2)*\n";
    std::cout << "PDKA - ?\n\n";

    NKA A1 = KleeneStar(NKA("ab"))*KleeneStar(NKA('b'))
            +KleeneStar((NKA('a')+NKA('b'))*(NKA('a')+NKA('b')));
    A1.delete_epsilon_transitions();
    DKA DA1("ab", A1);
    std::cout << DA1 << "\n\n";

    std::cout << "(a+b)^+abb(a+b)^+\n";
    std::cout << "PDKA - ?\n\n";

    NKA A2 = KleenePlus(NKA('a')+NKA('b'))*NKA("abb")*KleenePlus(NKA('a')+NKA('b'));
    A2.delete_epsilon_transitions();
    DKA DA2("ab", A2);
    std::cout << DA2 << "\n\n";

    std::cout << "a(a(ab)*a(ab)*+a)*\n";
    std::cout << "PDKA - ?\n\n";

    NKA A3 = NKA('a')*KleeneStar(
            NKA('a')*KleeneStar(NKA('a')+NKA('b'))*NKA('a')*KleeneStar(NKA('a')+NKA('b'))+NKA('a'));
    A3.delete_epsilon_transitions();
    DKA DA3("ab", A3);
    std::cout << DA3;
}
