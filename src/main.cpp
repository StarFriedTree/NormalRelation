#include "Normalizer/Relation.h"
#include "Normalizer/Normalize.h"
#include "Utility.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <limits>

using namespace Normalizer;

void printAttributes(const Relation& R) {
    std::cout << "Attributes: ";
    for (const auto& attr : R.getAttributes())
        std::cout << attr.getName() << " ";
    std::cout << "\n";
}

Attribute* getAttributePtrByName(Relation& R, const std::string& name) {
    for (auto& attr : R.getAttributes()) {
        if (attr.getName() == name)
            return const_cast<Attribute*>(&attr);
    }
    return nullptr;
}

void addAttributeMenu(Relation& R) {
    std::string name;
    std::cout << "Enter attribute name: ";
    std::cin >> name;
    if (R.addAttribute(Attribute(name)))
        std::cout << "Attribute added.\n";
    else
        std::cout << "Attribute already exists.\n";
}

void addFDMenu(Relation& R) {
    printAttributes(R);
    std::cout << "Enter left side attribute names (space separated, end with .): ";
    FD fd;
    std::string name;
    while (std::cin >> name && name != ".") {
        Attribute* ptr = getAttributePtrByName(R, name);
        if (ptr) fd.AddToLeft(ptr);
        else std::cout << "(Ignored unknown attribute " << name << ") ";
    }
    std::cout << "Enter right side attribute names (space separated, end with .): ";
    while (std::cin >> name && name != ".") {
        Attribute* ptr = getAttributePtrByName(R, name);
        if (ptr) fd.AddToRight(ptr);
        else std::cout << "(Ignored unknown attribute " << name << ") ";
    }
    if (R.addFD(fd))
        std::cout << "FD added.\n";
    else
        std::cout << "FD invalid or already exists.\n";
}

void addMVDMenu(Relation& R) {
    printAttributes(R);
    std::cout << "Enter left side attribute names (space separated, end with .): ";
    MVD mvd;
    std::string name;
    while (std::cin >> name && name != ".") {
        Attribute* ptr = getAttributePtrByName(R, name);
        if (ptr) mvd.AddToLeft(ptr);
        else std::cout << "(Ignored unknown attribute " << name << ") ";
    }
    std::cout << "Enter right side attribute names (space separated, end with .): ";
    while (std::cin >> name && name != ".") {
        Attribute* ptr = getAttributePtrByName(R, name);
        if (ptr) mvd.AddToRight(ptr);
        else std::cout << "(Ignored unknown attribute " << name << ") ";
    }
    if (R.addMVD(mvd))
        std::cout << "MVD added.\n";
    else
        std::cout << "MVD invalid or already exists.\n";
}

void changeTitleMenu(Relation& R) {
    std::string newTitle;
    std::cout << "Enter new relation title: ";
    std::cin >> newTitle;
    R.setTitle(newTitle);
    std::cout << "Title changed.\n";
}

void decomposeMenu(Relation& R) {
    std::cout << "Choose decomposition:\n";
    std::cout << "1. 3NF\n";
    std::cout << "2. BCNF\n";
    std::cout << "3. 4NF\n";
    int choice;
    std::cin >> choice;
    std::set<Relation> result;
    switch (choice) {
    case 1:
        result = threeNF(R);
        std::cout << "3NF Decomposition:\n";
        break;
    case 2:
        result = BCNF(R);
        std::cout << "BCNF Decomposition:\n";
        break;
    case 3:
        result = fourNF(R);
        std::cout << "4NF Decomposition:\n";
        break;
    default:
        std::cout << "Invalid choice.\n";
        return;
    }
    std::cout << Util::printSet(result, " ", " ", "") << "\n";
}

int main() {
    Relation R("R");
    int option = 0;
    while (true) {
        std::cout << "\n--- Relation Normalizer Menu ---\n";
        std::cout << "Current Relation: " << R.getTitle() << "\n";
        std::cout << "1. Change relation title\n";
        std::cout << "2. Add attribute\n";
        std::cout << "3. Add FD\n";
        std::cout << "4. Add MVD\n";
        std::cout << "5. Show relation\n";
        std::cout << "6. Decompose (3NF/BCNF/4NF)\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose option: ";
        std::cin >> option;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch (option) {
        case 1: changeTitleMenu(R); break;
        case 2: addAttributeMenu(R); break;
        case 3: addFDMenu(R); break;
        case 4: addMVDMenu(R); break;
        case 5: std::cout << R << "\n"; break;
        case 6: decomposeMenu(R); break;
        case 0: return 0;
        default: std::cout << "Invalid option.\n";
        }
    }
}