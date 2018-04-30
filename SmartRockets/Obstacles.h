#pragma once
#include <glm\glm.hpp>
#include <Renderer2D.h>

class Obstacles
{
public:
	Obstacles(glm::vec2 position, glm::vec2 extents);
	~Obstacles();
	void Draw(aie::Renderer2D* renderer);
	glm::vec2 position;
	glm::vec2 extents;
	glm::vec2 min;
	glm::vec2 max;

private:
};

