#pragma once

#include <set>
#include <sstream>
#include <algorithm>

namespace Util
{
    template <typename T>
    bool isSuperset(const std::set<T>& A, const std::set<T>& B)
    {
        return std::includes(
            A.begin(), A.end(),
            B.begin(), B.end()
        );
    }

    template <typename T>
    bool isSubset(const std::set<T>& A, const std::set<T>& B)
    {
        return std::includes(
            B.begin(), B.end(),
            A.begin(), A.end()
        );
    }

    template <typename T>
    std::set<std::set<T>> getPowerset(const std::set<T>& set)
    {
        std::vector<std::set<T>> powerset = { {} };
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
    std::string printSet(const std::set<T>& set)
    {
        std::stringstream out;
        out << " {";
        for (auto i = set.begin(); i != set.end(); i++)
        {
            out << *i;
            if (std::next(i) != set.end())
                out << ", ";
        }
        out << "} ";
        return out.str();
    }
}