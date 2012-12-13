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

namespace game
{
	class Engine
	{
		private:
			//game::Map*         m_map;        //!< basic system map (BSD or other ...)
			etk::Vector<game::Element*> m_elementsStatic;
			etk::Vector<game::Element*> m_elementsDynamic;
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
			void Process(int64_t lastTime, int32_t deltaTime);
			/**
			 * @brief Display the environement.
			 */
			void Draw(void);
	};
};


#endif

