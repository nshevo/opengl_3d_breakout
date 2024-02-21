#pragma once

#include "ColoredCubeProducer.h"
#include "MissingCubeProducer.h"

#include "cube.h"

#include <cstdlib>
#include <iostream>
#include <ctime>

#include "wall.h"
#include "CubeWall.h"

/* CubeMap Class */
/* saves all cubes and initializez levels based on walls */
class CubeMap {
public:
	struct ContactPoint {
		glm::vec3 point;
		glm::vec3 normal;
		GLfloat dt;
		int indexOfPlane;
	};

	int WIDTH;
	int HEIGHT;
	int DEPTH;
	float VIEW_OFFSET;

	Cube cube;
	Wall wall;
	CubeProducer ****cubeProducer;

	MissingCubeProducer *noCube = new MissingCubeProducer();

	CubeMap() {	}

	CubeMap(Wall wall, int level) {
		initializeDimensions(wall);
		initializeCubeDrawer();
		initializeLevel(level);
	}

	int getTotalCubes() {
		return totalCubes;
	}

	void draw(Shader shader) {
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 13; j++) {
				for (int k = 0; k < 30; k++) {
					cube.setCubeProducer(cubeProducer[i][j][k]);
					int x = getX(i);
					int y = getY(j);
					int z = getZ(k);
					cube.draw(shader, x,y,z);
				}
			}
		}
	}

	glm::vec3 getCubePosition(int index_x, int index_y, int index_z) {
		return glm::vec3(getX(index_x), getY(index_y), getZ(index_z));
	}

private:

	int totalCubes = 0;
	
	void initializeLevel(int level) {
		//setupCubeWall(CubeWall("walls/test.wall"), 20);

		if (level == 0) {
			setupCubeWall(CubeWall("walls/background.wall"), 0);
			setupCubeWall(CubeWall("walls/rgb.wall"), 8);
		}
		else if (level == 1) {
			setupCubeWall(CubeWall("walls/background.wall"), 0);
			setupCubeWall(CubeWall("walls/r.wall"), 4);
			setupCubeWall(CubeWall("walls/g.wall"), 6);
			setupCubeWall(CubeWall("walls/b.wall"), 8);
		}
		else if (level == 2) {
			setupCubeWall(CubeWall("walls/background.wall"), 0);
			setupCubeWall(CubeWall("walls/rgb.wall"), 8);
			setupCubeWall(CubeWall("walls/rgb.wall"), 10);
			setupCubeWall(CubeWall("walls/rgb.wall"), 12);
		}

	}

	void initializeDimensions(Wall wall) {
		WIDTH = wall.WIDTH;
		HEIGHT = wall.HEIGHT;
		DEPTH = wall.DEPTH;
		VIEW_OFFSET = wall.VIEW_OFFSET;
	}

	void initializeCubeDrawer() {
		cubeProducer = new CubeProducer***[13];

		for (int x = 0; x < 13; ++x) {
			cubeProducer[x] = new CubeProducer**[13];
			for (int y = 0; y < 13; ++y) {
				cubeProducer[x][y] = new CubeProducer*[30];
				for (int z = 0; z < 30; ++z) {
					cubeProducer[x][y][z] = noCube;
				}
			}
		}
	}

	void setupCubeWall(CubeWall wall, int z) {
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 13; j++) {
				cubeProducer[i][j][z] = wall.cubeProducer[i][j];
			}
		}
		totalCubes += wall.getNumberOfCubes();		
	}

	float getX(int index) {
		return -(float)WIDTH / 2 + 0.5f + index;
	}

	float getY(int index) {
		return -(float)HEIGHT / 2 + 0.5f + index;
	}

	float getZ(int index) {
		return -(float)DEPTH + this->VIEW_OFFSET + 0.5f + index;
	}

};