#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {

	m_2dRenderer = new aie::Renderer2D();

	//doing fancy stuff that i honestly dont understand 100%
	//to make the GA accept member functions instead of static functions
	std::function<char()> f = std::bind(&Application2D::getRandomGene, this);
	std::function<float(int)> t = std::bind(&Application2D::fitness, this, std::placeholders::_1);

	validChars = new std::string("abcdefghijklmnopqrstuvwxyz,.? ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	targetString = new std::string("pineapple");

	srand(time(NULL));

	////////////////////////////////////////////////////////////////
	//initialize genetic algorithm of type char
	//have 10 members in each population
	//gene size of population will be the size of the target string
	//then setting fitness/random gene functions
	//mutation rate of 5%
	////////////////////////////////////////////////////////////////
	m_GA = new GeneticAlgorithm<char>(10, targetString->length(), f, t, 5.0f);

	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;

	return true;
}

void Application2D::shutdown() {

	delete m_GA;
	delete m_font;
	delete validChars;
	delete targetString;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();
	//if (input->isKeyDown(aie::INPUT_KEY_1))
	//{
		if (m_GA->bestFitness != 1)
			m_GA->NewGeneration();

	//}


	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	// output some text, uses the last used colour
	//char fps[32];
	//sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, "Target string: ", 0, 720 - 32);
	m_2dRenderer->drawText(m_font, targetString->c_str(), 250, 720 - 32);


	//drawing best genes
	std::string s;
	for (auto iter = m_GA->bestGenes.begin(); iter != m_GA->bestGenes.end(); iter++)
	{
		s += *iter;
	}
	m_2dRenderer->drawText(m_font, "Best Genes   :", 0, 720 - 64);
	m_2dRenderer->drawText(m_font, s.c_str(), 250, 720 - 64);


	//draw whole population
	for (int i = 0; i < m_GA->Population.size(); i++)
	{
		std::string c;
		auto iter = m_GA->Population.begin();
		std::advance(iter, i);
		for (auto elIter = (*iter)->Genes.begin(); elIter != (*iter)->Genes.end(); elIter++)
		{
			c += *elIter;
		}

		m_2dRenderer->drawText(m_font, c.c_str(), 0, (720 - 128) - ((i + 1) * 32));
	}

	//generation number text
	std::string genNumber = std::to_string(m_GA->generationNumber);
	m_2dRenderer->drawText(m_font, genNumber.c_str(), 350, 720 - 128);
	m_2dRenderer->drawText(m_font, "Current Generation: ", 0, 720 - 128);

	// done drawing sprites
	m_2dRenderer->end();
}

char Application2D::getRandomGene()
{
	int random = rand() % validChars->length() + 0;
	return (*validChars)[random];
}

float Application2D::fitness(int index)
{
	float score = 0;
	auto it = m_GA->Population.begin();
	std::advance(it, index);
	DNA<char> dna = **it;

	for (int i = 0; i < dna.Genes.size(); i++)
	{
		auto iter = dna.Genes.begin();
		std::advance(iter, i);
		if (*iter == (*targetString)[i])
		{
			score += 1;
		}
	}

	score /= (*targetString).size();
	score = (pow(2, score) - 1) / (2 - 1);
	return score;
}