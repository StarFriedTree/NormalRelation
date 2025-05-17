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

			std::ostream& operator<< (std::ostream& os) const
			{
				os << name;
				return os;
			}

        private:
            std::string name;
    };
}