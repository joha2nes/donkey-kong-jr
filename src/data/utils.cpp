#include "utils.h"

#include "glm/glm.hpp"

namespace utils
{
	void utils::setScoreString(std::string& str, int value)
	{
		for (int i = 0; i < str.size(); i++)
		{
			auto n = (value % (int)glm::pow(10, i + 1)) / (int)glm::pow(10, i);
			auto j = str.size() - i - 1;
			str[j] = '0' + n;
		}
	}
}
