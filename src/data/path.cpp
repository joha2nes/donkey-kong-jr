#include "path.h"

#include "../glm/vec2.hpp"

void Path::Add(glm::vec2 pos)
{
	nodes.push_back({ pos, {} });
}

void Path::Connect(int fromIndex, int toIndex)
{
	nodes[fromIndex].links.push_back(toIndex);
}