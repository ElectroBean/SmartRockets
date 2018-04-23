#pragma once
#include <glm\glm.hpp>
#include <DNA.h>
#include <list>

class Rocket
{
public:
	Rocket(DNA<glm::vec2>* dna);
	~Rocket();
	void Update(float deltaTime);
	void Draw();
	void applyForce(glm::vec2 force);
	glm::vec2 position;
	bool reachedEnd;
	void setCrashed() { crashed = true; }
	float duration;
	bool crashed;
	bool completed;
	std::list<glm::vec2> positions;


private:
	glm::vec2 velocity;
	glm::vec2 acceleration;
	DNA<glm::vec2>* dna;
	float fitness;
	float index;
};

