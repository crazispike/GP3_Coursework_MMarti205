#include "cPlayer.h"

cPlayer::cPlayer() : cModel()
{

}


void cPlayer::attachInputMgr(cInputMgr* inputMgr)
{
	m_InputMgr = inputMgr;
}

void cPlayer::update(float elapsedTime)
{
	if (m_InputMgr->isKeyDown(68)) // D
	{
		m_mdlPosition.x += 5.0f;
		lane++;	

		if (lane == 3)
		{
			lane = 2;
			m_mdlPosition.x -= 5.0f;
		}
	}
	if (m_InputMgr->isKeyDown(65)) // A
	{
		m_mdlPosition.x -= 5.0f;
		lane--;
		if (lane == -3)
		{
			lane = -2;
			m_mdlPosition.x += 5.0f;
		}
	}
	if (m_InputMgr->isKeyDown(87)) // W
	{
		m_mdlPosition.z += 5.0f;
		lane2++;
		if (lane2 == 2)
		{
			lane2 = 1;
			m_mdlPosition.z -= 5.0f;
		}
	}
	if (m_InputMgr->isKeyDown(83)) // S
	{
		m_mdlPosition.z -= 5.0f;
		lane2--;
		if (lane2 == -2)
		{
			lane2 = -1;
			m_mdlPosition.z += 5.0f;
		}
	}

	if (m_InputMgr->isKeyDown(VK_SHIFT))
	{
		glm::vec3 mdlLaserDirection;
		mdlLaserDirection.x = -(float)glm::sin(glm::radians(this->getRotation()));
		mdlLaserDirection.y = 2.0f;
		mdlLaserDirection.z = -(float)glm::cos(glm::radians(this->getRotation()));
		mdlLaserDirection *= -1;

		// Add new bullet sprite to the vector array
		theTardisLasers.push_back(new cLaser);
		int numLasers = theTardisLasers.size() - 1;
		theTardisLasers[numLasers]->setDirection(mdlLaserDirection);
		theTardisLasers[numLasers]->setRotation(0.0f);
		theTardisLasers[numLasers]->setScale(glm::vec3(1, 1, 1));
		theTardisLasers[numLasers]->setSpeed(5.0f);
		theTardisLasers[numLasers]->setPosition(this->getPosition() + mdlLaserDirection);
		theTardisLasers[numLasers]->setIsActive(true);
		theTardisLasers[numLasers]->update(elapsedTime);
		// play the firing sound
		m_SoundMgr->getSnd("Shot")->playAudio(AL_TRUE);
	} 

	if (m_InputMgr->isKeyDown(VK_SPACE))
	{
		cameraMode++;
		if (cameraMode == 2)
		{
			cameraMode = 0;
		}
	}

	if (m_InputMgr->isKeyDown(77)) // M
	{
		if (mute == false)
		{
			mute = true;
		}
		else if (mute == true)
		{
			mute = false;
		}
	}

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
	{
		(*laserIterartor)->update(elapsedTime);
		for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
		{
			if ((*enemyIterator)->SphereSphereCollision((*laserIterartor)->getPosition(), (*laserIterartor)->getMdlRadius()))
			{
				// if a collision set the bullet and spaceship to false
				(*enemyIterator)->setIsActive(false);
				(*laserIterartor)->setIsActive(false);
				// play the explosion sound.
				m_SoundMgr->getSnd("Explosion")->playAudio(AL_TRUE);
			}
		}
	}

	vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin();
	while (laserIterartor != theTardisLasers.end())
	{
		if ((*laserIterartor)->isActive() == false)
		{
			laserIterartor = theTardisLasers.erase(laserIterartor);
		}
		else
		{
			++laserIterartor;
		}
	}

	vector<cEnemy*>::iterator enemyIterartor = theEnemy.begin();
	while (enemyIterartor != theEnemy.end())
	{
		if ((*enemyIterartor)->isActive() == false)
		{
			enemyIterartor = theEnemy.erase(enemyIterartor);
		}
		else
		{
			++enemyIterartor;
		}
	}



	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = 1.0f;  //-(float)glm::sin(glm::radians(m_mdlRotation));  // Remember to adjust for radians
	mdlVelocityAdd.y = 1.0f;  // was 0
	mdlVelocityAdd.z = 1.0f; //(float)glm::cos(glm::radians(m_mdlRotation));

	m_mdlRotation -= rotationAngle;

	mdlVelocityAdd *= translationX;  
	m_mdlDirection += mdlVelocityAdd;

	m_mdlPosition += m_mdlDirection * m_mdlSpeed *elapsedTime;
	m_mdlDirection *= 0.95f;

	rotationAngle = 0;
	translationX = 0;
	translationY = 0;
	translationZ = 0;
}

cPlayer::~cPlayer()
{

}