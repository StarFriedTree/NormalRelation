#pragma once

#include <set>
#include <sstream>

namespace Util
{
    template <typename T>
    bool isSubset (const std::set<T>& A, const std::set<T>& B);
    
    template <typename T>
    bool isSuperset (const std::set<T>& A, const std::set<T>& B);
    
    template <typename T>
    std::set<std::set<T>> getPowerset (const std::set<T>& set);

    template <class T>
    std::string printSet (const std::set<T>& set);
}