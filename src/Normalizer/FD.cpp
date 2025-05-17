#include "Normalizer/FD.h"
#include "Utility.h"
#include <algorithm>
#include <sstream>
#include <vector>

namespace Normalizer
{
    FD FD::operator+ (const FD& that) const
    {
        if (left != that.getLeft()) return *this;
        FD out;
        for (auto& item : left)
            out.AddToLeft(item);
        std::set<Attribute*> concat;
        std::set_union (
            right.begin(), right.end(),
            that.getRight().begin(), that.getRight().end(),
            std::inserter(concat, concat.begin())
        );
        for (auto& item : concat)
            out.AddToRight(item);
        return out;
    } //concatenate both `right`s if (this->left == that.getLeft())

    bool FD::operator== (const FD& that) const
    {
        return left == that.getLeft() && right == that.getRight();
    }
    bool FD::operator< (const FD& that) const
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

    std::string FD::display() const
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


    bool FD::isTrivial () const
    {
        return Util::isSubset(right, left);
    }
    bool FD::isSimplifiable () const
    {
        std::set<Attribute*> intersection;
        std::set_intersection (
            right.begin(), right.end(),
            left.begin(), left.end(),
            std::inserter(intersection, intersection.begin())
        );
        return !intersection.empty();
    }
    bool FD::simplify ()
    {
        std::set<Attribute*> intersection;
        std::set_intersection (
            right.begin(), right.end(),
            left.begin(), left.end(),
            std::inserter(intersection, intersection.begin())
        );

        if (intersection.empty()) return false;
        
        for (auto& item : intersection)
            right.erase(item);
        return true;
    } //remove attributes from right that are also in left
}