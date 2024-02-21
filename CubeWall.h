#pragma once

#include "ColoredCubeProducer.h"
#include "MissingCubeProducer.h"
#include <map>

/* CubeWall Class */
/* represents a number of cubes */
class CubeWall {
public:
	CubeProducer *** cubeProducer;

	CubeWall() {	}

	CubeWall(std::string path) {
		initializeCubeDrawer();
		initializeColors();
		initializeCubeDrawerFromFile(path);
	}

	int getNumberOfCubes() {
		return numberOfCubes;
	}

private:
	std::map<char, CubeProducer*> colors;

	int numberOfCubes = 0;


	void initializeCubeDrawer() {
		cubeProducer = new CubeProducer**[13];
		for (int i = 0; i < 13; ++i) {
			cubeProducer[i] = new CubeProducer*[13];
		}
	}

	void initializeColors() {
		// no cube
		colors['x'] = new MissingCubeProducer();
		// red cube
		colors['r'] = new ColoredCubeProducer(1.0f, 0.0f, 0.0f);
		// blue cube
		colors['b'] = new ColoredCubeProducer(0.0f, 0.0f, 0.8f);
		// green cube
		colors['g'] = new ColoredCubeProducer(0.3f, 0.8f, 0.0f);
		// black cube
		colors['z'] = new ColoredCubeProducer(0.3f, 0.3f, 0.3f);
	}

	void initializeCubeDrawerFromFile(std::string path) {
		std::ifstream wallFile;
		// ensures ifstream objects can throw exceptions
		wallFile.exceptions(std::ifstream::badbit);
		try {
			wallFile.open(path);
			std::string line;
			for (int i = 0; i < 13; i++) {
				std::getline(wallFile, line);
				for (int j = 0; j < 13; j++) {
					int m_i = 12 - i;
					cubeProducer[j][m_i] = colors[(char)line[j]];

					char brick = (char)line[j];
				
					// count colored cubes
					if ( brick != 'x') {
						numberOfCubes += 1;
					}

				}
			}
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR :: WALLCUBE :: FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
	}
};