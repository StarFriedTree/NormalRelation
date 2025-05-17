#pragma once

#include <string>

namespace Normalizer
{
    class Attribute
    {      
        public:
            explicit Attribute (const std::string& name) : name(name) {}

            const std::string& getName () const
            {
                return name;
            }

            bool operator== (const Attribute& that) const
            {
                return name == that.getName();
            }

            bool operator< (const Attribute& that) const
            {
                return name < that.getName();
            }
            bool operator> (const Attribute& that) const
            {
                return name > that.getName();
            }

        private:
            std::string name;
    };

    inline std::ostream& operator<<(std::ostream& os, const Normalizer::Attribute& attr)
    {
        os << attr.getName();
        return os;
    }
}