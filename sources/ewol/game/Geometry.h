/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_GEOMETRY_H__
#define __GAME_GEOMETRY_H__

#include <ewol/debug.h>
#include <ewol/game/Bounding.h>
#include <ewol/game/Mass.h>


namespace game
{
	class Geometry
	{
		private :
			bool                m_dynamic;  //!< Set at true if the Geometry can move or not (bounding can change)
			game::MeshObject&   m_object;   //!< Object vertex/ normal and other properties
			game::Bounding*     m_bounding; //!< Bounding interface.
			game::Mass*         m_mass;     //!< Mass properties.
		public:
			/**
			 * @brief main constructor
			 * @param[in] object the reference mesh object (must not be NULL)
			 */
			Geometry(game::MeshObject& object);
			/**
			 * @brief main destructor
			 */
			~Geometry(void);
			/**
			 * @brief Set this object static (the bounding does not change)
			 */
			void SetStatic(void);
			/**
			 * @brief Set this object Dynamic (the bounding change...)
			 */
			void SetDynamic(void);
			/**
			 * @brief Set bounding type
			 * @param[in] type Selected bounding methode (if no set, by default it does not have one)
			 */
			void SetBoundingMode(game::boundingMode type);
			/**
			 * @brief Update the bounding properties.
			 */
			void BoundingUpdate(void);
	};
};


#endif
