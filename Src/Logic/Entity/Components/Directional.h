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
//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la luz directional. La entidad que tenga este componente asociado, tendrá asociado
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
		Inicialización del componente usando la descripción de la entidad que hay en 
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
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		La implementación registrará al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cronómetro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementación eliminará al componente de algunos observers en los 
		que pueda estar registrado (como el cronómetro del sistema, etc.).m
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
		Dirección de la luz
		*/
		Vector3 dir;

		Graphics::CLight *light;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las luces.
		*/
		Graphics::CScene* _scene;

	}; // class CLights

	REG_FACTORY(CDirectional);

} // namespace Logic

#endif // __Logic_Lights_H
