#include "Rockets.h"



Rockets::Rockets(DNA<glm::vec2>* dna)
{
	this->dna = dna;
	this->position = glm::vec2(250, 0);
	this->velocity = glm::vec2(0, 0);
	this->acceleration = glm::vec2(0, 0);
	this->completed = false;
	this->crashed = false;
	this->fitness = 0.0f;
}


Rockets::~Rockets()
{
}

void Rockets::Update()
{




	//applyForce(dna->Genes)
	//
	//if (!completed && !crashed)
	//{
	//	velocity += acceleration;
	//	position += velocity;
	//	acceleration *= 0;
	//	velocity
	//}
}

void Rockets::Draw()
{
	
}

void Rockets::applyForce(glm::vec2 force)
{
	this->acceleration += force;
}
