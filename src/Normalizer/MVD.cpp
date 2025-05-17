#include "Normalizer/MVD.h"
#include <algorithm>
#include "Utility.h"
#include <sstream>
#include <vector>

namespace Normalizer
{
    const std::set<Attribute*> MVD::getAllAttributes() const
    {
        std::set<Attribute*> out;
        std::set_union (
            left.begin(), left.end(),
            right.begin(), right.end(),
            std::inserter(out, out.begin())
        );
        return out;
    }

    bool MVD::AddToLeft (Attribute* attrib)
    {
        if (inLeft(attrib)) return false;
        left.insert(attrib);
        return true;
    }
    bool MVD::AddToRight (Attribute* attrib)
    {
        if (inRight(attrib)) return false;
        right.insert(attrib);
        return true;
    }

    bool MVD::RemoveFromLeft (Attribute* attrib)
    {
        auto i = left.find(attrib);
        if (i == left.end()) return false;
        left.erase(i);
        return true;
    }
    bool MVD::RemoveFromRight (Attribute* attrib)
    {
        auto i = right.find(attrib);
        if (i == right.end()) return false;
        right.erase(i);
        return true;
    }

    bool MVD::inLeft (Attribute* attrib) const
    {
        if (left.find(attrib) != left.end()) return true;
        for (const auto& a : left)
            if (*a == *attrib) return true;
        return false;
    }
    bool MVD::inRight (Attribute* attrib) const
    {
        if (right.find(attrib) != right.end()) return true;
        for (const auto& a : right)
            if (*a == *attrib) return true;
        return false;
    }
    
    bool MVD::containsAttribute (Attribute* attrib) const
    {
        return inLeft (attrib) || inRight(attrib);
    }

    bool MVD::operator== (const MVD& that) const
    {
        return (left == that.getLeft() && right == that.getRight());
    }
    bool MVD::operator< (const MVD& that) const
    {

        auto nameSeq = [](const auto& container) {
            std::vector<std::string> names;
            for (const auto* attr : container)
                names.push_back(attr->getName());
            return names;
        };

        auto leftNames = nameSeq(left);
        auto thatLeftNames = nameSeq(that.left);
        if (leftNames < thatLeftNames)
            return true;
        if (thatLeftNames < leftNames)
            return false;

        auto rightNames = nameSeq(right);
        auto thatRightNames = nameSeq(that.right);
        return rightNames < thatRightNames;
    }

    bool MVD::isLeftSubset (const std::set<Attribute*>& attribs) const
    {
        return Util::isSubset (attribs, left);
    }
    bool MVD::isRightSubset (const std::set<Attribute*>& attribs) const
    {
        return Util::isSubset (attribs, right);
    }

    std::string MVD::display() const
    {
        std::stringstream out;

        auto printPtrSet = [](const std::set<Attribute*>& s) {
            std::stringstream ss;
            bool first = true;
            for (const auto* attr : s) {
                if (!first) ss << ", ";
                if (attr) ss << attr->getName();
                first = false;
            }
            return ss.str();
            };

        out << printPtrSet(left) << " --> " << printPtrSet(right);
        return out.str();
    }

}