#include "Normalizer/Relation.h"

#include <iostream>
using namespace Normalizer;

int main ()
{
    Attribute one ("one");
    Attribute two ("two");
    Attribute three ("three");

    Relation R ("Count");

    R.addAttribute(one);
    R.addAttribute(two);
    R.addAttribute(three);

    Attribute othertwo ("two");
    Attribute four ("four");


    std::cout << R.searchAttribute(two) << '\n'
              << R.searchAttribute("two") << '\n'
              << R.searchAttribute(&two) << '\n'
              << R.searchAttribute(othertwo) << '\n'
              << R.searchAttribute(&othertwo) << '\n'
              << R.searchAttribute(four) << '\n';

}