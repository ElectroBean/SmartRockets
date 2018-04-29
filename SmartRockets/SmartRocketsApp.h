#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "GeneticAlgorithm.h"
#include <glm\glm.hpp>
#include "Rocket.h"
#include <glm\gtc\random.hpp>
#include "Obstacles.h"

class SmartRocketsApp : public aie::Application {
public:

	SmartRocketsApp();
	virtual ~SmartRocketsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 GetRandomVector2();
	float GetFitness(int index);

	void CheckCollisions();

protected:

	GeneticAlgorithm<glm::vec2>*   m_GA;
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	int					PopulationSize;
	Rocket*				m_Population[100];
	glm::vec2			m_EndGoal;
	glm::vec2			m_obstacle1;

	Obstacles*			m_obstacles[4];

	float WindowX;
	float WindowY;
	std::string bestGen;
	int totalMembers;
	int membersCompleted;
};