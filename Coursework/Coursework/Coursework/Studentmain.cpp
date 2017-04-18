#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


//#include <windows.h>
#include "GameConstants.h"
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cColours.h"
#include "cShapes.h"
#include "cPyramid.h"
#include "cCube.h"
#include "cSphere.h"
#include "cMaterial.h"
#include "cLight.h"
#include "cStarfield.h"
#include "cFontMgr.h"
#include "cCamera.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cPlayer.h"
#include "cEnemy.h"
#include "cLaser.h"
#include "tardisWarsGame.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow)
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;


	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	//The example OpenGL code
	windowOGL theOGLWnd;

	//Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);


	//Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
	{
		//If it fails

		MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
	{
		MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
		return 1;
	}

	// Create Texture map
	cTexture blueTexture;
	blueTexture.createTexture("Images/blue.png");
	cTexture redTexture;
	redTexture.createTexture("Images/red.png");
	cTexture greenTexture;
	greenTexture.createTexture("Images/green.png");
	cTexture spaceShipTexture;
	spaceShipTexture.createTexture("Models/SpaceShip/sh3.jpg");
	cTexture laserTexture;
	laserTexture.createTexture("Models/laser.tga");
	cTexture whiteTexture;
	whiteTexture.createTexture("Images/white2.png");
	cTexture yellowTexture;
	whiteTexture.createTexture("Images/yellow.png");

	// the starfield
	cStarfield theStarField(whiteTexture.getTexture(), glm::vec3(14.0f, 14.0f, 5.0f));

	// Create Materials for lights
	cMaterial sunMaterial(lightColour4(0.0f, 0.0f, 0.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(1.0f, 1.0f, 1.0f, 1.0f), lightColour4(0, 0, 0, 1.0f), 5.0f);

	// Create Light
	cLight sunLight(GL_LIGHT0, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, 20, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight lfLight(GL_LIGHT1, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight rfLight(GL_LIGHT2, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(-30, 0, 100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	cLight cbLight(GL_LIGHT3, lightColour4(0, 0, 0, 1), lightColour4(1, 1, 1, 1), lightColour4(1, 1, 1, 1), glm::vec4(0, 0, -100, 1),
		glm::vec3(0.0, 0.0, 1.0), 0.0f, 180.0f, 1.0f, 0.0f, 0.0f);
	//Define Ambient light for scene
	GLfloat g_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, g_Ambient);

	// load game fonts
	// Load Fonts
	LPCSTR gameFonts[3] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/doctor_who.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 32);
	theFontMgr->addFont("Space", gameFonts[1], 32);
	theFontMgr->addFont("DrWho", gameFonts[2], 48);

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/Rickety Roadster.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };

	theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// Create Camera 1
	cCamera Camera1;
	Camera1.setTheCameraPos(glm::vec3(0.0f, 20.0f, 1.0f)); // First camera position
	Camera1.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	Camera1.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	Camera1.setTheCameraAspectRatio(windowWidth, windowHeight);
	Camera1.setTheProjectionMatrix(45.0f, Camera1.getTheCameraAspectRatio(), 0.1f, 300.0f);
	Camera1.update();

	// Create Camera 2

	cCamera Camera2;
	Camera2.setTheCameraPos(glm::vec3(0.0f, 4.0f, 6.0f)); // second camera position
	Camera2.setTheCameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	Camera2.setTheCameraUpVector(glm::vec3(0.0f, 1.0f, 0.0f)); // pointing upwards in world space
	Camera2.setTheCameraAspectRatio(windowWidth, windowHeight);
	Camera2.setTheProjectionMatrix(45.0f, Camera2.getTheCameraAspectRatio(), 0.1f, 300.0f);
	Camera2.update();


	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	// Model
	cModelLoader playerMdl;
	playerMdl.loadModel("Models/tardis1314.obj", blueTexture); // Player

	cModelLoader enemyMdl;
	enemyMdl.loadModel("Models/tardis1314.obj", redTexture); // Enemy

	cModelLoader theLaser;
	theLaser.loadModel("Models/laser.obj", laserTexture);

	cModelLoader player2Mdl;
	player2Mdl.loadModel("Models/tardis1314.obj", greenTexture); 

	//cModelLoader player3Mdl;
	//player3Mdl.loadModel("Models/Sphere.obj", yellowTexture);
	pgmWNDMgr->attachInputMgr(theInputMgr);


	for (int loop = 0; loop < 3; loop++)
	{
		theEnemy.push_back(new cEnemy);
		theEnemy[loop]->randomise();
		theEnemy[loop]->setMdlDimensions(enemyMdl.getModelDimensions());
		theEnemy[loop]->setScale(glm::vec3(1, 1, 1));
	}


	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 0, 0), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(player2Mdl.getModelDimensions());
	thePlayer.attachInputMgr(theInputMgr);
	thePlayer.attachSoundMgr(theSoundMgr);

	cPlayer thePlayer2;
	thePlayer2.initialise(glm::vec3(-5, 0, 0), 0.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer2.setMdlDimensions(playerMdl.getModelDimensions());


	float tCount = 0.0f;
	string outputMsg;
	string muteMsg;
	string timerMsg;


	
	
	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;

   //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();
		
		// Lab code goes here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL(windowWidth,windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (game == true)
		{
			milliseconds++;
		}

		if (milliseconds == 60)
		{
			milliseconds = 0;
			seconds++;
			if (seconds == 14)
			{
				gameOver = true;
				game = false;
			}
		}

		if (mute == false)
		{
			theSoundMgr->getSnd("Theme")->playAudio(AL_LOOPING);
		}


		if (cameraMode == 0)
		{
			glLoadMatrixf((GLfloat*)&Camera1.getTheViewMatrix());
		}
		else
		{
			glLoadMatrixf((GLfloat*)&Camera2.getTheViewMatrix());
		}

		theStarField.render(0.0f);
		sunMaterial.useMaterial();
		sunLight.lightOn();
		lfLight.lightOn();
		rfLight.lightOn();
		cbLight.lightOn();

		if (theInputMgr->isKeyDown(80))
		{
			game = true;
		}


		//if (theInputMgr->isKeyDown(68))
		//{
		//	camLane++;																ATTEMPTED 3RD PERSON - HOWEVER COULD NOT GET TO WORK	
		////	m_cameraPos.x;
		//	Camera1.setTheCameraPos(glm::vec3(5.0f, 4.0f, 6.0f));
		//}

		if (!game)
		{
			glPushMatrix();
			theOGLWnd.setOrtho2D(windowWidth, windowHeight);
			theFontMgr->getFont("Space")->printText("MENU", FTPoint(10, 35, 0.0f), colour3f(0.0f, 30.0f, 200.0f));
			theFontMgr->getFont("Space")->printText("PRESS P TO START", FTPoint(10, 70, 0.0f), colour3f(0.0f, 30.0f, 200.0f));
			glPopMatrix();
		}
		if (gameOver == true)
		{
			glPushMatrix();
			theOGLWnd.setOrtho2D(windowWidth, windowHeight);
			theFontMgr->getFont("Space")->printText("GAME OVER", FTPoint(400, 400, 0.0f), colour3f(255.0f, 0.0f, 0.0f));
			glPopMatrix();

			seconds = 0;
		}
		if (game == true)
		{
			for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
			{
				if ((*enemyIterator)->isActive())
				{
					enemyMdl.renderMdl((*enemyIterator)->getPosition(), (*enemyIterator)->getRotation(), (*enemyIterator)->getScale());
					(*enemyIterator)->update(elapsedTime);
				}
			}

			playerMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
			thePlayer.update(elapsedTime);

			for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
			{
				if ((*laserIterartor)->isActive())
				{
					theLaser.renderMdl((*laserIterartor)->getPosition(), (*laserIterartor)->getRotation(), (*laserIterartor)->getScale());
					(*laserIterartor)->update(elapsedTime);
				}
			}

			outputMsg = to_string(theEnemy.size()); // convert float to string
			timerMsg = to_string(seconds); // convert float to string



			if (mute == true)
			{
				muteMsg = ("Mute");
			}
			else
			{
				muteMsg = ("");
			}

			glPushMatrix();
			theOGLWnd.setOrtho2D(windowWidth, windowHeight);
			theFontMgr->getFont("Space")->printText("Runaway Shapes", FTPoint(10, 35, 0.0f), colour3f(0.0f, 30.0f, 200.0f));
			theFontMgr->getFont("Space")->printText(outputMsg.c_str(), FTPoint(10, 80, 0.0f), colour3f(0.0f, 30.0f, 200.0f));
			theFontMgr->getFont("DrWho")->printText("Time: ", FTPoint(850, 35, 0.0f), colour3f(0.0f, 30.0f, 200.0f));
			theFontMgr->getFont("DrWho")->printText(timerMsg.c_str(), FTPoint(945, 35, 0.0f), colour3f(255.0f, 255.0f, 0.0f)); // uses c_str to convert string to LPCSTR
			theFontMgr->getFont("SevenSeg")->printText(muteMsg.c_str(), FTPoint(850, 70, 0.0f), colour3f(255.0f, 0.0f, 0.0f));
			glPopMatrix();

			gameOver = false;
		}

		pgmWNDMgr->swapBuffers();

		tCount += elapsedTime;

		//Clear key buffers
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
