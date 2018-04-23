#pragma once
#include <glm\glm.hpp>
#include <DNA.h>

class Rockets
{
public:
	Rockets(DNA<glm::vec2>* dna);
	~Rockets();

	void Update();
	void Draw();
	void applyForce(glm::vec2 force);


private:
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	bool completed;
	bool crashed;
	DNA<glm::vec2>* dna;
	float fitness;
};

