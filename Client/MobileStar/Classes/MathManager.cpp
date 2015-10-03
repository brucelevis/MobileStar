#include "MathManager.h"

MathManager* MathManager::Instance(){
	static MathManager instance;
	return &instance;
}

MathManager::MathManager() : MaxInt((std::numeric_limits<int>::max)()),
													MaxDouble((std::numeric_limits<double>::max)()),
													MinDouble((std::numeric_limits<double>::min)()),
													MaxFloat((std::numeric_limits<float>::max)()),
													MinFloat((std::numeric_limits<float>::min)()),
													Pi(3.14159265359f)
{
	for(int i=0; i<360;++i){
		m_Sin[i] = sin(DegToRad(static_cast<float>(i)));
		m_Cos[i] = cos(DegToRad(static_cast<float>(i)));
	}
}
MathManager::~MathManager(){
}
int MathManager::RandInt(int x, int y){
	return rand()%(y-x+1) + x;
}
float MathManager::RandFloat(float x, float y){
	return x + (rand()/(RAND_MAX+1.0f)) * (y-x);
}
