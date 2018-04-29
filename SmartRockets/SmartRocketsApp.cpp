#include "SmartRocketsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm\gtc\random.hpp>
#include <string>
#include <iostream>

SmartRocketsApp::SmartRocketsApp() {

}

SmartRocketsApp::~SmartRocketsApp() {

}

bool SmartRocketsApp::startup() {

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	std::function<glm::vec2()> f = std::bind(&SmartRocketsApp::GetRandomVector2, this);
	std::function<float(int)> t = std::bind(&SmartRocketsApp::GetFitness, this, std::placeholders::_1);

	srand(time(NULL));
	PopulationSize = 100;
	bestGen = 1;

	WindowX = Application::getWindowWidth();
	WindowY = Application::getWindowHeight();

	m_EndGoal = glm::vec2(WindowX / 2, WindowY - 100);
	m_obstacle1 = glm::vec2(WindowX / 2, WindowY / 2);
	m_obstacles[0] = new Obstacles(glm::vec2(WindowX / 2, WindowY / 2), glm::vec2(800, 5));
	m_obstacles[1] = new Obstacles(glm::vec2(WindowX / 2, WindowY - 120), glm::vec2(100, 5));

	m_GA = new GeneticAlgorithm<glm::vec2>(100, 250, f, t, 2.0f);
	totalMembers = 0;
	totalMembers += PopulationSize;

	membersCompleted = 0;

	for (int i = 0; i < PopulationSize; i++)
	{
		auto iter = m_GA->Population.begin();
		std::advance(iter, i);
		DNA<glm::vec2>* genes = (*iter);

		m_Population[i] = new Rocket(genes);
	}

	return true;
}

void SmartRocketsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
	delete m_GA;
	for (int i = 0; i < PopulationSize; i++)
	{
		delete m_Population[i];
	}
	for (int i = 0; i < 2; i++)
	{
		delete m_obstacles[i];
	}
}

void SmartRocketsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();
	int popCount = 0;

	for (int i = 0; i < PopulationSize; i++)
	{
		m_Population[i]->Update(deltaTime);
		if (m_Population[i]->reachedEnd || m_Population[i]->crashed)
		{
			popCount++;
		}
	}

	if (popCount == PopulationSize)
	{
		m_GA->NewGeneration();
		totalMembers += PopulationSize;
		for (int i = 0; i < PopulationSize; i++)
		{
			delete m_Population[i];
		}
		for (int i = 0; i < PopulationSize; i++)
		{
			auto iter = m_GA->Population.begin();
			std::advance(iter, i);
			DNA<glm::vec2>* genes = (*iter);
			m_Population[i] = new Rocket(genes);
		}
	}

	CheckCollisions();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void SmartRocketsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	for (int i = 0; i < PopulationSize; i++)
	{
		if (m_Population[i]->crashed)
		{
			m_2dRenderer->setRenderColour(255, 0, 0);
			m_2dRenderer->drawCircle(m_Population[i]->position.x, m_Population[i]->position.y, 5, 1.0f);
			m_2dRenderer->setRenderColour(255, 255, 255);
		}
		else if (m_Population[i]->completed)
		{
			m_2dRenderer->setRenderColour(0, 255, 0);
			m_2dRenderer->drawCircle(m_Population[i]->position.x, m_Population[i]->position.y, 5, 1.0f);
			m_2dRenderer->setRenderColour(255, 255, 255);
		}
		else
		{
			m_2dRenderer->setRenderColour(255, 255, 255);
			m_2dRenderer->drawCircle(m_Population[i]->position.x, m_Population[i]->position.y, 5, 1.0f);
			m_2dRenderer->setRenderColour(255, 255, 255);
		}
	}

	m_2dRenderer->drawBox(m_EndGoal.x, m_EndGoal.y, 20, 20);

	for (int i = 0; i < 2; i++)
	{
		m_obstacles[i]->Draw(m_2dRenderer);
	}

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	bestGen = std::to_string(m_GA->generationNumber);
	m_2dRenderer->drawText(m_font, bestGen.c_str(), 0, getWindowHeight() - 25);

	//draw best fitness
	std::string bestFitness = std::to_string(m_GA->bestFitness);
	m_2dRenderer->drawText(m_font, bestFitness.c_str(), 0, getWindowHeight() - 50);

	//draw total members and members completed;
	std::string totalMembersstr = std::to_string(totalMembers);
	m_2dRenderer->drawText(m_font, "Total Members: ", 0, getWindowHeight() - 75);
	m_2dRenderer->drawText(m_font, totalMembersstr.c_str(), 250, getWindowHeight() - 75);

	//draw completed
	std::string completedMems = std::to_string(membersCompleted);
	m_2dRenderer->drawText(m_font, "Total completed: ", 0, getWindowHeight() - 100);
	m_2dRenderer->drawText(m_font, completedMems.c_str(), 285, getWindowHeight() - 100);

	////draw success percentage
	//double division = (membersCompleted / totalMembers);
	//double percent = division * 100;
	//std::string percentage = std::to_string(percent);
	//m_2dRenderer->drawText(m_font, "Success Percentage: ", 0, getWindowHeight() - 125);
	//m_2dRenderer->drawText(m_font, percentage.c_str(), 335, getWindowHeight() - 125);

	// done drawing sprites
	m_2dRenderer->end();
}

glm::vec2 SmartRocketsApp::GetRandomVector2()
{
	float first = (-1) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - (-1))));
	float second = (-1) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - (-1))));
	return glm::vec2(first, second);
}

float SmartRocketsApp::GetFitness(int index)
{
	glm::vec2 asd = m_EndGoal;
	m_Population[index]->positions.sort([asd](glm::vec2 lhs, glm::vec2 rhs) {return glm::distance(asd, rhs) < glm::distance(asd, lhs); });

	float d = glm::distance(*(m_Population[index]->positions.begin()), m_EndGoal);

	//store positions constantly, base score off closest position to end

	float fitness = 1 / d;

	if (m_Population[index]->completed)
		d -= m_Population[index]->duration *= 1.5f;

	if (m_Population[index]->position.y > m_EndGoal.y)
	{
		fitness *= 5;
	}
	else
	{
		fitness /= 5;
	}

	if (m_Population[index]->crashed)
	{
		fitness /= 10;
	}
	else
	{
		fitness *= 10;
	}
	if (m_Population[index]->completed)
	{
		fitness *= 25;
		membersCompleted++;
	}
	return fitness;
}

void SmartRocketsApp::CheckCollisions()
{
	for (int i = 0; i < PopulationSize; i++)
	{
		glm::vec2 min1 = glm::vec2(m_Population[i]->position.x - 5.0f, m_Population[i]->position.y - 5.0f);
		glm::vec2 max1 = glm::vec2(m_Population[i]->position.x + 5.0f, m_Population[i]->position.y + 5.0f);

		glm::vec2 endMin = m_EndGoal - glm::vec2(10.0f, 10.0f);
		glm::vec2 endMax = m_EndGoal + glm::vec2(10.0f, 10.0f);

		for (int j = 0; j < 2; j++)
		{
			bool collision = (max1.x < m_obstacles[j]->min.x || m_obstacles[j]->max.x < min1.x || max1.y < m_obstacles[j]->min.y || m_obstacles[j]->max.y < min1.y);
			if (!collision)
				m_Population[i]->setCrashed();
		}


		if (m_Population[i]->position.x > WindowX || m_Population[i]->position.x < 0
			|| m_Population[i]->position.y > WindowY || m_Population[i]->position.y < 0)
		{
			m_Population[i]->setCrashed();
		}

		bool collisionEnd = (max1.x < endMin.x || endMax.x < min1.x || max1.y < endMin.y || endMax.y < min1.y);
		if (!collisionEnd)
		{
			m_Population[i]->completed = true;
		}
	}
}