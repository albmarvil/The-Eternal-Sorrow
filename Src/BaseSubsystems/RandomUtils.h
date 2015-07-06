//---------------------------------------------------------------------------
// RandomUtils.h
//---------------------------------------------------------------------------

/**
@file RandomUtils.h



@see BaseSubsystems::RandomUtils

@author Alberto Martínez Villarán
@date Abril, 2015
*/


#ifndef __BaseSubsystems_RandomUtils_H
#define __BaseSubsystems_RandomUtils_H

#include <random>
#include <time.h>

namespace BaseSubsystems {


	class RandomUtils
	{

	public:

		/*
		Setea la semilla aleatoria
		*/
		static void setRandomSeed(int seed);

		/*
		Random clásico entre 0 y 1
		*/
		static float Random();

		/*
		Random clásico de rango
		*/
		static int RandomRange(int min, int max);

	protected:

		static std::default_random_engine _generator;

		static int _id;

		static int _seed;

	};


};///BaseSubsystems

#endif