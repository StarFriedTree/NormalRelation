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
                    for (const auto* ptr : fd.getRight()) {
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
                    for (const auto* ptr : fd.getRight()) {
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


}