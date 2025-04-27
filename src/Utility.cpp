#include "Utility.h"
#include <algorithm>

namespace Util
{
    template <typename T>
    bool isSubset (const std::set<T>& A, const std::set<T>& B) 
    {
        return std::includes (
            A.begin(), A.end(),
            B.begin(), B.end()
        )
    }
    
    template <typename T>
    bool isSuperset (const std::set<T>& A, const std::set<T>& B)
    {
        return std::includes (
            B.begin(), B.end(),
            A.begin(), A.end()
        )
    }
    
    template <typename T>
    std::set<std::set<T>> getPowerset (const std::set<T>& set)
    {
        //initializing with 2^n space
        std::set<std::set<T>> powerset ( std::set<T>() );
        powerset.reserve(1ULL << set.size());

        for (const auto& item : set)
        {
            auto currentCopies = powerset;
            for (auto& copy : currentCopies)
            {
                copy.insert(item);
                powerset.insert(copy);
            }
        }
        return powerset;
    }

    template <class T>
    std::string printSet (const std::set<T>& set)
    {
        std::stringstream out;
        out << " {";
        for (auto i = set.begin(); i != set.end(); i++)
        {
            out << *i;
            if (std::next(i) != set.end()) 
                out << ", ";
        }
        out << "} "
        return out.str();
    }
}