#pragma once

#include <set>
#include <sstream>
#include <algorithm>
#include <vector>

namespace Util
{
    template <typename T>
    bool isSuperset (const std::set<T>& A, const std::set<T>& B)
    {
        return std::includes(
            A.begin(), A.end(),
            B.begin(), B.end()
        );
    }

    template <typename T>
    bool isSubset (const std::set<T>& A, const std::set<T>& B)
    {
        return std::includes(
            B.begin(), B.end(),
            A.begin(), A.end()
        );
    }

    template <typename T>
    std::set<T> setUnion(const std::set<T>& a, const std::set<T>& b)
    {
        std::set<T> result;
        std::set_union(
            a.begin(), a.end(),
            b.begin(), b.end(),
            std::inserter(result, result.begin())
        );
        return result;
    }

    template <typename T>
    std::set<T> setIntersection(const std::set<T>& a, const std::set<T>& b)
    {
        std::set<T> result;
        std::set_intersection(
            a.begin(), a.end(),
            b.begin(), b.end(),
            std::inserter(result, result.begin())
        );
        return result;
    }

    template <typename T>
    std::set<T> setDifference(const std::set<T>& a, const std::set<T>& b)
    {
        std::set<T> result;
        std::set_difference(
            a.begin(), a.end(),
            b.begin(), b.end(),
            std::inserter(result, result.begin())
        );
        return result;
    }

    // Checks if all *ptr values in A are present in B
    template <typename T>
    bool ptrSetIsSubsetByValue(const std::set<T*>& A, const std::set<T*>& B)
    {
        // Build a set of values from B
        std::set<T> bValues;
        for (const auto* b : B) {
            if (b) bValues.insert(*b);
        }
        
        for (const auto* a : A) {
            if (!a || bValues.find(*a) == bValues.end())
                return false;
        }
        return true;
    }


    template <typename T>
    std::set<std::set<T>> getPowerset (const std::set<T>& set)
    {
        std::vector<std::set<T>> powerset = { std::set<T>() };
		powerset.reserve(1ULL << set.size()); // 2^n

        for (const auto& item : set)
        {
            size_t n = powerset.size();
            for (size_t i = 0; i < n; ++i)
            {
                auto subset = powerset[i];
                subset.insert(item);
                powerset.push_back(std::move(subset));
            }
        }
        return std::set<std::set<T>>(powerset.begin(), powerset.end());
    }

    template <class T>
    std::string printSet (  const std::set<T>& set, 
                            std::string start = " (", std::string end = ") ",
                            std::string delim = ", "
                         )
    {
        std::stringstream out;
        out << start;
        for (auto i = set.begin(); i != set.end(); i++)
        {
            out << *i;
            if (std::next(i) != set.end())
                out << delim;
        }
        out << end;
        return out.str();
    }

}