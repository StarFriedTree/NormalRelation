#pragma once

#include "FD.h"

namespace Normalizer
{
    class Relation 
    {
        public:    
            Relation (std::string title) : Title(title) {}
            Relation (const Relation& copy);

            const std::string& getTitle () const { return Title; }
			void setTitle (const std::string& title) { Title = title; }
            const std::set<Attribute>& getAttributes () const {return attributes; }
            const std::set<Attribute*> getAttributePtrs () const;

            const std::set<FD>& getFDs () const { return FDs; }
            const std::set<MVD>& getMVDs () const { return MVDs; }
            
            bool addAttribute (const Attribute& attrib);
            bool addFD (const FD& fd);
            bool addMVD (const MVD& mvd);

            bool removeAttribute (const Attribute& attrib);
            bool removeFD (const FD& fd);
            bool removeMVD (const MVD& mvd);
            bool clearFDs ();
            bool clearMVDs ();

            bool searchAttribute (const Attribute& attrib) const;
            bool searchAttribute (const Attribute* attrib) const;
            bool searchAttribute (const std::string& name) const;

            bool searchFD (const FD& fd) const;
            bool searchMVD (const MVD& mvd) const;

            std::string display () const;

            bool isProjection (const Relation& parent) const;
            bool isProjection (const std::set<Attribute>& attribs) const;

            bool isValidFD (const FD& fd) const;
            bool isValidateableFD (const FD& fd) const;
            bool isValidMVD (const MVD& mvd) const;
            bool isValidateableMVD (const MVD& mvd) const;

            FD validateFD (const FD& fd) const;
            MVD validateMVD (const MVD& mvd) const;

            void minimalBasisFDs ();
            void minimalBasisMVDs ();

            bool isGoodFD (std::set<FD>::const_iterator fd) const;
            bool isKey (std::set<Attribute*> attribs) const;
			bool isKeybyValues(std::set<Attribute*> attribs) const;
            std::set<FD>::const_iterator findBadFD () const;
            std::set<MVD>::const_iterator findBadMVD() const;

            void promoteFDsToMVDs ();
            void SingletonRightMVDs ();

            bool operator < (const Relation& that) const;

        private:
            std::string Title;
            std::set<Attribute> attributes;
            std::set<MVD> MVDs;
            std::set<FD> FDs;
    };

    std::ostream& operator<< (std::ostream& os, const Relation& R);
}