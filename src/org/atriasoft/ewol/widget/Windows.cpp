/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <etk/types.hpp>
#include <ewol/ewol.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/renderer/openGL/openGL-include.hpp>
#include <ewol/context/Context.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/widget/Windows.hpp>
#include <ewol/widget/Manager.hpp>
#include <ewol/widget/meta/StdPopUp.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::Windows);

ewol::widget::Windows::Windows() :
  propertyColorConfiguration(this,
                             "file-color",
                             etk::Uri("THEME_COLOR:///Windows.json?lib=ewol"),
                             "File color of the Windows",
                             ewol::widget::Windows::onChangePropertyColor),
  propertyTitle(this,
                "title",
                "No title",
                "Title of the windows",
                ewol::widget::Windows::onChangePropertyTitle),
  this.resourceColor(null),
  this.colorBg(-1) {
	addObjectType("ewol::widget::Windows");
	propertyCanFocus.setDirectCheck(true);
	//KeyboardShow(KEYBOARD_MODE_CODE);
}


void ewol::widget::Windows::init() {
	Widget::init();
	onChangePropertyColor();
}

ewol::widget::Windows::~Windows() {
	this.subWidget.reset();
	this.popUpWidgetList.clear();
}

void ewol::widget::Windows::onChangeSize() {
	Widget::onChangeSize();
	if (this.subWidget != null) {
		this.subWidget.calculateMinMaxSize();
		// TODO : do it better ... and manage gravity ...
		this.subWidget.setSize(this.size);
		this.subWidget.setOrigin(Vector2f(0.0f, 0.0f));
		this.subWidget.onChangeSize();
	}
	for (auto it : this.popUpWidgetList) {
		if(it != null) {
			it.calculateMinMaxSize();
			it.setSize(this.size);
			it.setOrigin(Vector2f(0.0f, 0.0f));
			it.onChangeSize();
		}
	}
}

Widget ewol::widget::Windows::getWidgetAtPos( Vector2f _pos) {
	Log.verbose("Get widget at pos : " + _pos);
	// calculate relative position
	Vector2f relativePos = relativePosition(_pos);
	// event go directly on the pop-up
	if (this.popUpWidgetList.size() != 0) {
		return this.popUpWidgetList.back().getWidgetAtPos(_pos);
	// otherwise in the normal windows
	} else if (this.subWidget != null) {
		return this.subWidget.getWidgetAtPos(_pos);
	}
	// otherwise the event go to this widget ...
	return ememory::dynamicPointerCast<Widget>(sharedFromThis());
}

void ewol::widget::Windows::sysDraw() {
	Log.verbose("Draw on " + this.size);
	// set the size of the open GL system
	gale::openGL::setViewPort(Vector2f(0,0), this.size);
	gale::openGL::disable(gale::openGL::flag_dither);
	//gale::openGL::disable(gale::openGL::flag_blend);
	gale::openGL::disable(gale::openGL::flag_stencilTest);
	gale::openGL::disable(gale::openGL::flag_alphaTest);
	gale::openGL::disable(gale::openGL::flag_fog);
	gale::openGL::disable(gale::openGL::flag_texture2D);
	gale::openGL::disable(gale::openGL::flag_depthTest);
	
	gale::openGL::enable(gale::openGL::flag_blend);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// clear the matrix system :
	mat4 newOne;
	gale::openGL::setBasicMatrix(newOne);
	
	ewol::DrawProperty displayProp;
	displayProp.this.windowsSize = this.size;
	displayProp.this.origin.setValue(0,0);
	displayProp.this.size = this.size;
	systemDraw(displayProp);
	gale::openGL::disable(gale::openGL::flag_blend);
	return;
}

void ewol::widget::Windows::onRegenerateDisplay() {
	if (this.subWidget != null) {
		this.subWidget.onRegenerateDisplay();
	}
	for (auto it : this.popUpWidgetList) {
		if (it != null) {
			it.onRegenerateDisplay();
		}
	}
}

//#define TEST_PERFO_WINDOWS

void ewol::widget::Windows::systemDraw( ewol::DrawProperty _displayProp) {
	Widget::systemDraw(_displayProp);
	#ifdef TEST_PERFO_WINDOWS
	long ___startTime0 = ewol::getTime();
	#endif
	// clear the screen with transparency ...
	etk::Color<float> colorBg(0.5, 0.5, 0.5, 0.5);
	if (this.resourceColor != null) {
		colorBg = this.resourceColor.get(this.colorBg);
	}
	gale::openGL::clearColor(colorBg);
	gale::openGL::clear(   uint(gale::openGL::clearFlag_colorBuffer)
	                     | uint(gale::openGL::clearFlag_depthBuffer));
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime0 = (float)(ewol::getTime() - ___startTime0) / 1000.0f;
	Log.error("      Windows000  : " + ___localTime0 + "ms ");
	long ___startTime1 = ewol::getTime();
	#endif
	//Log.warning(" WINDOWS draw on " + this.currentDrawId);
	// first display the windows on the display
	if (this.subWidget != null) {
		this.subWidget.systemDraw(_displayProp);
		//Log.debug("Draw Windows");
	}
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime1 = (float)(ewol::getTime() - ___startTime1) / 1000.0f;
	Log.error("      Windows111  : " + ___localTime1 + "ms ");
	long ___startTime2 = ewol::getTime();
	#endif
	// second display the pop-up
	for (auto it : this.popUpWidgetList) {
		if (it != null) {
			it.systemDraw(_displayProp);
			//Log.debug("Draw Pop-up");
		}
	}
	#ifdef TEST_PERFO_WINDOWS
	float ___localTime2 = (float)(ewol::getTime() - ___startTime2) / 1000.0f;
	Log.error("      Windows222  : " + ___localTime2 + "ms ");
	#endif
}

void ewol::widget::Windows::setSubWidget(Widget _widget) {
	if (this.subWidget != null) {
		Log.info("Remove current main windows Widget...");
		this.subWidget.removeParent();
		this.subWidget.reset();
	}
	if (_widget != null) {
		this.subWidget = _widget;
		this.subWidget.setParent(sharedFromThis());
	}
	
	// Regenerate the size calculation :
	onChangeSize();
}

void ewol::widget::Windows::popUpWidgetPush(Widget _widget) {
	if (_widget == null) {
		// nothing to do an error appear :
		Log.error("can not set widget pop-up (null pointer)");
		return;
	}
	this.popUpWidgetList.pushBack(_widget);
	_widget.setParent(sharedFromThis());
	// force the focus on the basic widget ==> this remove many time the virual keyboard area
	_widget.keepFocus();
	// Regenerate the size calculation :
	onChangeSize();
	// TODO : it is dangerous to access directly to the system ...
	getContext().resetIOEvent();
}

void ewol::widget::Windows::popUpWidgetPop() {
	if (this.popUpWidgetList.size() == 0) {
		return;
	}
	this.popUpWidgetList.popBack();
}

void ewol::widget::Windows::onChangePropertyColor() {
	this.resourceColor = ewol::resource::ColorFile::create(*propertyColorConfiguration);
	if (this.resourceColor != null) {
		this.colorBg = this.resourceColor.request("background");
	} else {
		Log.warning("Can not open the default color configuration file for the windows: " + *propertyColorConfiguration);
	}
}

void ewol::widget::Windows::onChangePropertyTitle() {
	EwolContext context = getContext();
	if (context.getWindows() == sharedFromThis()) {
		context.setTitle(*propertyTitle);
	} else {
		Log.info("Set title is delayed ...");
	}
}

void ewol::widget::Windows::requestDestroyFromChild( EwolObject _child) {
	Log.verbose("A child has been removed");
	auto it = this.popUpWidgetList.begin();
	while (it != this.popUpWidgetList.end()) {
		if (*it == _child) {
			Log.verbose("    Find it ...");
			if (*it == null) {
				this.popUpWidgetList.erase(it);
				it = this.popUpWidgetList.begin();
				continue;
			}
			(*it).removeParent();
			(*it).reset();
			this.popUpWidgetList.erase(it);
			it = this.popUpWidgetList.begin();
			markToRedraw();
			continue;
		}
		++it;
	}
	if (this.subWidget == _child) {
		Log.verbose("    Find it ... 2");
		if (this.subWidget == null) {
			return;
		}
		this.subWidget.removeParent();
		this.subWidget.reset();
		markToRedraw();
	}
}

EwolObject ewol::widget::Windows::getSubObjectNamed( String _objectName) {
	EwolObject tmpObject = Widget::getSubObjectNamed(_objectName);
	if (tmpObject != null) {
		return tmpObject;
	}
	// check direct subwidget
	if (this.subWidget != null) {
		tmpObject = this.subWidget.getSubObjectNamed(_objectName);
		if (tmpObject != null) {
			return tmpObject;
		}
	}
	// get all subwidget "pop-up"
	for (auto it : this.popUpWidgetList) {
		if (it != null) {
			tmpObject = it.getSubObjectNamed(_objectName);
			if (tmpObject != null) {
				return tmpObject;
			}
		}
	}
	// not find ...
	return null;
}

void ewol::widget::Windows::drawWidgetTree(int _level) {
	Widget::drawWidgetTree(_level);
	_level++;
	if (this.subWidget != null) {
		this.subWidget.drawWidgetTree(_level);
	}
	for (auto it: this.popUpWidgetList) {
		if (it != null) {
			it.drawWidgetTree(_level);
		}
	}
}

