#pragma once

#include "GLFont.h"
#include "FTLabel.h"

/* LevelManager Class */
/*  */
class MenuProducer {

public:
	int gameWindowWidth, gameWindowHeight;
	int selectedIndex;
	
	MenuProducer() {}

	MenuProducer(int gameWindowWidth, int gameWindowHeight, int selectedIndex) 
	{
		this->gameWindowWidth = gameWindowWidth;
		this->gameWindowHeight = gameWindowHeight;
		this->selectedIndex = selectedIndex;
	}

	typedef shared_ptr<FTLabel> Label;

	shared_ptr<GLFont> glFont = shared_ptr<GLFont>(new GLFont("fonts/SpaceMission.otf"));


	void drawMenu() {	

		int startX = gameWindowWidth / 2;
		int startY = gameWindowHeight / 2;

		string gameTitleText = "3 Level 3D-Breakout";

		string newGameText = "Start the game!";

		string aboutGameText = "About";

		Label gameTitleLabel = shared_ptr<FTLabel>(new FTLabel(
			glFont,
			gameTitleText.data(),
			startX,
			startY,
			gameWindowWidth,
			gameWindowHeight
		));

		Label newGameLabel = shared_ptr<FTLabel>(new FTLabel(
			glFont,
			newGameText.data(),
			startX,
			startY + 64 * 2,
			gameWindowWidth,
			gameWindowHeight
		));

		Label aboutGameLabel = shared_ptr<FTLabel>(new FTLabel(
			glFont,
			aboutGameText.data(),
			startX,
			startY + 64 * 4,
			gameWindowWidth,
			gameWindowHeight
		));

		gameTitleLabel->setColor(1.0, 1.0, 1.0, 1.0);
		gameTitleLabel->setPixelSize(48);

		newGameLabel->setColor(1.0, 1.0, 1.0, 1.0);
		newGameLabel->setPixelSize(32);

		aboutGameLabel->setColor(1.0, 1.0, 1.0, 1.0);
		aboutGameLabel->setPixelSize(32);

		if (selectedIndex == 0) {
			newGameLabel->setPixelSize(64);
		}
		else if (selectedIndex == 1) {
			aboutGameLabel->setPixelSize(64);
		}
		
		glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		gameTitleLabel->render();
		newGameLabel->render();
		aboutGameLabel->render();
		glEnable(GL_DEPTH_TEST);

	}
};