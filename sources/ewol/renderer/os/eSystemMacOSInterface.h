/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __C_INTERFACE_MAC_OS_H__
#define __C_INTERFACE_MAC_OS_H__

#include <etk/types.h>


#ifdef __cplusplus
extern "C" {
#endif

void eSystem_SetArchiveDir(int mode, const char* str);

void eSystem_Resize(int w, int h );
void eSystem_Move(int x, int y );
void eSystem_Hide(void);
void eSystem_Show(void);

void eSystem_SetInputMotion(int pointerID, float x, float y);
void eSystem_SetInputState(int pointerID, bool isDown, float x, float y);

void eSystem_SetMouseMotion(int pointerID, float x, float y);
void eSystem_SetMouseState(int pointerID, bool isDown, float x, float y);
/*
void eSystem_SetKeyboard(ewol::SpecialKey& special,
                 uniChar_t myChar,
                 bool isDown,
                 bool isARepeateKey=false);
void eSystem_SetKeyboardMove(ewol::SpecialKey& special,
                     ewol::keyEvent::keyboard_te move,
                     bool isDown,
                     bool isARepeateKey=false);
void eSystem_ClipBoardArrive(ewol::clipBoard::clipboardListe_te clipboardID);
*/
bool eSystem_Draw(bool displayEveryTime);
void eSystem_OpenGlContextDestroy(void);
void eSystem_OnKill(void);


#ifdef __cplusplus
}
#endif

#endif

