#include "Normalizer/Relation.h"
#include <iostream>

using namespace Normalizer;

int main()
{
    // Create attributes
    Attribute one("one");
    Attribute two("two");
    Attribute three("three");

    // Create a relation and add attributes
    Relation R("Count");
    R.addAttribute(one);
    R.addAttribute(two);
    R.addAttribute(three);

    // Create another relation with a different 'two' (different pointer, same value)
    Attribute othertwo("two");
    Attribute four("four");
    Relation R2("Other");
    R2.addAttribute(one);
    R2.addAttribute(othertwo);
    R2.addAttribute(four);

    // Create an FD in R2: {one} -> {two}
    FD fd_other;
    fd_other.AddToLeft(&one);
    fd_other.AddToRight(&othertwo);

    // Try to add this FD to R (should be validateable, not valid)
    std::cout << "R.isValidFD(fd_other): " << R.isValidFD(fd_other) << '\n';
    std::cout << "R.isValidateableFD(fd_other): " << R.isValidateableFD(fd_other) << '\n';
    bool added = R.addFD(fd_other);
    std::cout << "R.addFD(fd_other): " << added << '\n';

    // Try to add a non-validateable FD (with an attribute not in R)
    FD fd_invalid;
    fd_invalid.AddToLeft(&four);
    fd_invalid.AddToRight(&one);
    std::cout << "R.isValidateableFD(fd_invalid): " << R.isValidateableFD(fd_invalid) << '\n';
    std::cout << "R.addFD(fd_invalid): " << R.addFD(fd_invalid) << '\n';

    // Search for attributes
    std::cout << "R.searchAttribute(two): " << R.searchAttribute(two) << '\n';
    std::cout << "R.searchAttribute(\"two\"): " << R.searchAttribute("two") << '\n';
    std::cout << "R.searchAttribute(&two): " << R.searchAttribute(&two) << '\n';
    std::cout << "R.searchAttribute(othertwo): " << R.searchAttribute(othertwo) << '\n';
    std::cout << "R.searchAttribute(&othertwo): " << R.searchAttribute(&othertwo) << '\n';
    std::cout << "R.searchAttribute(four): " << R.searchAttribute(four) << '\n';

    // Display the relation (attributes and FDs)
    std::cout << "\nRelation R:\n" << R.display();

    return 0;
}