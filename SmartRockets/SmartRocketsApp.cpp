#include "SmartRocketsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm\gtc\random.hpp>

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
	PopulationSize = 50;

	WindowX = Application::getWindowWidth();
	WindowY = Application::getWindowHeight();
	
	m_EndGoal = glm::vec2(WindowX / 2, WindowY - 100);
	m_obstacle1 = glm::vec2(WindowX / 2, WindowY / 2);

	m_GA = new GeneticAlgorithm<glm::vec2>(50, 250, f, t, 8.0f);
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
		m_2dRenderer->drawCircle(m_Population[i]->position.x, m_Population[i]->position.y, 5, 1.0f);
	}

	m_2dRenderer->drawBox(m_EndGoal.x, m_EndGoal.y, 5, 5);
	m_2dRenderer->drawBox(m_obstacle1.x, m_obstacle1.y, 500, 5);

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

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
	
	float d = glm::distance(m_Population[index]->position, m_EndGoal);
	
	//store positions constantly, base score off closest position to end

	float fitness = -d;
	d += m_Population[index]->duration * 5.0f;
	if (m_Population[index]->crashed)
	{
		fitness -= 5000;
	}
	if (m_Population[index]->completed)
	{
		fitness += 1000;
	}
	return fitness;
}

void SmartRocketsApp::CheckCollisions()
{
	for (int i = 0; i < PopulationSize; i++)
	{
		glm::vec2 min1 = glm::vec2(m_Population[i]->position.x - 5.0f, m_Population[i]->position.y - 5.0f);
		glm::vec2 max1 = glm::vec2(m_Population[i]->position.x + 5.0f, m_Population[i]->position.y + 5.0f);

		glm::vec2 min2 = m_obstacle1 - glm::vec2(500 / 2, 5);
		glm::vec2 max2 = m_obstacle1 + glm::vec2(500 / 2, 5);

		glm::vec2 endMin = m_EndGoal - glm::vec2(2.5f, 2.5f);
		glm::vec2 endMax = m_EndGoal + glm::vec2(2.5f, 2.5f);

		bool collision = (max1.x < min2.x || max2.x < min1.x || max1.y < min2.y || max2.y < min1.y);
		if (!collision)
			m_Population[i]->setCrashed();

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