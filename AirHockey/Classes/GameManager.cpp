#include "GameManager.h" 

using namespace cocos2d;
//using namespace CocosDenshion;

//All static variables need to be defined in the .cpp file
//I've added this following line to fix the problem
GameManager* GameManager::m_mySingleton = NULL;

GameManager::GameManager()
{    
    this->setIpAddr("192.168.1.122");
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
    CCUserDefault::sharedUserDefault()->setBoolForKey("BGM", bgm);
    CCUserDefault::sharedUserDefault()->flush();
}
bool GameManager::getBgm()
{
    return CCUserDefault::sharedUserDefault()->getBoolForKey("BGM");
}
string GameManager::getName()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey("PlayerName");
}
void GameManager::setName(string name)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("PlayerName", name);
    CCUserDefault::sharedUserDefault()->flush();
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
string GameManager::getIpAddr()
{
    return this->ipAddr;
}
void GameManager::setIpAddr(string ipAddr) {
    this->ipAddr = ipAddr;
}
