/**
 *******************************************************************************
 * @file ewol/Game/GameElement.h
 * @brief ewol game element system (header)
 * @author Edouard DUPIN
 * @date 06/04/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

#ifndef __EWOL_GAME_ELEMENT_H__
#define __EWOL_GAME_ELEMENT_H__

#include <etk/Types.h>
#include <ewol/Debug.h>
#include <ewol/OObject/Sprite.h>
#include <ewol/Widget.h>


namespace ewol {
	class GameElement
	{
		protected:
			bool       m_visible;
			coord2D_ts m_position;
			coord2D_ts m_acceleration;
			etkFloat_t m_size;
			etkFloat_t m_angle;
		public:
			                    GameElement(void);
			virtual             ~GameElement(void);
			        bool        IsVisible(void)                                { return m_visible; };
			        void        SetVisible(bool state)                         { m_visible = state; };
			        coord2D_ts  PositionGet(void)                              { return m_position; };
			        void        PositionSet(coord2D_ts state)                  { m_position = state; };
			        void        PositionSet(etkFloat_t xxx, etkFloat_t yyy)    { m_position.x = xxx; m_position.y = yyy; };
			        coord2D_ts  AccelerationGet(void)                          { return m_acceleration; };
			        void        AccelerationSet(coord2D_ts state)              { m_acceleration = state; };
			        etkFloat_t  SizeGet(void)                                  { return m_size; };
			        void        SizeSet(etkFloat_t state)                      { m_size = state; };
			        etkFloat_t  AngleGet(void)                                 { return m_angle; };
			        void        AngleSet(etkFloat_t state)                     { m_angle = state; };
			virtual bool        Process(int64_t time, int32_t deltaTime, etk::VectorType<ewol::GameElement*> & listOfElement);
			virtual void        Draw(etk::VectorType<ewol::Sprite*> & listOfSprite, etk::VectorType<ewol::Sprite*> & listOfEffects);
			virtual void        RemoveElement(int32_t idOfElement);
					int32_t     LoadSprite(etk::VectorType<ewol::Sprite*> listOfElement[NB_BOUBLE_BUFFER], etk::UString fileName, coord2D_ts maxSize);
	};
	
};

#endif
