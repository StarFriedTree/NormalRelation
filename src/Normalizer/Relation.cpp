#include "Normalizer/Relation.h"

namespace Normalizer
{
    const std::set<const Attribute*> Relation::getAttributePtrs () const
    {
        std::set<const Attribute*> attribPtrs;
        for (auto& attrib : attributes)
            attribPtrs.insert(&attrib);
        return attribPtrs;
    }

    bool Relation::addAttribute (const Attribute& attrib)
    {
        if (searchAttribute(attrib)) return false;
        if (searchAttribute(attrib.getName())) return false;

        attributes.insert(attrib);
        return true;
    }
    // bool Relation::addFD (const FD& fd)
    // {

    // }
    // bool Relation::addMVD (const MVD& mvd)
    // {

    // }

    bool Relation::removeAttribute (const Attribute& attrib)
    {
        if (!searchAttribute(attrib)) return false;
        attributes.erase(attrib);
        return true;
    }
    bool Relation::removeFD (const FD& fd)
    {
        if (!searchFD(fd)) return false;
        FDs.erase(fd);
        return true;
    }
    bool Relation::removeMVD (const MVD& mvd)
    {
        if (!searchMVD(mvd)) return false;
        MVDs.erase(mvd);
        return true;
    }
    bool Relation::clearFDs ()
    {
        FDs.clear();
        return true;
    }
    bool Relation::clearMVDs ()
    {
        MVDs.clear();
        return true;
    }

    bool Relation::searchAttribute (const Attribute& attrib) const
    {
        return (attributes.find(attrib) != attributes.end());
    }
    bool Relation::searchAttribute (const Attribute* attrib) const
    {
        return (attributes.find(*attrib) != attributes.end());
    }
    bool Relation::searchAttribute (const std::string& name) const
    {
        Attribute check (name);
        return (attributes.find(name) != attributes.end());
    }

    // bool Relation::searchFD (const FD& fd) const
    // {

    // }
    // bool Relation::searchMVD (const MVD& mvd) const
    // {

    // }

    // std::string Relation::display () const
    // {

    // }

    // bool Relation::isProjection (const Relation& parent) const
    // {

    // }
    // bool Relation::isProjection (const std::set<Attribute>& attribs) const
    // {

    // }

    // bool Relation::isValidFD (const FD& fd) const
    // {

    // }
    // bool Relation::isValidateableFD (const FD& fd) const
    // {

    // }
    // bool Relation::isValidMVD (const MVD& mvd) const
    // {

    // }
    // bool Relation::isValidateableMVD (const MVD& mvd) const
    // {

    // }

    // FD Relation::validateFD (const FD& fd) const
    // {

    // }
    // FD Relation::validateMVD (const MVD& mvd) const
    // {

    // }
}