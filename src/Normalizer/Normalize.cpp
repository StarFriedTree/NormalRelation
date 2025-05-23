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

    std::set<Attribute*> findKey(const Relation& R)
    {
        auto cmp = [](const std::set<Attribute*>& lhs, const std::set<Attribute*>& rhs) 
        {
            return lhs.size() < rhs.size();
	    };

        std::set<std::set<Attribute*>, decltype(cmp)> combinations;

        auto powerset = Util::getPowerset(R.getAttributePtrs());
        for (auto& set : powerset)
            combinations.insert(set);

        int maxSize = 0;
		auto iter = combinations.begin();
        for (auto i = combinations.begin(); i != combinations.end(); i++)
        {
            auto size = findClosure(*i, R.getFDs()).size();
            if (size > maxSize)
            {
                iter = i;
                maxSize = size;
                if (maxSize == R.getAttributes().size())
					return *i;
            }
		}
		return *iter; //theoretically this should never happen, but just in case
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
            FD fd;
			for (auto* l : subset)
				fd.AddToLeft(l);

            for (auto* right : closure)
                if (!fd.inLeft(right) && R1.searchAttribute(right))
                    fd.AddToRight(right);
            
			if (fd.getLeft().empty() || fd.getRight().empty() || fd.isTrivial())
				continue;
            R1.addFD(fd);
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

    std::set<Relation> BCNF (Relation& R)
    {
        std::set<Relation> output;

        R.minimalBasisFDs();

        auto badFD = R.findBadFD();
        if (badFD == R.getFDs().end())
        {
            output.insert(R);
            return output;
        }

        auto attrib1 = findClosure ( badFD->getLeft(), R.getFDs() );
        auto attrib2 = Util::setDifference ( R.getAttributePtrs(), attrib1 );
        attrib2.insert(badFD->getLeft().begin(), badFD->getLeft().end());

        Relation R1 (R.getTitle() + ".a");
        Relation R2 (R.getTitle() + ".b");

        for (auto attrib : attrib1)
            R1.addAttribute(*attrib);
        for (auto attrib : attrib2)
            R2.addAttribute(*attrib);

        projectFDs(R, R1);
        projectFDs(R, R2);

        auto newBad1 = R1.findBadFD();
        auto newBad2 = R2.findBadFD();

        if (newBad1 == R1.getFDs().end())
            output.insert(R1);
        else
        {
            auto branch1 = BCNF(R1);
            output.insert(branch1.begin(), branch1.end());
        }

        if (newBad2 == R2.getFDs().end())
            output.insert(R2);
        else
        {
            auto branch2 = BCNF(R2);
            output.insert(branch2.begin(), branch2.end());
        }

        return output;
    }

    std::set<Relation> threeNF(Relation& R)
    {
		R.minimalBasisFDs();
		std::set<std::set<Attribute*>> schemas;

        bool keyPresent = false;
        for (const auto& fd : R.getFDs())
        {
            auto schema = fd.getAllAttributes();
           
            if (R.isKey(schema))
				keyPresent = true;
			schemas.insert(schema);
        }

        if (!keyPresent)
            schemas.insert(findKey(R));

        for (auto i = schemas.begin(); i != schemas.end();)
        {
            bool remove = false;
            for (auto j = schemas.begin(); j != schemas.end(); j++)
            {
                if (i == j) continue;
                if (Util::isSubset(*i, *j))
                {
                    remove = true;
                    break;
				}
            }
            if (remove)
                i = schemas.erase(i);
            else
				++i;
		}

		std::set<Relation> output;
        int title = 1;
        for (const auto& schema : schemas)
        {
            Relation R1(R.getTitle() + '_' + std::to_string(title++));
            for (const auto& attrib : schema)
                R1.addAttribute(*attrib);
            projectFDs(R, R1);
            
            output.insert(R1);
		}
        return output;
    }
}