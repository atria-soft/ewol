/**
 *******************************************************************************
 * @file guiAndroid.cpp
 * @brief Gui abstraction layer (Sources)
 * @author Edouard DUPIN
 * @date 20/10/2011
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



#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>

#include <ewol/Debug.h>
#include <etk/String.h>
#include <ewol/WidgetManager.h>
#include <ewol/base/gui.h>
#include <ewol/ewol.h>

#include <ewol/importgl.h>
#include <ewol/Texture.h>

#undef __class__
#define __class__ "AndroidJNI"


int   gAppAlive   = 1;

static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;

static etkFloat_t m_width = 320;
static etkFloat_t m_height = 480;

ewol::Windows* m_uniqueWindows = NULL;
static int64_t GetCurrentTime(void)
{
    struct timeval  now;
    gettimeofday(&now, NULL);
    //EWOL_VERBOSE("current time : " << now.tv_sec << "s " << now.tv_usec << "us");
    return (int64_t)((int64_t)now.tv_sec*(int64_t)1000 + (int64_t)now.tv_usec/(int64_t)1000);
}


void Draw(void);
#define OFFSET_MOVE_CLICKED             (20)
#define OFFSET_MOVE_CLICKED_DOUBLE      (40)
#undef SEPARATED_CLICK_TIME
#define SEPARATED_CLICK_TIME          (300)

bool    inputIsPressed[NB_MAX_INPUT];// = {false, false, false};
int32_t m_previousBouttonId = -1;
int32_t m_previousDown_x = -1;
int32_t m_previousDown_y = -1;
int32_t m_previous_x = -1;
int32_t m_previous_y = -1;
int64_t m_previousTime = 0;
bool    m_previousDouble = false;


/* Call to initialize the graphics state */
void EWOL_NativeInit(void)
{
	EWOL_INFO("Init : Start All Application");
	gAppAlive    = 1;
	sDemoStopped = 0;
	sTimeOffsetInit = 0;
	ewol::TextureOGLContext(true);
}


void EWOL_NativeResize(int w, int h )
{
	m_width = w;
	m_height = h;
	EWOL_INFO("Resize w=" << w << " h=" << h);
	ewol::TextureOGLContext(false);
	ewol::TextureOGLContext(true);
	if (NULL != m_uniqueWindows) {
		m_uniqueWindows->CalculateSize((etkFloat_t)m_width, (etkFloat_t)m_height);
		m_uniqueWindows->SetOrigin(0.0, 0.0);
	}
}

/* Call to finalize the graphics state */
void EWOL_NativeDone(void)
{
	EWOL_INFO("Renderer : Close All Application");
	ewol::TextureOGLContext(false);
}

void EWOL_NativeEventInputMotion(int pointerID, float x, float y )
{
	//EWOL_INFO("Event : Input Motion ID=" << pointerID << " x=" << x << " y=" << y);
	if(0<=pointerID && pointerID < NB_MAX_INPUT ) {
		if (true == inputIsPressed[pointerID]) {
			if(NULL != m_uniqueWindows) {
				//EWOL_DEBUG("ANDROID event: bt=" << pointerID+1 << " ** = \"MotionNotify\" (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
				m_uniqueWindows->GenEventInput(pointerID+1, ewol::EVENT_INPUT_TYPE_MOVE, (etkFloat_t)x, (etkFloat_t)y);
			}
		}
	}
}

void EWOL_NativeEventInputState(int pointerID, bool isUp, float x, float y )
{
	//EWOL_INFO("Event : Input ID=" << pointerID << " [" << isUp << "] x=" << x << " y=" << y);
	if (isUp) {
		//EWOL_INFO("Event : Input ID=" << pointerID << " [DOWN] x=" << x << " y=" << y);
		if(0<=pointerID && pointerID < NB_MAX_INPUT ) {
			// Send Down message
			if (NULL != m_uniqueWindows) {
				EWOL_DEBUG("ANDROID bt=" << pointerID+1 << " event : **=\"ButtonPress\"        (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
				m_uniqueWindows->GenEventInput(pointerID+1, ewol::EVENT_INPUT_TYPE_DOWN, (etkFloat_t)x, (etkFloat_t)y);
			}
			// Check double or triple click event ...
			m_previousDown_x = x;
			m_previousDown_y = y;
			if (m_previousBouttonId != pointerID+1) {
				m_previousBouttonId = pointerID+1;
				m_previous_x = -1;
				m_previous_y = -1;
				m_previousTime = 0;
				m_previousDouble = false;
			} else {
				if(    abs(m_previous_x - x) < OFFSET_MOVE_CLICKED
				    && abs(m_previous_y - y) < OFFSET_MOVE_CLICKED )
				{
					// nothink to do ... wait up ...
				} else {
					m_previous_x = -1;
					m_previous_y = -1;
					m_previousTime = 0;
					m_previousDouble = false;
				}
			}
		}
	} else {
		//EWOL_INFO("Event : Input ID=" << pointerID << " [UP]   x=" << x << " y=" << y);
		if(0<=pointerID && pointerID < NB_MAX_INPUT ) {
			// Send Down message
			if (NULL != m_uniqueWindows) {
				EWOL_DEBUG("ANDROID bt=" << pointerID+1 << " event : **=\"ButtonRelease\"      (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
				m_uniqueWindows->GenEventInput(pointerID+1, ewol::EVENT_INPUT_TYPE_UP, (etkFloat_t)x, (etkFloat_t)y);
			}
			if (m_previousBouttonId != pointerID+1) {
				m_previousDown_x = -1;
				m_previousDown_y = -1;
				m_previousBouttonId = 0;
				m_previous_x = -1;
				m_previous_y = -1;
				m_previousTime = 0;
				m_previousDouble = false;
			} else {
				int64_t currentTime = GetCurrentTime(); // return the tic in 10ms
				//EWOL_DEBUG("time is : " << (int)currentTime << "    "<< (int)(currentTime/100) <<"s " << (int)((currentTime%100)*10) << "ms");
				if (currentTime - m_previousTime >= SEPARATED_CLICK_TIME) {
					//check if the same area click : 
					if(    abs(m_previousDown_x - x) < OFFSET_MOVE_CLICKED
					    && abs(m_previousDown_y - y) < OFFSET_MOVE_CLICKED )
					{
						// might generate an sigle event :
						EWOL_DEBUG("ANDROID event : ** = \"ButtonClickedSingle\" (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
						m_uniqueWindows->GenEventInput(pointerID+1, ewol::EVENT_INPUT_TYPE_SINGLE, (etkFloat_t)x, (etkFloat_t)y);
						m_previous_x = m_previousDown_x;
						m_previous_y = m_previousDown_y;
						m_previousTime = currentTime;
					} else {
						// reset values ...
						m_previousDown_x = -1;
						m_previousDown_y = -1;
						m_previousBouttonId = 0;
						m_previous_x = -1;
						m_previous_y = -1;
						m_previousTime = 0;
					}
					m_previousDouble = false;
				} else {
					// TODO : the double ckick does not work, I need to check this later ... if needed
					//check if the same area click : 
					if(    abs(m_previous_x - x) < OFFSET_MOVE_CLICKED_DOUBLE
					    && abs(m_previous_y - y) < OFFSET_MOVE_CLICKED_DOUBLE )
					{
						// might generate an sigle event :
						if (false == m_previousDouble) {
							EWOL_DEBUG("ANDROID event : ** = \"ButtonClickedDouble\" (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
							m_uniqueWindows->GenEventInput(pointerID+1, ewol::EVENT_INPUT_TYPE_DOUBLE, (etkFloat_t)x, (etkFloat_t)y);
							m_previousTime = currentTime;
							m_previousDouble = true;
						} else {
							EWOL_DEBUG("ANDROID event : ** = \"ButtonClickedTriple\" (" << (etkFloat_t)x << "," << (etkFloat_t)y << ")");
							m_uniqueWindows->GenEventInput(pointerID+1, ewol::EVENT_INPUT_TYPE_TRIPLE, (etkFloat_t)x, (etkFloat_t)y);
							// reset values ...
							m_previousDown_x = -1;
							m_previousDown_y = -1;
							m_previousBouttonId = 0;
							m_previous_x = -1;
							m_previous_y = -1;
							m_previousTime = 0;
							m_previousDouble = false;
						}
					} else {
						// reset values ...
						m_previousDown_x = -1;
						m_previousDown_y = -1;
						m_previousBouttonId = 0;
						m_previous_x = -1;
						m_previous_y = -1;
						m_previousTime = 0;
						m_previousDouble = false;
					}
				}
			}
		}
	}
}

void EWOL_NativeEventUnknow(int eventID)
{
	EWOL_WARNING("Event : Unknow ID=" << eventID);
}


void EWOL_NativeParamSetArchiveDir(int mode, const char* str)
{
	switch(mode)
	{
		case 0:
			EWOL_WARNING("Directory APK : path=" << str);
			etk::SetBaseFolderData(str);
			break;
		case 1:
			EWOL_WARNING("Directory mode=FILE path=" << str);
			etk::SetBaseFolderDataUser(str);
			break;
		case 2:
			EWOL_WARNING("Directory mode=CACHE path=" << str);
			etk::SetBaseFolderCache(str);
			break;
		case 3:
			EWOL_WARNING("Directory mode=EXTERNAL_CACHE path=" << str);
			break;
		default:
			EWOL_WARNING("Directory mode=???? path=" << str);
			break;
	}
}


static bool isAlreadyInit = false;

void EWOL_NativeApplicationInit(void)
{
	EWOL_WARNING("Event : Init Application");
	if (false == isAlreadyInit) {
		guiAbstraction::Init(0, NULL);
		ewol::Init(0, NULL);
		APP_Init(0, NULL);
		isAlreadyInit = true;
	}
}

void EWOL_NativeApplicationUnInit(void)
{
	EWOL_WARNING("Event : UnInit application");
	// unset all windows
	ewol::DisplayWindows(NULL);
	// call application to uninit
	APP_UnInit();
	// basic abstraction un-init
	guiAbstraction::UnInit();
	// uninit Ewol
	ewol::UnInit();
}

/* Call to render the next GL frame */
void EWOL_NativeRender(void)
{
	long   curTime;

	/* NOTE: if sDemoStopped is TRUE, then we re-render the same frame
	 *       on each iteration.
	 */
	if (sDemoStopped) {
		curTime = sTimeStopped + sTimeOffset;
	} else {
		curTime =GetCurrentTime() + sTimeOffset;
		if (sTimeOffsetInit == 0) {
			sTimeOffsetInit = 1;
			sTimeOffset     = -curTime;
			curTime         = 0;
		}
	}
	
	Draw();
}





static void Setwindow(ewol::Windows* newWindows)
{
	m_uniqueWindows = newWindows;
	if (NULL != m_uniqueWindows) {
		m_uniqueWindows->CalculateSize((etkFloat_t)m_width, (etkFloat_t)m_height);
	}
}



static etkFloat_t gTriangleVertices[] = { 0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 200.0f };
static etkFloat_t gTriangleVertices5[] = { 200.0f, 200.0f, 100.0f, 200.0f, 200.0f, 100.0f,
                                           200.0f, 200.0f, 300.0f, 200.0f, 200.0f, 300.0f };

void Draw(void)
{
	//EWOL_DEBUG("redraw (" << m_width << "," << m_height << ")");
	if(NULL == m_uniqueWindows) {
		// set the size of the open GL system
		glViewport(0,0,m_width,m_height);
		
		// Clear the screen with transparency ...
		glClearColor(0.0,0.0,0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(0., width, 0., -height, 1., 20.);
		glOrtho(-m_width/2, m_width/2, m_height/2, -m_height/2, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		//glTranslatef(0, -height/2, -5);
		glTranslatef(-m_width/2, -m_height/2, -1.0);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
		//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
		
		glEnableClientState( GL_VERTEX_ARRAY );
		
		//LOGI("engine_draw_frame (%d,%d)",width,height);
		
		glColor4f(0.0, 1.0, 1.0, 1.0);
		glVertexPointer(2, oglTypeFloat_t, 0, gTriangleVertices5 );
		glDrawArrays( GL_TRIANGLES, 0, 6);
		static int vallllll = 0;
		static float transparency = 0.0;
		if (vallllll <= 1) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(1.0, 0.0, 0.0, 1.0);
			} else {
				glColor4f(1.0, 0.0, 0.0, transparency);
			}
		} else if (vallllll <= 2) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(1.0, 1.0, 0.0, 1.0);
			} else {
				glColor4f(1.0, 1.0, 0.0, transparency);
			}
		} else if (vallllll <= 3) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(0.0, 1.0, 0.0, 1.0);
			} else {
				glColor4f(0.0, 1.0, 0.0, transparency);
			}
		} else if (vallllll <= 4) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(0.0, 1.0, 1.0, 1.0);
			} else {
				glColor4f(0.0, 1.0, 1.0, transparency);
			}
		} else if (vallllll <= 5) {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll++;
				transparency = 0.0;
				glColor4f(0.0, 0.0, 1.0, 1.0);
			} else {
				glColor4f(0.0, 0.0, 1.0, transparency);
			}
		} else {
			transparency +=0.025;
			if (transparency >= 1.0) {
				vallllll = 0;
				transparency = 0.0;
				glColor4f(1.0, 0.0, 1.0, 1.0);
			} else {
				glColor4f(1.0, 0.0, 1.0, transparency);
			}
		}
		glVertexPointer(2, oglTypeFloat_t, 0, gTriangleVertices );
		glDrawArrays( GL_TRIANGLES, 0, 3);
		
		glDisableClientState( GL_VERTEX_ARRAY );
	
		glDisable(GL_BLEND);
	} else {
		m_uniqueWindows->SysDraw();
	}
	glFlush();
}




#undef __class__
#define __class__ "guiAbstraction"

static bool guiAbstractionIsInit = false;
//static guiAbstraction::X11systemInterface * myX11Access = NULL;


void guiAbstraction::Init(int32_t argc, char *argv[])
{
	if (false == guiAbstractionIsInit) {
		// set the gui is init :
		guiAbstractionIsInit = true;
		EWOL_INFO("INIT for X11 environement");
		//myX11Access = new guiAbstraction::X11systemInterface();
	} else {
		EWOL_CRITICAL("Can not INIT X11 ==> already init before");
	}
}


void guiAbstraction::Run(void)
{
	EWOL_INFO("Noting to run in android mode ...");
}

void guiAbstraction::Stop(void)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->Stop();
	} else {
		EWOL_CRITICAL("Can not Stop X11 ==> not init ... ");
	}
}

void guiAbstraction::SetDisplayOnWindows(ewol::Windows * newOne)
{
	if (true == guiAbstractionIsInit) {
		Setwindow(newOne);
	} else {
		EWOL_CRITICAL("Can not set Windows X11 ==> not init ... ");
	}
}

void guiAbstraction::UnInit(void)
{
	if (true == guiAbstractionIsInit) {
		EWOL_INFO("UN-INIT for X11 environement");
		//if (NULL != myX11Access) {
		//	delete(myX11Access);
		//}
		guiAbstractionIsInit = false;
	} else {
		EWOL_CRITICAL("Can not Un-Init X11 ==> not init ... ");
	}
}


void guiAbstraction::ChangeSize(int32_t w, int32_t h)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->ChangeSize(w, h);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::ChangePos(int32_t x, int32_t y)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->ChangePos(x, y);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

void guiAbstraction::GetAbsPos(int32_t & x, int32_t & y)
{
	if (true == guiAbstractionIsInit) {
		//myX11Access->GetAbsPos(x, y);
	} else {
		EWOL_CRITICAL("X11 ==> not init ... ");
	}
}

bool guiAbstraction::IsPressedInput(int32_t inputID)
{
	//if (true == guiAbstractionIsInit) {
	//	return myX11Access->IsPressedInput(inputID);
	//} else {
	//	EWOL_CRITICAL("X11 ==> not init ... ");
		return false;
	//}
}

void guiAbstraction::KeyboardShow(ewol::keyboardMode_te mode)
{
	if (NULL != m_uniqueWindows) {
		m_uniqueWindows->KeyboardShow(mode);
	}
}

void guiAbstraction::KeyboardHide(void)
{
	if (NULL != m_uniqueWindows) {
		m_uniqueWindows->KeyboardHide();
	}
}

void guiAbstraction::ForceRedrawAll(void)
{
	if (NULL != m_uniqueWindows) {
		m_uniqueWindows->CalculateSize((etkFloat_t)m_width, (etkFloat_t)m_height);
	}
}


void guiAbstraction::SendKeyboardEvent(bool isDown, etk::String &keyInput)
{
	// Get the current Focused Widget :
	ewol::Widget * tmpWidget = ewol::widgetManager::FocusGet();
	if (NULL != tmpWidget) {
		if(true == isDown) {
			EWOL_DEBUG("X11 PRESSED : \"" << keyInput << "\" size=" << keyInput.Size());
			tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_DOWN, keyInput.c_str());
		} else {
			EWOL_DEBUG("X11 Release : \"" << keyInput << "\" size=" << keyInput.Size());
			tmpWidget->OnEventKb(ewol::EVENT_KB_TYPE_UP, keyInput.c_str());
		}
	}
}

// never had main in android ...
/*
int main(int argc, char *argv[])
{
	return -1;
}
*/

void glOrtho(GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat nearVal,
             GLfloat farVal)
{
	GLfloat myMatrix[4*4];
	int iii;
	for(iii=0; iii<4*4 ; iii++) {
		myMatrix[iii] = 0;
	}
	myMatrix[0] = 2.0 / (right - left);
	myMatrix[5] = 2.0 / (top - bottom);
	myMatrix[10] = -2.0 / (farVal - nearVal);
#if 1
	myMatrix[3]  = -1*(right + left) / (right - left);
	myMatrix[7]  = -1*(top + bottom) / (top - bottom);
	myMatrix[11] = -1*(farVal + nearVal) / (farVal - nearVal);
#else
	// test if matrix is not corectly instanciate ...
	myMatrix[12]  = -1*(right + left) / (right - left);
	myMatrix[13]  = -1*(top + bottom) / (top - bottom);
	myMatrix[14] = -1*(farVal + nearVal) / (farVal - nearVal);
#endif
	myMatrix[15] = 1;

	glLoadMatrixf(myMatrix);


}
