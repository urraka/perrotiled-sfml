#pragma once

#include <string>

namespace hlp
{
	template <typename ContainerT> inline void tokenize(const std::string &str, ContainerT &tokens, const std::string &delimiters, const bool trimEmpty = false)
	{
		std::string::size_type pos, lastPos = 0;

		while (true)
		{
			pos = str.find_first_of(delimiters, lastPos);

			if (pos == std::string::npos)
			{
				pos = str.length();

				if (pos != lastPos || !trimEmpty)
					tokens.push_back(ContainerT::value_type(str.data() + lastPos, (ContainerT::value_type::size_type)pos - lastPos ));

				break;
			}
			else
			{
				if (pos != lastPos || !trimEmpty)
					tokens.push_back(ContainerT::value_type(str.data() + lastPos, (ContainerT::value_type::size_type)pos - lastPos ));
			}

			lastPos = pos + 1;
		}
	};

	template <typename T> inline std::string toString(const T& t)
	{
		std::ostringstream os;
		os << t;
		return os.str();
	}
}
