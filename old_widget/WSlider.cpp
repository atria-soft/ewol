/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/WSlider.hpp>
#include <ewol/ewol.hpp>
#include <ewol/object/Manager.hpp>

#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(enum ewol::widget::WSlider::sladingMode);
ETK_DECLARE_TYPE(ewol::widget::WSlider);

static  char* l_listsladingMode[ewol::widget::WSlider::sladingTransition_count] = {
	"transition vertical",
	"transition horisantal"
};

etk::Stream operator +(etk::Stream _os,  enum ewol::widget::WSlider::sladingMode _obj) {
	_os + l_listsladingMode[_obj];
	return _os;
}

ewol::widget::WSlider::WSlider() :
  signalStartSlide(this, "start", ""),
  signalStopSlide(this, "stop", ""),
  propertyTransitionSpeed(this, "speed",
                                1.0f, 0.0f, 200.0f,
                                "Transition speed of the slider"),
  propertyTransitionMode(this, "mode",
                               sladingTransitionHori,
                               "Transition mode of the slider",
                               ewol::widget::WSlider::onChangePropertyTransitionMode),
  propertySelectWidget(this, "select",
                             "",
                             "Select the requested widget to display",
                             ewol::widget::WSlider::onChangePropertySelectWidget),
  this.windowsSources(0),
  this.windowsDestination(0),
  this.windowsRequested(-1),
  this.slidingProgress(1.0f) {
	addObjectType("ewol::widget::WSlider");
	propertyTransitionMode.add(sladingTransitionVert, "vert");
	propertyTransitionMode.add(sladingTransitionHori, "hori");
}

ewol::widget::WSlider::~WSlider() {
	
}


void ewol::widget::WSlider::onChangeSize() {
	ewol::widget::ContainerN::onChangeSize();
	if (this.windowsDestination == this.windowsSources) {
		auto it = this.subWidget.begin();
		it+= this.windowsDestination;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			(*it).setOrigin(this.origin+this.offset);
			(*it).setSize(this.size);
			(*it).onChangeSize();
		}
	} else {
		float factor = -1.0f;
		if (this.windowsSources < this.windowsDestination) {
			factor = 1.0f;
		}
		auto it = this.subWidget.begin();
		it += this.windowsSources;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
		     if (*propertyTransitionMode == sladingTransitionHori) {
				(*it).setOrigin(   Vector2f(this.origin.x() + factor*(this.size.x()*this.slidingProgress),
				                         this.origin.y())
				                  + this.offset);
			} else {
				(*it).setOrigin(   Vector2f(this.origin.x(),
				                         this.origin.y() + factor*(this.size.y()*this.slidingProgress))
				                  + this.offset);
			}
			(*it).setSize(this.size);
			(*it).onChangeSize();
		}
		it = this.subWidget.begin();
		it += this.windowsDestination;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			if (*propertyTransitionMode == sladingTransitionHori) {
				(*it).setOrigin(   Vector2f(this.origin.x() + factor*(this.size.x()*this.slidingProgress - this.size.x()),
				                         this.origin.y())
				                  + this.offset);
			} else {
				(*it).setOrigin(   Vector2f(this.origin.x(),
				                         this.origin.y() + factor*(this.size.y()*this.slidingProgress - this.size.y()))
				                  + this.offset);
			}
			(*it).setSize(this.size);
			(*it).onChangeSize();
		}
	}
	markToRedraw();
}

void ewol::widget::WSlider::subWidgetSelectSetVectorId(int _id) {
	if (_id<0) {
		LOGGER.error("Can not change to a widget not present : vectID=" + _id);
		return;
	}
	if (_id != this.windowsDestination) {
		this.windowsRequested = _id;
		signalStartSlide.emit();
		this.PCH = getObjectManager().periodicCall.connect(this, ewol::widget::WSlider::periodicCall);
		markToRedraw();
	}
}

void ewol::widget::WSlider::subWidgetSelectSet(int _id) {
	int elementID = 0;
	// search element in the list : 
	for (auto it : this.subWidget) {
		elementID ++;
		if (it != null) {
			if (it.getId() == _id) {
				if (it.propertyName.get() != "") {
					// change the internal event parameter (in case...) ==> no event generation
					propertySelectWidget.setDirect(it.propertyName.get());
				} else {
					propertySelectWidget.setDirect("");
				}
				break;
			}
		}
	}
	if (elementID < this.subWidget.size()) {
		subWidgetSelectSetVectorId(elementID);
	} else {
		subWidgetSelectSetVectorId(-1);
		// change the internal event parameter (in case...) ==> no event generation
		propertySelectWidget.setDirect("");
	}
}

void ewol::widget::WSlider::subWidgetSelectSet( Widget _widgetPointer) {
	if (_widgetPointer == null) {
		LOGGER.error("Can not change to a widget null");
		return;
	}
	int iii = 0;
	for (auto it : this.subWidget) {
		if (    it != null
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM it == _widgetPointer) {
			subWidgetSelectSetVectorId(iii);
			if (_widgetPointer.propertyName.get() != "") {
				// change the internal event parameter (in case...) ==> no event generation
				propertySelectWidget.setDirect(_widgetPointer.propertyName.get());
			} else {
				propertySelectWidget.setDirect("");
			}
			return;
		}
		iii++;
	}
	LOGGER.error("Can not change to a widget not present");
}

void ewol::widget::WSlider::subWidgetSelectSet( String _widgetName) {
	if (_widgetName == "") {
		LOGGER.error("Can not change to a widget with no name (input)");
		return;
	}
	LOGGER.trace("Select a new sub-widget to dosplay : '" + _widgetName + "'");
	int iii = 0;
	for (auto it : this.subWidget) {
		if (    it != null
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM it.propertyName.get() == _widgetName) {
			subWidgetSelectSetVectorId(iii);
			// change the internal event parameter (in case...) ==> no event generation
			propertySelectWidget.setDirect(_widgetName);
			return;
		}
		iii++;
	}
	LOGGER.error("Can not change to a widget not present");
}
void ewol::widget::WSlider::periodicCall( ewol::event::Time _event) {
	LOGGER.error("Periodic: " + this.slidingProgress + "/1.0 " + this.windowsSources + " ==> " + this.windowsDestination + "  " + _event);
	if (this.slidingProgress >= 1.0) {
		this.windowsSources = this.windowsDestination;
		if(    this.windowsRequested != -1
		    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.windowsRequested != this.windowsSources) {
			this.windowsDestination = this.windowsRequested;
			this.slidingProgress = 0.0;
		} else {
			// end of periodic :
			this.PCH.disconnect();
			signalStopSlide.emit();
		}
		this.windowsRequested = -1;
	}
	
	if (this.slidingProgress < 1.0) {
		if (    this.windowsRequested != -1
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.slidingProgress<0.5 ) {
			// invert sources with destination
			int tmppp = this.windowsDestination;
			this.windowsDestination = this.windowsSources;
			this.windowsSources = tmppp;
			this.slidingProgress = 1.0f - this.slidingProgress;
			if (this.windowsRequested == this.windowsDestination) {
				this.windowsRequested = -1;
			}
		}
		this.slidingProgress += _event.getDeltaCall()/propertyTransitionSpeed;
		this.slidingProgress = etk::avg(0.0f, this.slidingProgress, 1.0f);
	}
	onChangeSize();
}

void ewol::widget::WSlider::systemDraw( ewol::DrawProperty _displayProp) {
	if (*propertyHide == true){
		// widget is hidden ...
		return;
	}
	// note : do not call the widget container  == > overload this one ...
	Widget::systemDraw(_displayProp);
	
	// subwidget draw
	ewol::DrawProperty prop = _displayProp;
	prop.limit(this.origin, this.size);
	
	if (this.windowsDestination == this.windowsSources) {
		//LOGGER.debug("Draw : " + this.windowsDestination);
		auto it = this.subWidget.begin();
		it += this.windowsDestination;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			//LOGGER.info("Draw : [" + propertyName + "] t=" + getObjectType() + "o=" + this.origin + "  s=" + this.size);
			(*it).systemDraw(prop);
		}
	} else {
		//LOGGER.debug("Draw : " + this.windowsSources + "=>" + this.windowsDestination + "progress=" + ((float)this.slidingProgress/1000.) );
		// draw Sources :
		auto it = this.subWidget.begin();
		it += this.windowsSources;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			(*it).systemDraw(prop);
		}
		// draw Destination : 
		it = this.subWidget.begin();
		it += this.windowsDestination;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			(*it).systemDraw(prop);
		}
	}
}

void ewol::widget::WSlider::onRegenerateDisplay() {
	if (this.windowsDestination == this.windowsSources) {
		auto it = this.subWidget.begin();
		it += this.windowsDestination;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			(*it).onRegenerateDisplay();
		}
	} else {
		auto it = this.subWidget.begin();
		it += this.windowsSources;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			(*it).onRegenerateDisplay();
		}
		it = this.subWidget.begin();
		it += this.windowsDestination;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			(*it).onRegenerateDisplay();
		}
	}
}

void ewol::widget::WSlider::onChangePropertySelectWidget() {
	if (propertySelectWidget.get() != "") {
		LOGGER.error("SELECT new widget: " + propertySelectWidget.get());
		subWidgetSelectSet(*propertySelectWidget);
	}
}

void ewol::widget::WSlider::onChangePropertyTransitionMode() {
	markToRedraw();
}


Widget ewol::widget::WSlider::getWidgetAtPos( Vector2f _pos) {
	if (*propertyHide == true) {
		return null;
	}
	if (this.windowsDestination == this.windowsSources) {
		auto it = this.subWidget.begin();
		it += this.windowsDestination;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			Vector2f tmpSize = (*it).getSize();
			Vector2f tmpOrigin = (*it).getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (tmpOrigin.y() <= _pos.y() LOMLOMLOMLOMLOM tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				Widget tmpWidget = (*it).getWidgetAtPos(_pos);
				if (tmpWidget != null) {
					return tmpWidget;
				}
				return null;
			}
		}
	} else {
		auto it = this.subWidget.begin();
		it += this.windowsDestination;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			Vector2f tmpSize = (*it).getSize();
			Vector2f tmpOrigin = (*it).getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (tmpOrigin.y() <= _pos.y() LOMLOMLOMLOMLOM tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				Widget tmpWidget = (*it).getWidgetAtPos(_pos);
				if (tmpWidget != null) {
					return tmpWidget;
				}
				return null;
			}
		}
		it = this.subWidget.begin();
		it += this.windowsSources;
		if (    it != this.subWidget.end()
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM *it != null) {
			Vector2f tmpSize = (*it).getSize();
			Vector2f tmpOrigin = (*it).getOrigin();
			if(    (tmpOrigin.x() <= _pos.x() LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM tmpOrigin.x() + tmpSize.x() >= _pos.x())
			    LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (tmpOrigin.y() <= _pos.y() LOMLOMLOMLOMLOM tmpOrigin.y() + tmpSize.y() >= _pos.y()) )
			{
				Widget tmpWidget = (*it).getWidgetAtPos(_pos);
				if (tmpWidget != null) {
					return tmpWidget;
				}
				return null;
			}
		}
	}
	return null;
}

