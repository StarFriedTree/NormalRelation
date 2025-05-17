#include "Normalizer/Normalize.h"
#include "Utility.h"
#include <algorithm>


namespace Normalizer
{
    std::set<Attribute*> findClosure(std::set<Attribute*> A, const std::set<FD>& S)
    {
        bool changed = true;
        while (changed)
        {
            changed = false;

            std::set<Attribute> valueSet;
            for (const auto* a : A)
                valueSet.insert(*a);

            for (const auto& fd : S)
            {
                if (Util::ptrSetIsSubsetByValue(fd.getLeft(), A) && !Util::ptrSetIsSubsetByValue(fd.getRight(), A))
                {
                    for (auto* ptr : fd.getRight()) {
                        if (valueSet.find(*ptr) == valueSet.end()) {
                            A.insert(ptr);
                            valueSet.insert(*ptr); // Keep valueSet in sync
                        }
                    }
                    changed = true;
                }
            }
        }
        return A;
    }

    //overloaded func to ignore an fd
    std::set<Attribute*> findClosure(std::set<Attribute*> A, const std::set<FD>& S, std::set<FD>::const_iterator ignore)
    {
        bool changed = true;
        while (changed)
        {
            changed = false;
            std::set<Attribute> valueSet;
            for (const auto* a : A)
                valueSet.insert(*a);

            for (auto it = S.begin(); it != S.end(); ++it)
            {
                if (it == ignore)
                    continue;
                const auto& fd = *it;
                if (Util::ptrSetIsSubsetByValue(fd.getLeft(), A) && !Util::ptrSetIsSubsetByValue(fd.getRight(), A))
                {
                    for (auto* ptr : fd.getRight()) {
                        if (valueSet.find(*ptr) == valueSet.end()) {
                            A.insert(ptr);
                            valueSet.insert(*ptr);
                        }
                    }
                    changed = true;
                }
            }
        }
        return A;
    }

    void projectFDs(const Relation& R, Relation& R1)
    {
        if (!R1.isProjection(R))
            return;
        if (!R1.getFDs().empty())
            R1.clearFDs();

        
        std::set<std::set<Attribute*>> powerset = Util::getPowerset(R1.getAttributePtrs());

        for (const auto& subset : powerset)
        {
            std::set<Attribute*> closure = findClosure(subset, R.getFDs());

            // For each attribute in closure but not in left, add FD: left -> {right}
            for (auto* right : closure)
            {
                bool inLeft = false;
                for (auto* l : subset)
                    if (*l == *right)
                        inLeft = true;
                if (!inLeft)
                {
                    FD fd;
                    for (auto* l : subset)
                        fd.AddToLeft(l);
                    fd.AddToRight(right);
                    R1.addFD(fd);
                }
            }
        }

        R1.minimalBasisFDs();
    }

    bool FDfollows(const std::set<FD>& set, std::set<FD>::const_iterator fd)
    {
        auto goal = findClosure(fd->getLeft(), set);

        auto match = findClosure(fd->getLeft(), set, fd);

        return match == goal;
    }

    bool FDfollows(const std::set<FD>& set, const FD& reducedFD)
    {
        auto match = findClosure(reducedFD.getLeft(), set);

        if (!reducedFD.isRightSubset(match)) return false;

        return true;
    }

}