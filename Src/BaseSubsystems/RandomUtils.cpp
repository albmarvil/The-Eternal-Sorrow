//---------------------------------------------------------------------------
// RandomUtils.cpp
//---------------------------------------------------------------------------

/**
@file RandomUtils.cpp


@see BaseSubsystems::RandomUtils

@author Alberto Martínez Villarán
@date Abril, 2015
*/


#include "RandomUtils.h"
#include "LUA\ScriptManager.h"

using namespace std;

namespace BaseSubsystems {

	int RandomUtils::_seed = -1;

	int RandomUtils::_id = 0;

	std::default_random_engine RandomUtils::_generator = std::default_random_engine();

	float RandomUtils::Random()
	{
		std::uniform_real_distribution<float> distribution(0, 1);

		//float res = distribution(_generator);
		//_id++;
		//Log::Debug(std::to_string(_id) + " - RND()"+std::to_string(res));
		//return res;

		return distribution(_generator);
	}


	int RandomUtils::RandomRange(int min, int max)
	{
		std::uniform_int_distribution<int> distribution(min, max);

		//int res = distribution(_generator);
		//_id++;
		//Log::Debug(std::to_string(_id) + " - RND("+std::to_string(min)+", "+std::to_string(max)+")"+std::to_string(res));
		//return res;

		return distribution(_generator);
	}

	void RandomUtils::setRandomSeed(int seed)
	{
		_seed = seed;
		_generator.seed(seed);
		//Log::Debug("RandomSeed: "+std::to_string(seed));	
	}

};///BaseSubsystems

