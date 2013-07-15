#include "GameManager.h" 

using namespace cocos2d;
//using namespace CocosDenshion;

//All static variables need to be defined in the .cpp file
//I've added this following line to fix the problem
GameManager* GameManager::m_mySingleton = NULL;

GameManager::GameManager()
{    
    
}

GameManager* GameManager::sharedGameManager()
{
    //If the singleton has no instance yet, create one
    if(NULL == m_mySingleton)
    {
        //Create an instance to the singleton
        m_mySingleton = new GameManager();
    }
    
    //Return the singleton object
    return m_mySingleton;
}
void GameManager::setBgm(bool bgm)
{
    this->bgm = bgm;
}
bool GameManager::getBgm()
{
    return this->bgm;
}
string GameManager::getName()
{
    return this->name;
}
void GameManager::setName(string name)
{
    this->name = name;
}

string GameManager::getEmail()
{
    return this->email;
}
void GameManager::setEmail(string email)
{
    this->email = email;
}

int GameManager::getLevel()
{
    return this->level;
}
void GameManager::setLevel(int level)
{
    this->level = level;
}

int GameManager::getPoint()
{
    return this->point;
}
void GameManager::setPoint(int point)
{
    this->point = point;
}
