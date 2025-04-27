#pragma once

#include "Attribute.h"
#include <set>

namespace Normalizer
{
    class MVD
    {
        public:
            MVD () {};

            const std::set<Attribute*>& getLeft () const { return left; }
            const std::set<Attribute*>& getRight () const { return right; }
            const std::set<Attribute*> getAllAttributes() const;

            bool AddToLeft (Attribute* attrib);
            bool AddToRight (Attribute* attrib);

            bool RemoveFromLeft (Attribute* attrib);
            bool RemoveFromRight (Attribute* attrib);

            bool inLeft (Attribute* attrib) const;
            bool inRight (Attribute* attrib) const;
            
            bool containsAttribute (Attribute* attrib) const;

            bool operator== (const MVD& that) const;

            bool isLeftSubset (const std::set<Attribute*>& attribs) const;
            bool isRightSubset (const std::set<Attribute*>& attribs) const;

            std::string display() const;
        
        protected:
            std::set<Attribute*> left, right;
    };
}