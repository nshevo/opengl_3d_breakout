#pragma once 

#include "Ball.h"
#include "CubeMap.h"
#include "Camera.h"
#include "Collision.h"
#include "collider.h"
#include "Shader.h"

#include "MenuProducer.h"

/* Game controller class */
/* manages ball, wall, collider models as well as cubeMap and collision of ball with cubes */
class GameController {
	public:
		Ball ball;
		Wall wall;
		CubeMap cubeMap;
		Collision collision;
		Collider collider;
		int slowSteps = 8;		

		Shader ballShader, wallShader, cubeShader;
		Camera camera;

		int gameWindowHeight, gameWindowWidth;

		int totalNumberOfLifes = 3;
		int level = 0;
		bool gameOver = false;
		bool gameRunning = false;
		bool levelComplete = false;

		MenuProducer menuProducer;
		int selectedIndex = 0;
		bool enterPressed = false;
		bool backspacePressed = false;

		GameController(Shader ballShader, Shader wallShader, Shader cubeShader, int height, int width) {
			this->ballShader = ballShader;
			this->wallShader = wallShader;
			this->cubeShader = cubeShader;
			this->camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
			this->cubeMap = CubeMap(this->wall, level);
			
			this->ball = Ball(wall.VIEW_OFFSET);
			this->collider = Collider(this->wall, &ball);
			this->collision = Collision(&cubeMap, &ball);

			gameWindowHeight = height;
			gameWindowWidth = width;

		}

		void update(GLfloat deltaTime, Camera camera) {
			this->camera = camera;
			float length = glm::length(ball.velocity);
			for (int i = 0; i < slowSteps; i++) {
				ball.setUpVirtualPosition(deltaTime / slowSteps);
				collision.collide();
				collider.update();
				ball.updatePosition(deltaTime / slowSteps);
			}
		
			checkGameState();

		}

		void setEnterPressed() {
			this->enterPressed = true;	
		}

		void setBackspacePressed() {
			backspacePressed = true;
		}


		void checkSelectedMenuPoint() {
			if (selectedIndex == 0 && enterPressed) {
				//start game menu point
				gameRunning = true;
			}
			else if (selectedIndex == 1 && enterPressed) {
				// about the game menu point
				drawAboutText();
				if (backspacePressed) {
					resetBackspacePressed();
					resetEnterPressed();
				}
			}
		}

		void resetEnterPressed() {
			if (enterPressed) {
				enterPressed = false;
			}
		}

		void resetBackspacePressed() {
			if (backspacePressed) {
				backspacePressed = false;
			}
		}

		void selectedIndexChanged(int index) {
			this->selectedIndex = index;
		}

		void restartGame() {
			this->camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
			this->cubeMap = CubeMap(this->wall, level);

			this->ball = Ball(wall.VIEW_OFFSET);
			this->collider = Collider(this->wall, &ball);
			this->collision = Collision(&cubeMap, &ball);
			gameOver = false;
		}

		void checkGameState() {
			if (collider.getBallMissedCollider() == totalNumberOfLifes) {
				gameOver = true;
			}

			if (cubeMap.getTotalCubes() == collision.getTotalCollisions()) {
				levelComplete = true;
			}
		}

		void processMouseMovement(GLfloat xoffset, GLfloat yoffset) {
			if (gameRunning) {
				collider.processMouseMovement(xoffset, yoffset);
			}
		}

		void processMouseClick() {
			if (gameRunning) {
				collider.releaseBall();
			}
		}

		typedef shared_ptr<FTLabel> Label;


		shared_ptr<GLFont> glFont = shared_ptr<GLFont>(new GLFont("fonts/SpaceMission.otf"));

		bool once = true;

		void drawAboutText() {
			int startX = gameWindowWidth / 3;
			int startY = gameWindowHeight / 5;

			string text = "3D-Breakout was made for CG course with OpenGL 3.3 by Nikita Shevchenko (571034) at the HTW in Berlin.";

			string hintText = "Press backspace to go back to the menu";

			Label aboutTextLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				text.data(),
				startX,
				startY,
				gameWindowWidth,
				gameWindowHeight
			));

			Label hintTextLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				hintText.data(),
				startX,
				startY + 150,
				gameWindowWidth,
				gameWindowHeight
			));

			aboutTextLabel->setColor(1.0, 1.0, 1.0, 1.0);
			aboutTextLabel->setPixelSize(24);

			hintTextLabel->setColor(1.0, 1.0, 1.0, 1.0);
			hintTextLabel->setPixelSize(24);

			glClear(GL_COLOR_BUFFER_BIT);

			glDisable(GL_DEPTH_TEST);
			aboutTextLabel->render();
			hintTextLabel->render();
			glEnable(GL_DEPTH_TEST);

		}

		void drawGameOverText() {
			int startX = gameWindowWidth / 2.5;
			int startY = gameWindowHeight / 2;

			string gameOverText = "Game Over, " + to_string(totalNumberOfLifes) + " / " + to_string(totalNumberOfLifes) + " lifes lost!";

			string postGameOverText = "Press Enter to try again!";

			Label gameOverLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				gameOverText.data(),
				startX,
				startY,
				gameWindowWidth,
				gameWindowHeight
			));

			Label postGameOverLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				postGameOverText.data(),
				startX,
				startY + 64,
				gameWindowWidth,
				gameWindowHeight
			));

			gameOverLabel->setColor(1.0, 1.0, 1.0, 1.0);
			gameOverLabel->setPixelSize(32);

			postGameOverLabel->setColor(1.0, 1.0, 1.0, 1.0);
			postGameOverLabel->setPixelSize(32);

			glDisable(GL_DEPTH_TEST);
			gameOverLabel->render();
			postGameOverLabel->render();
			glEnable(GL_DEPTH_TEST);

		}

		void drawNextLevelText() {
			int startX = gameWindowWidth / 2.5;
			int startY = gameWindowHeight / 2;

			string nextLevelText = "Level Complete! You can play the next level!";

			string postNextLevelText = "Press Enter";

			Label nextLevelLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				nextLevelText.data(),
				startX,
				startY,
				gameWindowWidth,
				gameWindowHeight
			));

			Label postNextLevelLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				postNextLevelText.data(),
				startX,
				startY + 64,
				gameWindowWidth,
				gameWindowHeight
			));

			nextLevelLabel->setColor(1.0, 1.0, 1.0, 1.0);
			nextLevelLabel->setPixelSize(32);

			postNextLevelLabel->setColor(1.0, 1.0, 1.0, 1.0);
			postNextLevelLabel->setPixelSize(32);

			glDisable(GL_DEPTH_TEST);
			nextLevelLabel->render();
			postNextLevelLabel->render();
			glEnable(GL_DEPTH_TEST);

		}

		void drawGameFinished() {
			int startX = gameWindowWidth / 2.5;
			int startY = gameWindowHeight / 2;

			string gameFinished = "Game Complete! Follow me on Github.com/psylocube";


			Label gameFinishedLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				gameFinished.data(),
				startX,
				startY,
				gameWindowWidth,
				gameWindowHeight
			));		

			gameFinishedLabel->setColor(1.0, 1.0, 1.0, 1.0);
			gameFinishedLabel->setPixelSize(32);

			glDisable(GL_DEPTH_TEST);
			gameFinishedLabel->render();
			glEnable(GL_DEPTH_TEST);
		}

		void drawInterface() {

			int startX = 15;
			int startY = 0;

			string scoreLabelText = "Score: " + to_string(collision.getTotalCollisions()) + " / " + to_string(cubeMap.getTotalCubes());

			string livesLabelText = "Lifes: " + to_string(totalNumberOfLifes - collider.getBallMissedCollider()) + " / " + to_string(totalNumberOfLifes);


			Label scoreLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				scoreLabelText.data(),
				startX,
				startY,
				gameWindowWidth,
				gameWindowHeight
			));

			Label livesLabel = shared_ptr<FTLabel>(new FTLabel(
				glFont,
				livesLabelText.data(),
				startX * 25,
				startY,
				gameWindowWidth,
				gameWindowHeight
			));

			scoreLabel->setColor(0.0, 0.0, 0.0, 0.9);
			scoreLabel->setPixelSize(32);

			livesLabel->setColor(0.0, 0.0, 0.0, 0.9);
			livesLabel->setPixelSize(32);

			glDisable(GL_DEPTH_TEST);
			scoreLabel->render();
			livesLabel->render();
			glEnable(GL_DEPTH_TEST);							
		}

		void draw() {

			if (!gameRunning) {
				menuProducer = MenuProducer(gameWindowWidth, gameWindowHeight, selectedIndex);
				menuProducer.drawMenu();
				checkSelectedMenuPoint();
			}
			else {
				if (!gameOver) {

					if (levelComplete) {
						if (level <= 2) {
							drawNextLevelText();
							if (enterPressed) {
								resetEnterPressed();
								playNextLevel();
							}
						}
						else {
							gameFinish();
							drawGameFinished();
						}
					}
					else {
						resetEnterPressed();

						setupShader(ballShader);
						ball.draw(ballShader);

						setupShader(wallShader);
						wall.draw(wallShader);

						setupShader(cubeShader);
						cubeMap.draw(cubeShader);
						collider.draw(cubeShader);

						drawInterface();
					}					
				}
				else {
					
					drawGameOverText();
					if (enterPressed) {
						resetEnterPressed();
						restartGame();
					}
								
				}
			}		

		}

		private:
			void setupShader(Shader shader) {
				shader.use();
				glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
				glm::mat4 view = camera.getViewMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			}

			void playNextLevel() {
				level += 1;
				this->cubeMap = CubeMap(this->wall, level);

				this->ball = Ball(wall.VIEW_OFFSET);
				this->collider = Collider(this->wall, &ball);
				this->collision = Collision(&cubeMap, &ball);

				levelComplete = false;

			}

			void gameFinish() {
				this->ball = Ball(wall.VIEW_OFFSET);
				this->collider = Collider(this->wall, &ball);
				this->collision = Collision(&cubeMap, &ball);
			}
};