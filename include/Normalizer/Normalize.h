#pragma once

#include "Relation.h"

namespace Normalizer
{
    std::set<Attribute*> findClosure (std::set<Attribute*> A, const std::set<FD>& S);
    std::set<Attribute*> findClosure (std::set<Attribute*> A, const std::set<FD>& S, const std::set<FD>::const_iterator ignore);

    std::set<Attribute*> findKey (const Relation& R);

    void projectFDs (const Relation& R, Relation& R1);
    void projectMVDs(const Relation& R, Relation& R1);

    bool FDfollows (const std::set<FD>& set, std::set<FD>::const_iterator fd);
    bool FDfollows (const std::set<FD>& set, const FD& reducedFD);

    bool isMVDImplied(const Relation& R, const MVD& target);
	bool isMVDImplied(const Relation& R, std::set<MVD>::const_iterator ignore);

    std::set<Relation> BCNF (Relation& R);
	std::set<Relation> threeNF (Relation& R);
	std::set<Relation> fourNF(Relation& R);
}