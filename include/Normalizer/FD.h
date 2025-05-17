#pragma once

#include "MVD.h"

namespace Normalizer
{
    class FD : public MVD
    {
        public:
            FD operator+ (const FD& that) const; //concatenate both `right`s if (this->left == that.getLeft())

            bool operator== (const FD& that) const;
            bool operator< (const FD& that) const;

            std::string display() const;

            bool isTrivial () const;
            bool isSimplifiable () const;
            bool simplify (); //remove attributes from right that are also in left
    };
}