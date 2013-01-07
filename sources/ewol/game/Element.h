/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __GAME_ELEMENT_H__
#define __GAME_ELEMENT_H__


#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/Vector.h>
#include <ewol/debug.h>
#include <ewol/game/Gravity.h>
#include <ewol/renderer/resources/Mesh.h>
#include <ewol/game/BoundingAABB.h>

namespace game
{
	class Engine;
	class Element
	{
		protected:
			game::Engine*       m_engine;
			ewol::Mesh*         m_resource;   //!< Resource to display the element.
			game::BoundingAABB  m_bounding;   //!< Bounding of this element
		private:
			mat4     m_matrix;           //!< generated display matrix.
			vec3     m_scale;            //!< scale of the element. (change the size in dynamic from the loaded model)
			mat4     m_displayRotation;  //!< Associated matrix of totation associated with m_displayAngle.
		
		protected:
			// specific for the physical engine :
			vec3     m_position;         //!< position of the element. (in m)
			vec3     m_speed;            //!< Speed of the element. (in m/s)
			vec3     m_speedMax;         //!< Speed maximum limitation. (in m/s)
			float    m_mass;             //!< object mass (in kg)
			vec3     m_gravityForce;     //!< curent gravity force in newton of the object (m/s^2)
			vec3     m_userAcceleration; //!< the only one parameter that the user can change (m/s^2), if the coder want that the equation do not take stipid things ...
			
		protected:
			uint32_t m_uniqueId; //!< General element ID (uint16_t, because all is reference with the groupId like this only a uint32_t reference an element)
			uint32_t m_groupId;  //!< General group Id More than 65000 group can be really interesting to create supid game ...
			int32_t  m_type;     //!< type of this element
			bool     m_visible;  //!< This is to know if the element is displayed or not ==> TODO : check if usefull ...
			bool     m_static;   //!< This element is static...
		public:
			/**
			 * @brief Basic constructor.
			 * @param[in] meshResource Resource name.
			 */
			Element(etk::UString meshResource);
			/**
			 * @brief Basic destructor.
			 */
			virtual ~Element(void);
			/**
			 * @brief Link with a specific engine.
			 * @param[in] engine Engine pointer
			 */
			void SetEngine(game::Engine* engine) { m_engine = engine; };
			/**
			 * @brief Draw the element.
			 */
			virtual void Draw(void);
			/**
			 * @brief Draw Debug information of the element.
			 */
			virtual void DrawDebug(void);
			/**
			 * @brief Process IA of this element.
			 * @param[in] deltaMicroSecond delta from the last call.
			 * @return true if this element must be destroyed
			 */
			virtual bool ArtificialIntelligence(float delta);
			/**
			 * @brief Clear the current gravity reference
			 */
			void ProcessGravityClear(void);
			/**
			 * @brief Update the gravity properties.
			 * @param[in] delta delta from the last call.
			 * @param[in] gravity reference on all the gravity point.
			 */
			virtual void ProcessGravity(float delta, game::Gravity& gravity);
			/**
			 * @brief Update the position of the element
			 * @param[in] delta delta from the last call.
			 */
			virtual void ProcessPosition(float delta);
			/**
			 * @param reset the position / speed / angle / and result matrix
			 */
			void Identity(void);
			/**
			 * @brief Translate The curent element to a new position
			 * @param[in] vect new position
			 */
			void Translate(etk::Vector3D<float> vect);
			/**
			 * @brief Scale the element to an other size
			 * @param[in] vect new object scaling
			 */
			void Scale(etk::Vector3D<float> vect);
			/**
			 * @brief Scale the element to an other size
			 * @param[in] proportion scale value in all direction ...
			 */
			void Scale(float proportion);
			/**
			 * @brief Rotate the current object
			 * @param[in] vect rotation angle
			 * @param[in] angleRad radian angle
			 */
			void Rotate(etk::Vector3D<float> vect, float angleRad=0.0);
			/**
			 * @brief Get the Current matrix of the element (never access it directly, it might be sometime updated)
			 * @return the reference on the matrix (updated if needed)
			 */
			mat4& GetMatrix(void);
		private:
			/**
			 * @brief when position or rotation is set, the system call this function ...
			 */
			void UpdateMatrix(void);
		public:
			void SetSpeed(vec3 newSpeed)
			{
				m_speed = newSpeed;
			}
			
			game::BoundingAABB& GetBounding(void)
			{
				return m_bounding;
			}
			/**
			 * @brief Set this element Static or not
			 */
			void SetStaticMode(bool newMode) { m_static = newMode; };
			/**
			 * @brief Get the current static or dynamic mode
			 */
			bool GetStaticMode(void) { return m_static; };
			/**
			 * @brief A collision has bee detected ...
			 * @param[in] elementCollide the element that has been collided
			 * @param[in] penetrate The size of the penetration ...
			 */
			virtual void CollisionDetected(game::Element* elementCollide, vec3& penetrate);
	};
};


#endif
