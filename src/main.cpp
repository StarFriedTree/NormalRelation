#include "Normalizer/Relation.h"
#include "Normalizer/Normalize.h"
#include "Utility.h"
#include <iostream>
#include <set>

using namespace Normalizer;

int main()
{
    // Define attributes
    Attribute A("A");
    Attribute B("B");
    Attribute C("C");
    Attribute D("D");
    Attribute E("E");

    // Create relation and add attributes
    Relation R("R");
    R.addAttribute(A);
    R.addAttribute(B);
    R.addAttribute(C);
    R.addAttribute(D);
    R.addAttribute(E);

    // Add FDs
    FD fd1;
    fd1.AddToLeft(&A);
    fd1.AddToLeft(&B);
    fd1.AddToRight(&C);
    fd1.AddToRight(&D);

    FD fd2;
    fd2.AddToLeft(&D);
    fd2.AddToRight(&E);

    FD fd3;
    fd3.AddToLeft(&A);
    fd3.AddToRight(&C);

    FD fd4;
    fd4.AddToLeft(&B);
    fd4.AddToRight(&D);

    R.addFD(fd1);
    R.addFD(fd2);
    R.addFD(fd3);
    R.addFD(fd4);

    std::cout << "Original Relation R:\n" << R << "\n";

    // Test findKey
    auto keyPtrs = findKey(R);
    std::cout << "Candidate Key: { ";
    for (auto* attr : keyPtrs)
        std::cout << attr->getName() << " ";
    std::cout << "}\n\n";

    // Test threeNF
    auto threeNFRelations = threeNF(R);
    std::cout << "3NF Decomposition:\n" << Util::printSet(threeNFRelations, " ", " ", "") << "\n";

    return 0;
}
