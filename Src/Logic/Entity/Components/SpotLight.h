/**
@author Cesar Manuel Paz Guzman
@date Enero, 2015
*/


#ifndef __Logic_SpotLight_H
#define __Logic_SpotLight_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Entity.h"


namespace Graphics
{
	class CLight;
	class CScene;
}
//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la luz focal (Spotlight). La entidad que tenga este componente asociado, tendrá asociado
	una luz. Desde el fichero Map.txt puede ser configurada en las diferentes luces de ogre:
*/
	class CSpotLight : public IComponent, public CEntityTransformListener
	{
		DEC_FACTORY(CSpotLight);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSpotLight() : IComponent(){}
		
		virtual ~CSpotLight();
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

		/**
		Este componente sólo acepta mensajes de tipo SET_POSITION. Estos mensajes  
		sólo se utilizan para mover entidades de tipo cinemático.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Cuando recibe mensajes de tipo SET_POSITION cambia la posicion de la luz en la escena
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		///Método de escucha de la posicion de la entidad
		void OnEntitySetTransform(const Matrix4 &transform);

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

		/**
		Rango de la luz
		*/
		Vector2 range;


		Graphics::CLight *light;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las luces.
		*/
		Graphics::CScene* _scene;

	}; // class CLights

	REG_FACTORY(CSpotLight);

} // namespace Logic

#endif // __Logic_Lights_H
