#include "Normalizer/Relation.h"
#include "Normalizer/Normalize.h"
#include "Utility.h"
#include <iostream>
#include <set>

using namespace Normalizer;

int main()
{
	Attribute A("A");
	Attribute B("B");
	Attribute C("C");
	Attribute D("D");
	Attribute E("E");

    Relation R("R");
	R.addAttribute(A);
	R.addAttribute(B);
	R.addAttribute(C);
	R.addAttribute(D);
	R.addAttribute(E);

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

	std::cout << "R:\n" << R << "\n\n";

	auto bcnfRelations = bcnf(R);

	std::cout << "R:\n" << R << "\ndecomposed:\n" << Util::printSet(bcnfRelations, ' ', ' ', "");

    return 0;
}
