//Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <sstream> //used for concatanating strings together
#include <SFML/Audio.hpp>

using namespace sf;

//Function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//Location of player/branch: left or right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
    //Creating video mode object
	VideoMode vm(1920, 1080);

	//Creating a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	
	//Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	//Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	//Create a sprite
	Sprite spriteBackground;

	//Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	//Set the spriteBackground tocover the screen
	spriteBackground.setPosition(0, 0);

	/***********Creating a center tree sprite. Stationary sprite.  */
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	//All trees in the background
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
	Sprite spriteTree6;
	spriteTree6.setTexture(textureTree2);


	spriteTree2.setPosition(20, 0);
	spriteTree3.setPosition(300, -400);
	spriteTree4.setPosition(1300, -400);
	spriteTree5.setPosition(1500, -500);
	spriteTree6.setPosition(1900, 0);

	/************************Creating the bee sprite. It moves, needs adjustments */
	Texture textureBee;
	//textureBee.loadFromFile("graphics/bee.png");
	textureBee.loadFromFile("graphics/bug1.gif");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	//IS the bee currently moving?
	bool beeActive = false;
	//Bee flying speed
	float beeSpeed = 0.0f;

	/************************Creating the cloud sprite. It moves, needs adjustments */
	Texture textCloud;
	//3 clouds from 1 texture
	textCloud.loadFromFile("graphics/cloud.png");
	// 3 new cloud sprite with the same texture
	const int NUM_CLOUDS = 6;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeeds[NUM_CLOUDS];
	bool cloudsActive[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++)
	{
		clouds[i].setTexture(textCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudsActive[i] = false;
		cloudSpeeds[i] = 0;
	}
	

	//*************variables to control time
	Clock clock;

	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//track whether the game is running. For enter button
	bool paused = true;



	//**********************HUD items. Text, score, font
	int score = 0;

	//Draw text
	Text messageText;
	Text scoreText;
	//Text fpsText;
	
	//Defining font for text
	Font font;
	font.loadFromFile("fonts/Fool.ttf"); //KOMIKAP_

	//Setting font to the text
	messageText.setFont(font);
	scoreText.setFont(font);
	//fpsText.setFont(font);

	// Set up fps text
	//fpsText.setFillColor(Color::White);
	//fpsText.setCharacterSize(115);
	//fpsText.setPosition(1200,20);

	//Assign actual message to text
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	//Increase Font Size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(110);

	//Font color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Positioning the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 5);

	// Backgrounds for the text
	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(400, 105));
	rect1.setPosition(10, 30);

	//RectangleShape rect2;
	//rect2.setFillColor(sf::Color(0, 0, 0, 150));
	//rect2.setSize(Vector2f(880, 105));
	//rect2.setPosition(1150, 30);

	//****************Preparing the branches or kunai
	Texture textureBranch;
	//textureBranch.loadFromFile("graphics/Kunai.png");
	textureBranch.loadFromFile("graphics/branch.png");

	//setting texture for each branch/kunai sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		//Setting the branches position to center, spin it around without changing its position
		branches[i].setOrigin(220, 20);
	}

	

	//*****************Player, gravestone, chopped log, axe Creation
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//The player starts on the left side
	side playerSide = side::LEFT;

	//The tombstone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//The axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Line up axe with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//The logs that have been chopped off
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	//log variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//*****************Player input controls
	bool acceptInput = false;// bool to determine when to accept input and when not to

	//******************************* Sounds = chop, death, out of time
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	// control the drawing of the score
	//int lastDrawn = 0;

	while (window.isOpen())
	{
		//*************Player input

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				//Listen for key presses again
				acceptInput = true;

				//hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		//Player input
		if (Keyboard::isKeyPressed(Keyboard::Escape)) 
		{
			window.close();
		}

		//Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			//Reset time and score
			score = 0;
			timeRemaining = 5;

			//Make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			//The tombstone is hidden when starting the new game
			spriteRIP.setPosition(675, 2000);

			//Move the player into postion
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}

		//Wrapping player controls to make sure we accept single key strokes
		if (acceptInput)
		{
			//the right arrow key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				//Ensuring player is on the right
				playerSide = side::RIGHT;
				score++;

				// Adding amount of time to time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				//update branches
				updateBranches(score);

				//set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				//Play chop sound
				chop.play();
			}
			//the left arrow key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				//Ensuring player is on the right
				playerSide = side::LEFT;
				score++;

				// Adding amount of time to time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				//update branches
				updateBranches(score);

				//set the log flying to the Right
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				
				//Play chop sound
				chop.play();
			}
		}

		
		//*************Update scene
		if (!paused)
		{
			//Measure time
			Time dt = clock.restart();

			//Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			//Reduce the size of the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining,
				timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				//Pause the game
				paused = true;

				//Change the message shown to show time has run out
				messageText.setString("Out of Time!!");

				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				
				//Play the out of time sound
				outOfTime.play();
			}

			//Setup the bee
			if (!beeActive)
			{
				//Speed of the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				//height of the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else //Move the bee
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				// Has the bee reached the right hand edge of the screen?
				if (spriteBee.getPosition().x < -100)
				{
					// Set it up ready to be a new bee in the next frame
					beeActive = false;
				}

			}

			//Coding the clouds
			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				if (!cloudsActive[i])
				{
					// Speed of the cloud
					srand((int)time(0) * i);
					cloudSpeeds[i] = (rand() % 200);

					// Height of the cloud
					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;

				}
				else
				{
					// Set the new position
					clouds[i].setPosition(
						clouds[i].getPosition().x +	(cloudSpeeds[i] * dt.asSeconds()),
						clouds[i].getPosition().y);

					// Has the cloud reached the right hand edge of the screen?
					if (clouds[i].getPosition().x > 1920)
					{
						// Set it up ready to be a whole new cloud next frame
						cloudsActive[i] = false;
					}

				}
			}

			//Update Score text

			//lastDrawn++;
			//if (lastDrawn == 100) {
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());
				
				//// Draw the fps
				//std::stringstream ss2;
				//ss2 << "FPS = " << 1 / dt.asSeconds();
				//fpsText.setString(ss2.str());
				//lastDrawn = 0;
			//}

			//****************Updating the branch/kunai sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					//Move sprite to the left side
					branches[i].setPosition(610, height);
					//Flip the sprite round the other way
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					//Move sprite to the right side
					branches[i].setPosition(1330, height);
					//Set the sprite rotation to normal
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(0);
				}
				else
				{
					//hide the branch/kunai
					branches[i].setPosition(3000, height);
				}
			}

			//Handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				//Has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					//Set it up ready to be a whole new log in the next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			//Player squished by branch?
			if (branchPositions[5] == playerSide)
			{
				//death
				paused = true;
				acceptInput = false;

				//Bring in tombstone
				spriteRIP.setPosition(525, 760);
				//hide the player
				spritePlayer.setPosition(2000, 660);

				//Change the text of the message
				messageText.setString("YOU MESSED UP!!");
				//Center message on screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				//Play the out of time sound
				death.play();
				
			}

		} //End if(!paused)

		//*************Draw the scene

		//*************Clear everything from the last frame
		window.clear();

		//*************Game scenes drawn
		window.draw(spriteBackground);

		//*************Draw the clouds
		for (int i = 0; i < NUM_CLOUDS; i++)
		{
			window.draw(clouds[i]);
		}

		//********Background trees
		window.draw(spriteTree2);
		window.draw(spriteTree3);
		window.draw(spriteTree4);
		window.draw(spriteTree5);
		window.draw(spriteTree6);

		//*************Draw the branches/kunai
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}		

		//*************Draw the center tree
		window.draw(spriteTree);

		//*************Draw the player
		window.draw(spritePlayer);

		//*************Draw the axe
		window.draw(spriteAxe);

		//*************Draw the chopped log
		window.draw(spriteLog);

		//*************Draw the tombstone
		window.draw(spriteRIP);

		// Draw backgrounds for the text
		window.draw(rect1);
		//window.draw(rect2);

		//*************Draw the bug
		window.draw(spriteBee);

		//*************Draw the score
		window.draw(scoreText);

		//*************Draw the timebar
		window.draw(timeBar);


		// Draw the FPS
		//window.draw(fpsText);

		if (paused)
		{
			window.draw(messageText);
		}

		//*************Displaying all the assets drawn
		window.display();

	}

	return 0;

}

//Function definition
void updateBranches(int seed)
{
	//Move all branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}
	//Spawn new branch at position 0
	//LEFT RIGHT OR NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[1] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
