/*
@filename Bank.cpp

Contiene la definición del banco de sonidos de FMOD Studio
@author Alberto Martínez Villarán
@date Marzo, 2015
*/


#include "Bank.h"

#include "Server.h"
#include "BaseSubsystems/Log.h"
#include <cassert>

//includes de FMOD
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.hpp>

namespace Sounds
{
	///definición nazi de ERRCHECK sacado de los samples
	void ERRCHECK_bank(FMOD_RESULT result, const char *file, int line);
	#define ERRCHECK(_result) ERRCHECK_bank(_result, __FILE__, __LINE__)

	void ERRCHECK_bank(FMOD_RESULT result, const char *file, int line)
	{
#ifdef _DEBUG
		if (result != FMOD_OK)
		{
			BaseSubsystems::Log::Error(FMOD_ErrorString(result));
		}
#endif
	}


	CBank::CBank(const std::string &bankName) : _bankName(bankName)
	{
		_server = CServer::getSingletonPtr();
		if(_server)
		{
			std::string path = _server->getSoundPath() + bankName + std::string(".bank");
			_bank = NULL;
			FMOD_RESULT result = _server->getStudioSystem()->loadBankFile(path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &_bank);
			ERRCHECK(result);

			//Podemos cargar si queremos toda la información de los samples de golpe
			/*result = _bank->loadSampleData();
			ERRCHECK(result);*/
		}
	}

	CBank::~CBank()
	{
		//hay que hacer deletes???
		_bank->unloadSampleData();
		_bank->unload();
		_server = 0;
		//delete _bank;
	}


};//namespace Sounds