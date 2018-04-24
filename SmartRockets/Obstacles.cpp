#include "Obstacles.h"


Obstacles::Obstacles(glm::vec2 position, glm::vec2 extents)
{
	this->position = position;
	this->extents = extents;
	this->min = this->position - glm::vec2(extents.x / 2, extents.y);
	this->max = this->position + glm::vec2(extents.x / 2, extents.y);
}


Obstacles::~Obstacles()
{
}

void Obstacles::Draw(aie::Renderer2D* renderer)
{
	renderer->drawBox(position.x, position.y, extents.x, extents.y);
}
