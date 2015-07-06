/**
@author Cesar Manuel Paz Guzman
@date Enero, 2015
*/


#ifndef __Logic_Directional_H
#define __Logic_Directional_H

#include "Logic/Entity/Component.h"


namespace Graphics
{
	class CLight;
	class CScene;
}
//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la luz directional. La entidad que tenga este componente asociado, tendr� asociado
	una luz. Desde el fichero Map.txt puede ser configurada en las diferentes luces de ogre:
*/
	class CDirectional : public IComponent
	{
		DEC_FACTORY(CDirectional);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CDirectional(): IComponent(){}
		
		virtual ~CDirectional();
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Metodo que sirve para setear el entityInfo y el map en donde sera respawneada. No pongo solo la posicion, sino mas bien
		el entityInfo entero, porque puede ocurrir que queramos setear por ejemplo, la vida que tenga un enemigo, dado
		que los enemigos se haran mas fuertes. 

		@param map Mapa Logic en el que se registrara la entidad
		@param entity Informacion de construccion de la entidad leida del fichero
		@return Cierto si el respawn ha sido satisfatorio
		**/
		virtual bool respawn(const Map::CEntity *entity);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		La implementaci�n registrar� al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cron�metro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementaci�n eliminar� al componente de algunos observers en los 
		que pueda estar registrado (como el cron�metro del sistema, etc.).m
		*/
		virtual void deactivate();

	protected:

		/**
		Color difuso de la luz
		*/
		Vector3 diff;

		/**
		Color especular de la luz
		*/
		Vector3 spec;

		/**
		Direcci�n de la luz
		*/
		Vector3 dir;

		Graphics::CLight *light;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las luces.
		*/
		Graphics::CScene* _scene;

	}; // class CLights

	REG_FACTORY(CDirectional);

} // namespace Logic

#endif // __Logic_Lights_H
