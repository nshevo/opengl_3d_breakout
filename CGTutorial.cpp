// Include Standardheader, steht bei jedem C/C++-Programm am Anfang
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>

#include "stdio.h"
#include "stdlib.h"
#include "GLFont.h"
#include "FTLabel.h"
#include "Shader.h"

// Include GLEW, GLEW ist ein notwendiges Übel. Der Hintergrund ist, dass OpenGL von Microsoft
// zwar unterstützt wird, aber nur in einer Uralt-Version. Deshalb beinhaltet die Header-Datei,
// die vom Betriebssystem zur Verfügung gestellt wird, nur Deklarationen zu den uralten Funktionen,
// obwohl der OpenGL-Treiber, und damit die OpenGL-dll die neuesten Funktionen implementiert.
// Die neueren Funktionen werden deshalb über diese Header-Datei separat zur Verfügung gestellt.
#include <GL/glew.h>

// SOIL
#include "external/SOIL2/include/SOIL2.h"

// Include GLFW, OpenGL definiert betriebssystemunabhängig die graphische Ausgabe. Interaktive 
// Programme beötigen aber natürlich auch Funktionen für die Eingabe (z. B. Tastatureingaben)
// Dies geht bei jedem OS (z. B. Windows vs. MacOS/Unix) etwas anders. Um nun generell plattformunabhängig
// zu sein, verwenden wir GLFW, was die gleichen Eingabe-Funktionen auf die Implementierung unterschiedlicher
// OS abbildet. (Dazu gibt es Alternativen, glut wird z. B. auch häufig verwendet.)
#include <GLFW/glfw3.h>

// Include GLM, GLM definiert für OpenGL-Anwendungen Funktionen der linearen Algebra wie
// Transformationsmatrizen. Mann könnte GLM auch durch etaws anderes ersetzen oder aber in einem
// anderen Kontext verwenden.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// In C- und C++-Programmen ist die Reihenfolge der include-Direktiven wichtig.
// Dateien, die mit Spitzklammern includiert werden, werden in den System-Verzeichnissen
// gesucht, die mit doppelten Hochkommata im lokalen Projektverzeichnis
// (wo genau ist in den Projekteinstellungen zu finden und ggf. zu ändern.) 
// Diese Datei benötigen wir, um die Shader-Programme komfortabel in die Hardware zu laden.
// (Mit der rechten Mouse-taste können Sie in VS diese Datei öffnen, um nachzuschauen, was dort deklariert wird.)
//#include "shader.hpp"

#include "CGTutorial.h"
#include "Camera.h"
#include "GameController.h"

// globale variablen

//camera conf
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
GameController *gameController;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLfloat deltaTime = 0.0f; // time between current and last frame
GLfloat lastFrame = 0.0f; // time of last frame

int WIDTH, HEIGHT;
GLfloat lastX, lastY;

// Callback functions

// Callback-Mechanismen gibt es in unterschiedlicher Form in allen möglichen Programmiersprachen,
// sehr häufig in interaktiven graphischen Anwendungen. In der Programmiersprache C werden dazu 
// Funktionspointer verwendet. Man übergibt einer aufgerufenen Funktion einer Bibliothek einen
// Zeiger auf eine Funktion, die zurückgerufen werden kann. Die Signatur der Funktion muss dabei
// passen. Dieser Mechanismus existiert auch in C++ und wird hier verwendet, um eine einfache
// Fehlerbehandlung durchzuführen. Diese Funktion gibt Fehler aus, die beim Aufruf von OpenGL-Befehlen
// auftreten.
void error_callback(int error, const char* description)
{
	// Mit fputs gibt man hier den String auf den Standarderror-Kanal aus.
	// In der C-Welt, aus der das hier übernommen ist, sind Strings Felder aus "char"s, die mit 
	// dem Wert null terminiert werden.
	fputs(description, stderr);
}

// Diese Funktion wird ebenfalls über Funktionspointer der GLFW-Bibliothek übergeben.
// (Die Signatur ist hier besonders wichtig. Wir sehen, dass hier drei Parameter definiert
//  werden müssen, die gar nicht verwendet werden.)
// Generell überlassen wir der GLFW-Bibliothek die Behandlung der Input-Ereignisse (Mouse moved,
// button click, Key pressed, etc.).
// Durch die Übergabe dieser Funktion können wir Keyboard-Events 
// abfangen. Mouse-Events z. B. erhalten wir nicht, da wir keinen Callback an GLFW übergeben.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	// Mit rechte Mousetaste -> gehe zu Deklaration finden Sie andere Konstanten für Tasten.
	case GLFW_KEY_ESCAPE:
		// Das Programm wird beendet, wenn BenutzerInnen die Escapetaste betätigen.
		// Wir könnten hier direkt die C-Funktion "exit" aufrufen, eleganter ist aber, GLFW mitzuteilen
		// dass wir das Fenster schliessen wollen (siehe Schleife unten).
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_W:
		(*gameController).selectedIndexChanged(0);
		break;
	case GLFW_KEY_S:
		(*gameController).selectedIndexChanged(1);
		break;
	case GLFW_KEY_ENTER:
		(*gameController).setEnterPressed();
		break;
	case GLFW_KEY_DOWN:
		(*gameController).selectedIndexChanged(1);
		break;
	case GLFW_KEY_UP:
		(*gameController).selectedIndexChanged(0);
		break;
	case GLFW_KEY_BACKSPACE:
		(*gameController).setBackspacePressed();
		break;
	default:
		break;
	}
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	(*gameController).processMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		(*gameController).processMouseClick();
	}
}

// Diese drei Matrizen speichern wir global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen. Ihre Bedeutung habe ich in der Vorlesung Geometrische
// Transformationen erklärt, falls noch nicht geschehen, jetzt anschauen !
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 ModelMatrix;

// Einstiegspunkt für C- und C++-Programme (Funktion), Konsolenprogramme könnte hier auch Parameter erwarten
int main(void)
{
	// Initialisierung der GLFW-Bibliothek
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_SAMPLES, 4); //4x antialiasing

	// Öffnen eines Fensters für OpenGL, die letzten beiden Parameter sind hier unwichtig
	// Diese Funktion darf erst aufgerufen werden, nachdem GLFW initialisiert wurde.
	// (Ggf. glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen -> MacOSX)
	// use opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWmonitor* MyMonitor = glfwGetPrimaryMonitor(); // The primary monitor
	const GLFWvidmode* mode = glfwGetVideoMode(MyMonitor);
	
	WIDTH = mode->width;
	HEIGHT = mode->height;
	
	lastX = WIDTH / 2;
	lastY = HEIGHT / 2;

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, // Breite	 // Hoehe
											"3D Ping-Pong Breakout", // Ueberschrift
											MyMonitor,				 // windowed mode
											nullptr);				 // shared window

	if (!window)
	{
		fprintf(stderr, "GLFW Window creation failed!\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Wir könnten uns mit glfwCreateWindow auch mehrere Fenster aufmachen...
	// Spätestens dann wäre klar, dass wir den OpenGL-Befehlen mitteilen müssen, in
	// welches Fenster sie "malen" sollen. Wir müssen das aber zwingend auch machen,
	// wenn es nur ein Fenster gibt.

	// Bis auf weiteres sollen OpenGL-Befehle in "window" malen.
	// Ein "Graphic Context" (GC) speichert alle Informationen zur Darstellung, z. B.
	// die Linienfarbe, die Hintergrundfarbe. Dieses Konzept hat den Vorteil, dass
	// die Malbefehle selbst weniger Parameter benötigen.
	// Erst danach darf man dann OpenGL-Befehle aufrufen !
    glfwMakeContextCurrent(window);

	// Auf Keyboard-, Cursor-, Mouse-Events reagieren (s. o.)
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// GLFW cursor input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialisiere GLEW
	// (GLEW ermöglicht Zugriff auf OpenGL-API > 1.1)
	glewExperimental = GL_TRUE; // Diese Zeile ist leider notwendig.

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	int SCREEN_WIDTH, SCREEN_HEIGHT;
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader ourShader("shaders/lighting.vs", "shaders/lighting.frag");
	Shader wallShader("shaders/wall.vs", "shaders/wall.frag");
	Shader ballShader("shaders/ball.vs", "shaders/ball.frag");

	gameController = new GameController(ballShader, wallShader, ourShader, mode->height, mode->width);
	
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	// Alles ist vorbereitet, jetzt kann die Eventloop laufen...
	while (!glfwWindowShouldClose(window))
	{

		lightPos = glm::vec3(1.2f * glm::cos(glfwGetTime()), 0.0f, 1.5f * glm::sin(glfwGetTime()));

		// calculate frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	
		View = camera.getViewMatrix();
		
		Projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		//render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// game state update
		(*gameController).update(deltaTime, camera);
		(*gameController).draw();

		// Bildende. 
		// Bilder werden in den Bildspeicher gezeichnet (so schnell wie es geht.). 
		// Der Bildspeicher wird mit der eingestellten Bildwiederholfrequenz (also z. B. 60Hz)
		// ausgelesen und auf dem Bildschirm dargestellt. Da beide Frequenzen nicht übereinstimmen, würde
		// man beim Auslesen auf unfertige Bilder stoßen. Das wäre als Flimmern auf dem Bildschirm zu
		// erkennen. (War bei älteren Grafikkarten tatsächlich so.)
		// Dieses Problem vermeidet man, wenn man zwei Bildspeicher benutzt, wobei in einen gerade
		// gemalt wird, bzw. dort ein neues Bild entsteht, und der andere auf dem Bildschirm ausgegeben wird.
		// Ist man mit dem Erstellen eines Bildes fertig, tauscht man diese beiden Speicher einfach aus ("swap").
		glfwSwapBuffers(window);

		// Hier fordern wir glfw auf, Ereignisse zu behandeln. GLFW könnte hier z. B. feststellen,
		// das die Mouse bewegt wurde und eine Taste betätigt wurde.
		// Da wir zurzeit nur einen "key_callback" installiert haben, wird dann nur genau diese Funktion
		// aus "glfwPollEvents" heraus aufgerufen.
        glfwPollEvents();
	} 
		

	// Wenn der Benutzer, das Schliesskreuz oder die Escape-Taste betätigt hat, endet die Schleife und
	// wir kommen an diese Stelle. Hier können wir aufräumen, und z. B. das Shaderprogramm in der
	// Grafikkarte löschen. (Das macht zurnot das OS aber auch automatisch.)
	glDeleteProgram((*gameController).cubeShader.program);
	glDeleteProgram((*gameController).wallShader.program);
	glDeleteProgram((*gameController).ballShader.program);

	// Schießen des OpenGL-Fensters und beenden von GLFW.
	glfwTerminate();

	return 0; // Integer zurückgeben, weil main so definiert ist
}
