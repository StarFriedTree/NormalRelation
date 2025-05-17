#include "Normalizer/Relation.h"
#include "Normalizer/Normalize.h"
#include <iostream>
#include <set>

using namespace Normalizer;

int main()
{
    // Create attributes
    Attribute one("one");
    Attribute two("two");
    Attribute three("three");
    Attribute four("four");

    // Create a relation and add attributes
    Relation R("Count");
    R.addAttribute(one);
    R.addAttribute(two);
    R.addAttribute(three);

    // Add FDs to R: {one} -> {two}, {two} -> {three}, {one, three} -> {two}
    FD fd1, fd2, fd3;
    fd1.AddToLeft(&one);      fd1.AddToRight(&two);
    fd2.AddToLeft(&two);      fd2.AddToRight(&three);
    fd3.AddToLeft(&one); fd3.AddToLeft(&three); fd3.AddToRight(&two);

    R.addFD(fd1);
    R.addFD(fd2);
    R.addFD(fd3);

    std::cout << "Initial Relation R:\n" << R.display();

    // Test findClosure: closure of {one}
    std::set<Attribute*> closureInput;
    for (auto* ptr : R.getAttributePtrs())
        if (ptr->getName() == "one")
            closureInput.insert(ptr);

    auto closure = findClosure(closureInput, R.getFDs());
    std::cout << "Closure of {one}: { ";
    for (auto* attr : closure) std::cout << attr->getName() << " ";
    std::cout << "}\n";

    // Create a projection relation R1 with a subset of attributes
    Relation R1("Projection");
    R1.addAttribute(one);
    R1.addAttribute(two);

    std::cout << "\nBefore projection, R1:\n" << R1.display();

    // Project FDs from R to R1
    projectFDs(R, R1);
    std::cout << "\nAfter projectFDs, R1:\n" << R1.display();

    // Minimize the FDs in R1
    R1.minimalBasisFDs();
    std::cout << "\nAfter minimalBasisFDs, R1:\n" << R1.display();

	// Minimize the FDs in R
	R.minimalBasisFDs();
    std::cout << "\nAfter minimalBasisFDs, R:\n" << R.display();

    return 0;
}
