/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_ENGINE_H__
#define __GAME_ENGINE_H__


#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/game/Element.h>
#include <ewol/game/Gravity.h>
#include <ewol/game/Contact.h>
#include <ewol/widget/Widget.h>



namespace game
{
	class Engine
	{
		private:
			etk::Vector<game::Gravity>  m_gravity;          //!< list of gravity element
			etk::Vector<game::Element*> m_elementsStatic;   //!< List of the game element (bounding does not move)
			etk::Vector<game::Element*> m_elementsDynamic;  //!< List of the game element (change position all the time)
			etk::Vector<game::Contact>  m_contacts;         //!< list of all contact that existe in the system
		public:
			/**
			 * @brief Basic constructor.
			 */
			Engine(void);
			/**
			 * @brief Basic destructor.
			 */
			~Engine(void);
			/**
			 * @brief periodic call for processing.
			 * @param[in] lastTime Previous call time (if the system is in pause this time does restart at the same time the next time.
			 * @param[in] deltaTime delta time in µs from the previous call.
			 */
			void Process(double lastTime, float deltaTime);
			/**
			 * @brief Processing the gravity properties.
			 * @param[in] deltaTime delta time in µs from the previous call.
			 */
			void ProcessGravity(float deltaTime);
			/**
			 * @brief Processing the Artificial Intelligence.
			 * @param[in] deltaTime delta time in Âµs from the previous call.
			 */
			void ProcessIA(float deltaTime);
			/**
			 * @brief Processing the collision.
			 * @param[in] deltaTime delta time in µs from the previous call.
			 */
			void ProcessCollision(float deltaTime);
			/**
			 * @brief Display the environement.
			 */
			void Draw(ewol::DrawProperty& displayProp);
			/**
			 * @brief Add an element on the system.
			 * @param[in] newElement element to display.
			 * @param[in] dynamic this element change of place.
			 */
			void AddElement(game::Element* newElement, bool dynamic);
			/**
			 * @brief Add a gravity on the system.
			 * @param[in] gravity The gravity to add.
			 */
			void AddGravity(game::Gravity gravity);
	};
};


#endif

