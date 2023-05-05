// Примеры построения по регулярному выражению
// МПДКА и регулярных выражений для дополнений языков

#include <NKA.h>
#include <DKA.h>

int main() {
    std::cout << "((ab+ba)*(1+a+ba)\n";
    std::cout << "MPDKA - ?\n\n";

    NKA A1 = KleeneStar(NKA("ab"))*KleeneStar(NKA('b'))
            +KleeneStar((NKA('a')+NKA('b'))*(NKA('a')+NKA('b')));
    A1.delete_epsilon_transitions();
    DKA DA1("ab", A1);
    DA1.make_minimal();
    std::cout << DA1 << "\n\n";

    std::cout << "(ab)*b*+((a+b)^2)*\n";
    std::cout << "MPDKA - ?\n\n";

    NKA A2 = KleeneStar(NKA("ab"))*KleeneStar(NKA('b'))
            +KleeneStar((NKA('a')+NKA('b'))*(NKA('a')+NKA('b')));
    A2.delete_epsilon_transitions();
    DKA DA2("ab", A2);
    DA2.make_minimal();
    std::cout << DA2 << "\n\n";

    std::cout << "(a+b)^+abb(a+b)^+\n";
    std::cout << "MPDKA - ?\n\n";

    NKA A3 = KleenePlus(NKA('a')+NKA('b'))*NKA("abb")*KleenePlus(NKA('a')+NKA('b'));
    A3.delete_epsilon_transitions();
    DKA DA3("ab", A3);
    DA3.make_minimal();
    std::cout << DA3 << "\n\n";

    std::cout << "a(a(ab)*a(ab)*+a)*\n";
    std::cout << "MPDKA - ?\n\n";

    NKA A4 = NKA('a')*KleeneStar(
            NKA('a')*KleeneStar(NKA('a')+NKA('b'))*NKA('a')*KleeneStar(NKA('a')+NKA('b'))+NKA('a'));
    A4.delete_epsilon_transitions();
    DKA DA4("ab", A4);
    DA4.make_minimal();
    std::cout << DA4 << "\n\n";


    std::cout << "((ab+ba)*(1+a+ba)\n";
    std::cout << "RegEx for addition - ?\n";

    DA1.make_complement();
    DA1.make_minimal();
    std::cout << DA1.RegExFromDKA() << "\n\n";

    std::cout << "(ab)*b*+((a+b)^2)*\n";
    std::cout << "RegEx for addition - ?\n";

    DA2.make_complement();
    DA2.make_minimal();
    std::cout << DA2.RegExFromDKA() << "\n\n";

    std::cout << "(a+b)^+abb(a+b)^+\n";
    std::cout << "RegEx for addition - ?\n";

    DA3.make_complement();
    DA3.make_minimal();
    std::cout << DA3.RegExFromDKA() << "\n\n";

    std::cout << "a(a(ab)*a(ab)*+a)*\n";
    std::cout << "RegEx for addition - ?\n";

    DA4.make_complement();
    DA4.make_minimal();
    std::cout << DA4.RegExFromDKA() << '\n';
}
