#pragma once

#include "Relation.h"

namespace Normalizer
{
    std::set<Attribute*> findClosure (std::set<Attribute*> A, const std::set<FD>& S);
    std::set<Attribute*> findClosure (std::set<Attribute*> A, const std::set<FD>& S, const std::set<FD>::const_iterator ignore);

    void projectFDs (const Relation& R, Relation& R1);

    bool FDfollows (const std::set<FD>& set, std::set<FD>::const_iterator fd);
    bool FDfollows (const std::set<FD>& set, const FD& reducedFD);

    std::set<Relation> bcnf (Relation& R);
}