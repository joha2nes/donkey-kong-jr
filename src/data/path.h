#pragma once

#include <vector>
#include "glm/glm.hpp"

struct Path
{
	struct Node
	{
		glm::vec2 pos;
		std::vector<int> links;
	};

	std::vector<Node> nodes;

	void Add(glm::vec2 pos);
	void Connect(int fromIndex, int toIndex);
};
