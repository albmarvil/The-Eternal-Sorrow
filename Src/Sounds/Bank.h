/*
@filename Bank.h

Contiene la definición del banco de sonidos de FMOD Studio

@author Alberto Martínez Villarán
@date Marzo, 2015
*/

#ifndef __Sounds_Bank_H
#define __Sounds_Bank_H

#include <string>

namespace FMOD
{
	namespace Studio
	{
		class Bank;
	}
}
namespace Sounds
{
	class CServer;
}

namespace Sounds
{

	class CBank
	{
	public:

		CBank(const std::string &bankName);

		virtual ~CBank();

	protected:

		CServer* _server;

		FMOD::Studio::Bank* _bank;

		std::string _bankName;


	};//CServer

};//namespace Sounds

#endif //__Sounds_Server_H