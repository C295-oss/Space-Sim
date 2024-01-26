#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <vector>


// TODO: Physics for particles
// TODO: Build collision detection


/// <summary>
/// Doesn't move, but attracts particles
/// </summary>
class Star
{
	sf::Vector2i pos;
	sf::Vector2f velocity;
	float strength;
	sf::CircleShape star;

	sf::Vector2f position = { 0,0 };

public:

	Star(sf::Vector2i pos, sf::Vector2f velocity, 
		 sf::Vector3i rgb, int sz, float strength)
	{
		this->pos = pos;
		//this->pos.x = pos.x;
		//this->pos.y = pos.y;
		this->strength = strength;
		this->velocity = velocity;

		star.setPosition(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
		star.setFillColor(sf::Color(rgb.x, rgb.y, rgb.z));
		star.setRadius(sz);
	}

	~Star()
	{
	}

	sf::Vector2i getPosition() const { return pos; }

	float getStrength() const { return strength; }

	void render(sf::RenderWindow* win)
	{
		win->draw(star);
	}
};


/// <summary>
///  Will spawn randomly on the screen
/// </summary>
class Particle
{
	sf::CircleShape shape;
	double mass;
	sf::Vector2f velocity = { 0,0 };
	sf::Vector2f pos = { 0,0 };

public:
	Particle(int r, sf::Vector2f pos, sf::Vector3i rgb)
	{
		this->pos = pos;
		shape.setRadius(r);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color(rgb.x, rgb.y, rgb.z));
		
		mass = 4.0 / 3.0 * 3.14159265359 * pow(r, 3);
	}

	~Particle()
	{
	}

	void render(sf::RenderWindow* win)
	{
		shape.setPosition(pos);
		win->draw(shape);
	}

	sf::Vector2f getPosition() const
	{
		return shape.getPosition();
	}

	void updatePhysics(Star& star)
	{
		const double G = 6.67408e-11;
		float distX = star.getPosition().x - shape.getPosition().x;
		float distY = star.getPosition().y - shape.getPosition().y;

		float dist = sqrt((distX * distX) + (distY * distY )); //Pythagorean Theorem~
		
		float inverseDist = 1.f / dist;
		float normalizedX = inverseDist * distX;
		float normalizedY = inverseDist * distY;

		// The further you are from the star, the less strength its
		// gravity has on the particle.
		float inverseSquareDist = inverseDist * inverseDist;
		float accelX = normalizedX * star.getStrength() * inverseSquareDist;
		float accelY = normalizedY * star.getStrength() * inverseSquareDist;
		
		velocity.x += accelX;
		velocity.y += accelY;

		pos.x += velocity.x;
		pos.y += velocity.y;
	}
};


int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0],
		"Gravity Simulation");
	window.setFramerateLimit(60);

	double dt = 0.1;
	int r = 5;
	sf::Vector3i rgbVals = { 255, 255, 255 };

	std::vector<Particle> particles;
	std::vector<Star> stars;
	
	sf::Vector2f particlePos;

	// Create 100 particles
	for (int i = 0; i < 10000; ++i)
	{
		particlePos.x = rand() % window.getSize().x;
		particlePos.y = rand() % window.getSize().y;

		//Particle(int& r, sf::Vector2f & pos, sf::Vector3i & rgb)
		Particle p = Particle(r, particlePos, rgbVals);
		particles.push_back(p);
	}

	// run the program as long as the window is open
	while (window.isOpen())
	{	
		// check all the window's events that were triggered 
		// since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				window.close();

			// check if ousepad held down
			
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i pos = sf::Mouse::getPosition();
				stars.push_back(Star(pos, { 0.0,0.0 }, {255,255,255}, 15, 2000));
			}
		}

		window.clear();
		
		for (int i = 0; i < stars.size(); ++i)
			stars.at(i).render(&window);

		for (int i = 0; i < particles.size(); ++i)
		{
			for (int j = 0; j < stars.size(); ++j)
			{
				particles.at(i).updatePhysics(stars.at(j));
			}
			particles.at(i).render(&window);
		}

		window.display();
	}
	return 0;
}
