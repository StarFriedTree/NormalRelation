#include "Normalizer/Relation.h"
#include <sstream>
#include "Utility.h"
#include "Normalizer/Normalize.h"
#include <cassert>

namespace Normalizer
{
    Relation::Relation (const Relation& copy)
    {
        this->Title = copy.getTitle();
        this->attributes = copy.getAttributes();
		
        for (auto& fd : copy.getFDs())
            this->addFD(fd);
		for (auto& mvd : copy.getMVDs())
			this->addMVD(mvd);
    }

    const std::set<Attribute*> Relation::getAttributePtrs () const
    {
        std::set<Attribute*> attribPtrs;
        for (auto& attrib : attributes)
            attribPtrs.insert(const_cast<Attribute*>(&attrib));
        return attribPtrs;
    }

    bool Relation::addAttribute (const Attribute& attrib)
    {
        if (searchAttribute(attrib)) return false;
        if (searchAttribute(attrib.getName())) return false;

        attributes.insert(attrib);
        return true;
    }
    bool Relation::addFD (const FD& fd)
    {
        // Check if already valid for this relation
        if (isValidFD(fd)) 
        {
            if (FDs.find(fd) == FDs.end()) 
            {
                FDs.insert(fd);
                return true;
            }
            return false; // Already present
        }
        // If not valid, check if it is validateable
        if (isValidateableFD(fd)) 
        {
            FD validated = validateFD(fd);
            if (FDs.find(validated) == FDs.end()) 
            {
                FDs.insert(validated);
                return true;
            }
            return false; // Already present after validation
        }
        // Not valid or validateable
        return false;
    }
    bool Relation::addMVD (const MVD& mvd)
    {
        // Check if already valid for this relation
        if (isValidMVD(mvd)) 
        {
            if (MVDs.find(mvd) == MVDs.end()) 
            {
                MVDs.insert(mvd);
                return true;
            }
            return false; // Already present
        }
        // If not valid, check if it is validateable
        if (isValidateableMVD(mvd)) 
        {
            MVD validated = validateMVD(mvd);
            if (MVDs.find(validated) == MVDs.end()) 
            {
                MVDs.insert(validated);
                return true;
            }
            return false; // Already present after validation
        }
        // Not valid or validateable
        return false;
    }

    bool Relation::removeAttribute(const Attribute& attrib)
    {
        // Find the pointer to the attribute in this relation
        auto it = attributes.find(attrib);
        if (it == attributes.end())
            return false; // Attribute not found

        Attribute* attribPtr = const_cast<Attribute*>(&(*it));

        // Check all MVDs for presence of this pointer
        for (const auto& mvd : MVDs)
        {
            const auto& left = mvd.getLeft();
            const auto& right = mvd.getRight();
            if (left.find(attribPtr) != left.end() || right.find(attribPtr) != right.end())
                return false; // Attribute pointer is used in an MVD
        }

        // Check all FDs for presence of this pointer
        for (const auto& fd : FDs)
        {
            const auto& left = fd.getLeft();
            const auto& right = fd.getRight();
            if (left.find(attribPtr) != left.end() || right.find(attribPtr) != right.end())
                return false; // Attribute pointer is used in an FD
        }

        // Safe to remove
        attributes.erase(it);
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
        return (attributes.find(check) != attributes.end());
    }

    bool Relation::searchFD(const FD& fd) const
    {
        return FDs.find(fd) != FDs.end();
    }

    bool Relation::searchMVD(const MVD& mvd) const
    {
        return MVDs.find(mvd) != MVDs.end();
    }


    std::string Relation::display() const
    {
        std::stringstream out;
        out << Title << Util::printSet(attributes) << "\n\n";
        for (auto i = FDs.begin(); i != FDs.end(); i++)
        {
            out << '\t' << (*i).display() << '\n';
        }
		out << '\n';
		for (auto i = MVDs.begin(); i != MVDs.end(); i++)
		{
			out << '\t' << (*i).display() << '\n';
		}
        return out.str();
    }

	std::ostream& operator<< (std::ostream& os, const Relation& R)
	{
		os << R.display();
		return os;
	}

    bool Relation::isProjection(const Relation& parent) const
    {
        return Util::isSubset(attributes, parent.attributes);
    }


    bool Relation::isProjection (const std::set<Attribute>& attribs) const
    {
		return Util::isSubset(attributes, attribs);
    }

    bool Relation::isValidFD (const FD& fd) const
    {
        std::set<Attribute*> attribPtrs = getAttributePtrs();
        for (auto* attr : fd.getAllAttributes())
            if (attribPtrs.find(attr) == attribPtrs.end())
                return false;
        return true;
    }
    bool Relation::isValidateableFD (const FD& fd) const
    {
        for (const auto* attr : fd.getAllAttributes())
            if (attributes.find(*attr) == attributes.end())
                return false;
        return true;
    }
    bool Relation::isValidMVD (const MVD& mvd) const
    {
        std::set<Attribute*> attribPtrs = getAttributePtrs();
        for (auto* attr : mvd.getAllAttributes())
            if (attribPtrs.find(attr) == attribPtrs.end())
                return false;
        return true;
    }
    bool Relation::isValidateableMVD (const MVD& mvd) const
    {
        for (const auto* attr : mvd.getAllAttributes())
            if (attributes.find(*attr) == attributes.end())
                return false;
        return true;
    }

    FD Relation::validateFD (const FD& fd) const
    {
        FD result;
        // left
        for (const auto* attr : fd.getLeft()) {
            auto it = attributes.find(*attr);
            if (it != attributes.end())
                result.AddToLeft(const_cast<Attribute*>(&(*it)));
        }
        // right
        for (const auto* attr : fd.getRight()) {
            auto it = attributes.find(*attr);
            if (it != attributes.end())
                result.AddToRight(const_cast<Attribute*>(&(*it)));
        }
        return result;
    }
    MVD Relation::validateMVD (const MVD& mvd) const
    {
        MVD result;
        // left
        for (const auto* attr : mvd.getLeft()) {
            auto it = attributes.find(*attr);
            if (it != attributes.end())
                result.AddToLeft(const_cast<Attribute*>(&(*it)));
        }
        // right
        for (const auto* attr : mvd.getRight()) {
            auto it = attributes.find(*attr);
            if (it != attributes.end())
                result.AddToRight(const_cast<Attribute*>(&(*it)));
        }
        return result;
    }

    void Relation::minimalBasisFDs ()
    {
        // 1. Remove trivial and simplify FDs
        for (auto it = FDs.begin(); it != FDs.end(); )
        {
            FD fd = *it;

            if (fd.isTrivial()) 
            {
                it = FDs.erase(it);
                continue;
            }
            else if (fd.isSimplifiable()) 
            {
                fd.simplify();
                if (!(fd == *it)) 
                {
                    it = FDs.erase(it);
                    addFD(fd);
                    continue;
                }
            }

            ++it;
        }

        // 2. Remove redundant FDs (those that follow from the rest)
        for (auto it = FDs.begin(); it != FDs.end(); )
        {
            if (FDfollows(FDs, it)) {
                it = FDs.erase(it);
            }
            else {
                ++it;
            }
        }

        // 3. Minimize left sides
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto it = FDs.begin(); it != FDs.end(); )
            {
                FD fd = *it;
                
                if (fd.getLeft().size() < 2) {
                    ++it;
                    continue;
                }

                bool reduced = false;
                for (auto l = fd.getLeft().begin(); l != fd.getLeft().end(); ++l)
                {
                    FD reducedFD = fd;
                    reducedFD.RemoveFromLeft(*l);

                    if (FDfollows(FDs, reducedFD)) {
                        // Replace with reduced FD
                        it = FDs.erase(it);
                        addFD(reducedFD);
                        changed = true;
                        reduced = true;
                        break;
                    }
                }
                if (!reduced)
                    ++it;
            }
        }
    }

    bool Relation::isGoodFD(std::set<FD>::const_iterator fd) const
    {
        auto closure = findClosure(fd->getLeft(), FDs);

        return closure == getAttributePtrs();

    }
    
    std::set<FD>::const_iterator Relation::findBadFD() const
    {
        for (auto i = FDs.begin(); i != FDs.end(); i++)
            if (!isGoodFD(i))
                return i;
        return FDs.end();
    }

    std::set<MVD>::const_iterator Relation::findBadMVD() const
    {
        for (auto i = MVDs.begin(); i != MVDs.end(); i++)
            if (!isKey(i->getLeft()))
                return i;
        return MVDs.end();
    }

    bool Relation::operator < (const Relation& that) const
    {
        if (this->Title < that.getTitle())
            return true;
        if (this->Title > that.getTitle())
            return false;
        if (attributes < that.attributes)
            return true;
        if (that.attributes < attributes)
            return false;
        if (FDs < that.FDs)
            return true;
        if (that.FDs < FDs)
            return false;
        return MVDs < that.MVDs;
    }

    bool Relation::isKey(std::set<Attribute*> attribs) const
    {
		auto closure = findClosure(attribs, FDs);
        return closure == getAttributePtrs();
    }
    bool Relation::isKeybyValues(std::set<Attribute*> attribs) const
    {
		auto closure = findClosure(attribs, FDs);
		return Util::ptrSetIsSubsetByValue(getAttributePtrs(), closure) && closure.size() == attributes.size();
    }


    void Relation::promoteFDsToMVDs()
    {
		minimalBasisFDs();

        auto allAttribs = getAttributePtrs();

        for (auto& fd : FDs)
            if (!isKey(fd.getLeft()) && !fd.isTrivialMVD(allAttribs))
            {
                MVD mvd;
                for (auto& attrib : fd.getLeft())
                    mvd.AddToLeft(attrib);
                for (auto& attrib : fd.getRight())
                    mvd.AddToRight(attrib);
                if (!MVDs.contains(mvd))
                    addMVD(mvd);
            }
		
    }

    void Relation::SingletonRightMVDs()
    {
        std::set<MVD> singletonRHS;
        for (auto& mvd : MVDs)
        {
            MVD temp;
            for (const auto& attrib : mvd.getLeft())
                temp.AddToLeft(attrib);
            for (const auto& attrib : mvd.getRight())
            {
                if (temp.getLeft().contains(attrib)) continue;
                temp.AddToRight(attrib);
                assert (temp.getRight().size() == 1);
                
                singletonRHS.insert(temp);
                temp.RemoveFromRight(attrib);
            }
        }
        clearMVDs();
        MVDs.insert(singletonRHS.begin(), singletonRHS.end());
    }

    void Relation::minimalBasisMVDs ()
    {
		auto allAttribs = getAttributePtrs();
        minimalBasisFDs();
		promoteFDsToMVDs();
		SingletonRightMVDs();

        // 1. Remove trivial MVDs
        for (auto i = MVDs.begin(); i != MVDs.end(); )
        {
            if (i->isTrivialMVD(allAttribs))
                i = MVDs.erase(i);
            else
                i++;
        }

		// 2. Remove implied MVDs (those that follow from the rest)
        for (auto it = MVDs.begin(); it != MVDs.end(); )
        {
            if (isMVDImplied(*this, it))
                it = MVDs.erase(it);
            else
                it++;
        }

		// 3. Minimize left sides
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto it = MVDs.begin(); it != MVDs.end(); )
            {
                MVD mvd = *it;
                if (mvd.getLeft().size() < 2) {
                    ++it;
                    continue;
                }
                bool reduced = false;
                for (auto l = mvd.getLeft().begin(); l != mvd.getLeft().end(); ++l)
                {
                    MVD reducedMVD = mvd;
                    reducedMVD.RemoveFromLeft(*l);
                    if (isMVDImplied(*this, reducedMVD)) {
                        // Replace with reduced MVD
                        it = MVDs.erase(it);
                        addMVD(reducedMVD);
                        changed = true;
                        reduced = true;
                        break;
                    }
                }
                if (!reduced)
                    ++it;
            }
		}
    }
}