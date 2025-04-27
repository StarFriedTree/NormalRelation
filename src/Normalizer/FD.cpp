#include "Normalizer/FD.h"
#include "Utility.h"
#include <algorithm>
#include <sstream>

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

    std::string FD::display() const
    {
        std::stringstream out;
        for (auto i = left.begin(); i != left.end(); i++)
        {
            out << (*i)->getName();
            if (std::next(i) != left.end())
                out << ", ";
        }
        out << " --> ";
        for (auto i = right.begin(); i != right.end(); i++)
        {
            out << (*i)->getName();
            if (std::next(i) != right.end())
                out << ", ";
        }
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
    void FD::simplify ()
    {
        std::set<Attribute*> intersection;
        std::set_intersection (
            right.begin(), right.end(),
            left.begin(), left.end(),
            std::inserter(intersection, intersection.begin())
        );
        
        for (auto& item : intersection)
            right.erase(item);
    } //remove attributes from right that are also in left
}