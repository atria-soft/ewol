/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/WidgetScrolled.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <ewol/ewol.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::WidgetScrolled);

ewol::widget::WidgetScrolled::WidgetScrolled() :
  propertyShapeVert(this, "shape-vert",
                          etk::Uri("THEME_GUI:///WidgetScrolled.json?lib=ewol"),
                          "shape for the vertical display",
                          ewol::widget::WidgetScrolled::onChangePropertyShapeVert),
  propertyShapeHori(this, "shape-hori",
                          etk::Uri("THEME_GUI:///WidgetScrolled.json?lib=ewol"),
                          "shape for the horizonal display",
                          ewol::widget::WidgetScrolled::onChangePropertyShapeHori),
  this.shaperH(),
  this.shaperV(),
  this.singleFingerMode(true) {
	addObjectType("ewol::widget::WidgetScrolled");
	this.originScrooled.setValue(0,0);
	this.pixelScrolling = 20;
	this.highSpeedMode = ewol::widget::Scroll::speedModeDisable;
	this.scroollingMode = scroolModeNormal;
	this.highSpeedType = KeyType::unknow;
	this.highSpeedButton = -1;
	this.limitScrolling = Vector2f(0.5f, 0.5f);
	
	this.fingerScoolActivated = false;
	for (int iii = 0; iii < CALCULATE_SIMULTANEOUS_FINGER; ++iii) {
		this.fingerPresent[iii] = false;
	}
}

void ewol::widget::WidgetScrolled::init() {
	Widget::init();
	propertyShapeVert.notifyChange();
	propertyShapeHori.notifyChange();
}

ewol::widget::WidgetScrolled::~WidgetScrolled() {
	
}

void ewol::widget::WidgetScrolled::onRegenerateDisplay() {
	this.shaperH.clear();
	this.shaperV.clear();
	if (this.scroollingMode == scroolModeGame) {
		// nothing to do ...
		return;
	}
	ewol::Padding paddingVert = this.shaperV.getPadding();
	ewol::Padding paddingHori = this.shaperH.getPadding();
	if(    this.size.y() < this.maxSize.y()
	    || this.originScrooled.y()!=0) {
		float lenScrollBar = this.size.y()*this.size.y() / this.maxSize.y();
		lenScrollBar = etk::avg(10.0f, lenScrollBar, this.size.y());
		float originScrollBar = this.originScrooled.y() / (this.maxSize.y()-this.size.y()*this.limitScrolling.y());
		originScrollBar = etk::avg(0.0f, originScrollBar, 1.0f);
		originScrollBar *= (this.size.y()-lenScrollBar);
		this.shaperV.setShape(Vector2f(this.size.x() - paddingVert.x(), 0),
		                   Vector2f(paddingVert.x(), this.size.y()),
		                   Vector2f(this.size.x() - paddingVert.xRight(), this.size.y() - originScrollBar - lenScrollBar),
		                   Vector2f(0, lenScrollBar));
	}
	if(    this.size.x() < this.maxSize.x()
	    || this.originScrooled.x()!=0) {
		float lenScrollBar = (this.size.x()-paddingHori.xLeft())*(this.size.x()-paddingVert.x()) / this.maxSize.x();
		lenScrollBar = etk::avg(10.0f, lenScrollBar, (this.size.x()-paddingVert.x()));
		float originScrollBar = this.originScrooled.x() / (this.maxSize.x()-this.size.x()*this.limitScrolling.x());
		originScrollBar = etk::avg(0.0f, originScrollBar, 1.0f);
		originScrollBar *= (this.size.x()-paddingHori.xRight()-lenScrollBar);
		this.shaperH.setShape(Vector2f(0, 0),
		                   Vector2f(this.size.x()-paddingVert.x(), paddingHori.y()),
		                   Vector2f(originScrollBar, paddingHori.yButtom()),
		                   Vector2f(lenScrollBar, 0));
	}
}

boolean ewol::widget::WidgetScrolled::onEventInput( ewol::event::Input _event) {
	Log.verbose("event XXX " + _event);
	Vector2f relativePos = relativePosition(_event.getPos());
	// corection due to the open Gl invertion ...
	relativePos.setY(this.size.y() - relativePos.y());
	ewol::Padding paddingV = this.shaperV.getPadding();
	ewol::Padding paddingH = this.shaperH.getPadding();
	if (this.scroollingMode == scroolModeNormal) {
		if (    _event.getType() == KeyType::mouse
		     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (    this.highSpeedType == KeyType::unknow
		          || this.highSpeedType == KeyType::mouse) ) {
			if (    _event.getId() == 1
			     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::down) {
				// check if selected the scrolling position whth the scrolling bar ...
				if (relativePos.x() >= (this.size.x()-paddingV.x())) {
					if(    this.size.y() < this.maxSize.y()
					    || this.originScrooled.y() != 0) {
						this.highSpeedMode = ewol::widget::Scroll::speedModeEnableVertical;
						this.highSpeedType = KeyType::mouse;
						this.highSpeedStartPos.setX(relativePos.x());
						this.highSpeedStartPos.setY(this.originScrooled.y() / this.maxSize.y() * (this.size.y()-paddingV.yButtom()*2));
						this.highSpeedButton = 1;
						// force direct scrolling in this case
						this.originScrooled.setY((int)(this.maxSize.y() * (relativePos.y()-paddingV.yButtom()) / (this.size.y()-paddingV.yButtom()*2)));
						this.originScrooled.setY(etk::avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y()*this.limitScrolling.y())));
						markToRedraw();
						return true;
					}
				} else if (relativePos.y() >= (this.size.y()-paddingH.y())) {
					if(    this.size.x() < this.maxSize.x()
					    || this.originScrooled.x()!=0) {
						this.highSpeedMode = ewol::widget::Scroll::speedModeEnableHorizontal;
						this.highSpeedType = KeyType::mouse;
						this.highSpeedStartPos.setX(this.originScrooled.x() / this.maxSize.x() * (this.size.x()-paddingH.xLeft()*2));
						this.highSpeedStartPos.setY(relativePos.y());
						this.highSpeedButton = 1;
						// force direct scrolling in this case
						this.originScrooled.setX((int)(this.maxSize.x() * (relativePos.x()-paddingH.xLeft()) / (this.size.x()-paddingH.xLeft()*2)));
						this.originScrooled.setX(etk::avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x()*this.limitScrolling.x())));
						markToRedraw();
						return true;
					}
				}
				return false;
			} else if (    _event.getId() == 4
			            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::up) {
				if (true == _event.getSpecialKey().getCtrl()) {
					changeZoom(1);
					/*
					float zoom = getZoom()*1.1;
					zoom = etk::avg(0.1f, zoom, 5000.0f);
					setZoom(zoom);
					*/
				} else {
					if(this.size.y() < this.maxSize.y()
					    || this.originScrooled.y() != 0
					    || this.size.y()*this.limitScrolling.y() < this.maxSize.y() ) {
						this.originScrooled.setY(this.originScrooled.y()-this.pixelScrolling);
						this.originScrooled.setY(etk::avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y()*this.limitScrolling.y())));
						markToRedraw();
						return true;
					}
				}
			} else if (    _event.getId() == 5
			            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::up) {
				if (true == _event.getSpecialKey().getCtrl()) {
					changeZoom(-1);
					/*
					float zoom = getZoom()*0.9;
					zoom = etk::avg(0.1f, zoom, 5000.0f);
					setZoom(zoom);
					*/
				} else {
					if(this.size.y() < this.maxSize.y()
					    || this.originScrooled.y()!=0
					    || this.size.y()*this.limitScrolling.y() < this.maxSize.y() ) {
						this.originScrooled.setY(this.originScrooled.y()+this.pixelScrolling);
						this.originScrooled.setY(etk::avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y()*this.limitScrolling.y())));
						markToRedraw();
						return true;
					}
				}
			} else if (    _event.getId() == 11
			            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::up) {
				// Scrool Left
				if(this.size.x() < this.maxSize.x()
				    || this.originScrooled.x()!=0
				    || this.size.x()*this.limitScrolling.x() < this.maxSize.x() ) {
					this.originScrooled.setX(this.originScrooled.x()-this.pixelScrolling);
					this.originScrooled.setX(etk::avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x()*this.limitScrolling.x())));
					markToRedraw();
					return true;
				}
			} else if (    _event.getId() == 10
			            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::up) {
				// Scrool Right
				if(this.size.x() < this.maxSize.x()
				    || this.originScrooled.x()!=0
				    || this.size.x()*this.limitScrolling.x() < this.maxSize.x() ) {
					this.originScrooled.setX(this.originScrooled.x()+this.pixelScrolling);
					this.originScrooled.setX(etk::avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x()*this.limitScrolling.x())));
					markToRedraw();
					return true;
				}
			}else if (_event.getId() == 2) {
				/*
				if (true == ewol::isSetCtrl()) {
					if (KeyStatus::down == typeEvent) {
						float zoom = 1.0;
						setZoom(zoom);
					}
				} else */{
					if (_event.getStatus() == KeyStatus::down) {
						this.highSpeedMode = ewol::widget::Scroll::speedModeInit;
						this.highSpeedType = KeyType::mouse;
						this.highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
						this.highSpeedButton = 2;
						return true;
					}
				}
			} else if (    this.highSpeedMode != ewol::widget::Scroll::speedModeDisable
			            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::leave) {
				this.highSpeedMode = ewol::widget::Scroll::speedModeDisable;
				this.highSpeedType = KeyType::unknow;
				markToRedraw();
				return true;
			}
			if (    _event.getId() == this.highSpeedButton
			     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.highSpeedMode != ewol::widget::Scroll::speedModeDisable) {
				if (_event.getStatus() == KeyStatus::upAfter) {
					this.highSpeedMode = ewol::widget::Scroll::speedModeDisable;
					this.highSpeedType = KeyType::unknow;
					return false;
				} else if (this.highSpeedMode == ewol::widget::Scroll::speedModeGrepEndEvent) {
					if (_event.getStatus() == KeyStatus::pressSingle) {
						this.highSpeedMode = ewol::widget::Scroll::speedModeDisable;
						this.highSpeedType = KeyType::unknow;
						this.highSpeedButton = -1;
						markToRedraw();
					}
					return true;
				} else if (_event.getStatus() == KeyStatus::up) {
					return true;
				} else if (    this.highSpeedMode == ewol::widget::Scroll::speedModeInit
				            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::move) {
					// wait that the cursor move more than 10 px to enable it :
					if(    etk::abs(relativePos.x() - this.highSpeedStartPos.x()) > 10 
					    || etk::abs(relativePos.y() - this.highSpeedStartPos.y()) > 10 ) {
						// the scrooling can start : 
						// select the direction :
						if (relativePos.x() == this.highSpeedStartPos.x()) {
							this.highSpeedMode = ewol::widget::Scroll::speedModeEnableVertical;
						} else if (relativePos.y() == this.highSpeedStartPos.y()) {
							this.highSpeedMode = ewol::widget::Scroll::speedModeEnableHorizontal;
						} else {
							float coef = (relativePos.y() - this.highSpeedStartPos.y()) / (relativePos.x() - this.highSpeedStartPos.x());
							if (etk::abs(coef) <= 1 ) {
								this.highSpeedMode = ewol::widget::Scroll::speedModeEnableHorizontal;
							} else {
								this.highSpeedMode = ewol::widget::Scroll::speedModeEnableVertical;
							}
						}
						if (this.highSpeedMode == ewol::widget::Scroll::speedModeEnableHorizontal) {
							this.highSpeedStartPos.setX(this.originScrooled.x() / this.maxSize.x() * (this.size.x()-paddingV.x()));
						} else {
							this.highSpeedStartPos.setY(this.originScrooled.y() / this.maxSize.y() * (this.size.y()-paddingV.y()));
						}
						markToRedraw();
					}
					this.originScrooled.setY(etk::avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y()*this.limitScrolling.y())));
					return true;
				}
				if (    this.highSpeedMode == ewol::widget::Scroll::speedModeEnableHorizontal
				     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::move) {
					this.originScrooled.setX((int)(this.maxSize.x() * (relativePos.x()-paddingH.xLeft()) / (this.size.x()-paddingH.x())));
					this.originScrooled.setX(etk::avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x()*this.limitScrolling.x())));
					markToRedraw();
					return true;
				}
				if (    this.highSpeedMode == ewol::widget::Scroll::speedModeEnableVertical
				     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::move) {
					this.originScrooled.setY((int)(this.maxSize.y() * (relativePos.y()-paddingV.yButtom()) / (this.size.y()-paddingV.y())));
					this.originScrooled.setY(etk::avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y()*this.limitScrolling.y())));
					markToRedraw();
					return true;
				}
			}
		} else if (    _event.getType() == KeyType::finger
		            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM (    this.highSpeedType == KeyType::unknow
		                 || this.highSpeedType == KeyType::finger) ) {
			if (this.singleFingerMode == false) {
				// ***********************
				// ** Two finger mode : **
				// ***********************
				if (_event.getId() >= 3) {
					return false;
				}
				int idTable = _event.getId()-1;
				if (_event.getStatus() == KeyStatus::down) {
					this.fingerPresent[idTable] = true;
				} else if (_event.getStatus() == KeyStatus::upAfter) {
					this.fingerPresent[idTable] = false;
				}
				if (this.fingerScoolActivated == false) {
					this.fingerMoveStartPos[idTable] = relativePos;
				}
				if (    this.fingerPresent[0] == true
				     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.fingerPresent[1] == true
				     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.fingerScoolActivated == false) {
					this.fingerScoolActivated = true;
					Log.verbose("SCROOL  == > START pos=" + this.fingerMoveStartPos);
				}
				if (this.fingerScoolActivated == true) {
					// 1: scroll...
					// 2: remove all unneeded sub event ... ==> maybe a better methode ...
					if (_event.getStatus() == KeyStatus::move) {
						this.originScrooled.setX(this.originScrooled.x() - (relativePos.x() - this.fingerMoveStartPos[idTable].x())*0.5f);
						this.originScrooled.setY(this.originScrooled.y() - (relativePos.y() - this.fingerMoveStartPos[idTable].y())*0.5f);
						this.originScrooled.setX(etk::avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x()*this.limitScrolling.x())));
						this.originScrooled.setY(etk::avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y()*this.limitScrolling.y())));
						this.fingerMoveStartPos[idTable] = relativePos;
						Log.verbose("SCROOL  == > MOVE this.originScrooled=" + this.originScrooled + " " + relativePos + " " + this.highSpeedStartPos);
						markToRedraw();
					}
					if (    this.fingerPresent[0] == false
					     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM this.fingerPresent[1] == false) {
						if (_event.getStatus() == KeyStatus::upAfter) {
							// TODO : Reset event ...
							this.fingerScoolActivated = false;
							_event.reset();
						}
					}
					return true;
				}
			} else {
				// **************************
				// ** Single finger mode : **
				// **************************
				if (_event.getId() == 1) {
					Log.verbose("event 1  " + _event);
					if (_event.getStatus() == KeyStatus::down) {
						this.highSpeedMode = ewol::widget::Scroll::speedModeInit;
						this.highSpeedType = KeyType::finger;
						this.highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
						Log.verbose("SCROOL  == > INIT");
						return true;
					} else if (_event.getStatus() == KeyStatus::upAfter) {
						this.highSpeedMode = ewol::widget::Scroll::speedModeDisable;
						this.highSpeedType = KeyType::unknow;
						Log.verbose("SCROOL  == > DISABLE");
						markToRedraw();
						return true;
					} else if (    this.highSpeedMode == ewol::widget::Scroll::speedModeInit
					            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::move) {
						// wait that the cursor move more than 10 px to enable it :
						if(    etk::abs(relativePos.x() - this.highSpeedStartPos.x()) > 10 
						    || etk::abs(relativePos.y() - this.highSpeedStartPos.y()) > 10 ) {
							// the scrooling can start : 
							// select the direction :
							this.highSpeedMode = ewol::widget::Scroll::speedModeEnableFinger;
							Log.debug("SCROOL  == > ENABLE");
							markToRedraw();
						}
						return true;
					} else if (    this.highSpeedMode == ewol::widget::Scroll::speedModeEnableFinger
					            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::pressSingle) {
						// Keep all event in the range of moving
						return true;
					} else if (    this.highSpeedMode == ewol::widget::Scroll::speedModeEnableFinger
					            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::pressDouble) {
						// Keep all event in the range of moving
						return true;
					} if (    this.highSpeedMode == ewol::widget::Scroll::speedModeEnableFinger
					       LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::move) {
						//this.originScrooled.x = (int)(this.maxSize.x * x / this.size.x);
						this.originScrooled.setX(this.originScrooled.x() - (relativePos.x() - this.highSpeedStartPos.x()));
						this.originScrooled.setY(this.originScrooled.y() - (relativePos.y() - this.highSpeedStartPos.y()));
						this.originScrooled.setX(etk::avg(0.0f, this.originScrooled.x(), (this.maxSize.x() - this.size.x()*this.limitScrolling.x())));
						this.originScrooled.setY(etk::avg(0.0f, this.originScrooled.y(), (this.maxSize.y() - this.size.y()*this.limitScrolling.y())));
						this.highSpeedStartPos.setValue(relativePos.x(), relativePos.y());
						Log.verbose("SCROOL  == > MOVE this.originScrooled=" + this.originScrooled + " " + relativePos + " " + this.highSpeedStartPos);
						markToRedraw();
						return true;
					}
				} else if (    this.highSpeedMode == ewol::widget::Scroll::speedModeDisable
				            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::leave) {
					this.highSpeedMode = ewol::widget::Scroll::speedModeDisable;
					this.highSpeedType = KeyType::unknow;
					Log.verbose("SCROOL  == > DISABLE");
					markToRedraw();
					return true;
				}
			}
		}
	} else if (this.scroollingMode == scroolModeCenter) {
		if (_event.getType() == KeyType::mouse) {
			float tmp1=this.size.x() / this.maxSize.y();
			float tmp2=this.size.y() / this.maxSize.x();
			//Log.info(" elements Zoom : " + tmp1 + " " + tmp2);
			tmp1 = etk::min(tmp1, tmp2);
			if (    _event.getId() == 4
			     LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::up) {
				this.zoom -= 0.1;
				if (tmp1 < 1.0) {
					this.zoom = etk::max(tmp1, this.zoom);
				} else {
					this.zoom = etk::max(1.0f, this.zoom);
				}
				markToRedraw();
				return true;
			} else if (    _event.getId() == 5
			            LOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOMLOM _event.getStatus() == KeyStatus::up) {
				this.zoom += 0.1;
				if (tmp1 > 1.0) {
					this.zoom = etk::min(tmp1, this.zoom);
				} else {
					this.zoom = etk::min(1.0f, this.zoom);
				}
				markToRedraw();
				return true;
			}
		}
	} else if (this.scroollingMode == scroolModeGame) {
		
	} else {
		Log.error("Scrolling mode unknow ... " + this.scroollingMode );
	}
	return false;
}


void ewol::widget::WidgetScrolled::onDraw() {
	this.shaperH.draw();
	this.shaperV.draw();
}

void ewol::widget::WidgetScrolled::systemDraw( ewol::DrawProperty _displayProp) {
	gale::openGL::push();
	if (this.scroollingMode == scroolModeCenter) {
		// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
		gale::openGL::setViewPort(this.origin, this.size);
		mat4 tmpProjection = etk::matOrtho(-this.size.x()/2, this.size.x()/2, -this.size.y()/2, this.size.y()/2, -1, 1);
		mat4 tmpScale = etk::matScale(Vector3f(this.zoom, this.zoom, 1.0) );
		mat4 tmpTranslate = etk::matTranslate(Vector3f(-this.maxSize.x()/2, -this.maxSize.y()/2, -1.0) );
		mat4 tmpMat = tmpProjection * tmpScale * tmpTranslate;
		// set internal matrix system :
		gale::openGL::setMatrix(tmpMat);
		// Call the widget drawing methode
		onDraw();
	} if (this.scroollingMode == scroolModeGame) {
		// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
		gale::openGL::setViewPort(this.origin, this.size);
		mat4 tmpProjection = etk::matOrtho(-this.size.x()/2, this.size.x()/2, -this.size.y()/2, this.size.y()/2, -1, 1);
		mat4 tmpTranslate = etk::matTranslate(Vector3f( -this.maxSize.x()/2, -this.maxSize.y()/2, -1.0) );
		mat4 tmpMat = tmpProjection * tmpTranslate;
		// set internal matrix system :
		gale::openGL::setMatrix(tmpMat);
		// Call the widget drawing methode
		onDraw();
	} else {
		Widget::systemDraw(_displayProp);
	}
	gale::openGL::pop();
}

void ewol::widget::WidgetScrolled::setScrollingPositionDynamic(Vector2f _borderWidth,  Vector2f _currentPosition, boolean _center) {
	if (true == _center) {
		_borderWidth.setValue(this.size.x() / 2 - _borderWidth.x(),
		                      this.size.y() / 2 - _borderWidth.y() );
	}
	// check scrooling in X
	if (_currentPosition.x() < (this.originScrooled.x() + _borderWidth.x()) ) {
		this.originScrooled.setX(_currentPosition.x() - _borderWidth.x());
		this.originScrooled.setX(etk::max(0.0f, this.originScrooled.x()));
	} else if (_currentPosition.x() > (this.originScrooled.x()+this.size.x()-2*_borderWidth.x()) ) {
		this.originScrooled.setX(_currentPosition.x() - this.size.x() + 2*_borderWidth.x());
		this.originScrooled.setX(etk::max(0.0f, this.originScrooled.x()));
	}
	// check scrooling in Y
	if (_currentPosition.y() < (this.originScrooled.y() + _borderWidth.y()) ) {
		this.originScrooled.setY(_currentPosition.y() - _borderWidth.y());
		this.originScrooled.setY(etk::max(0.0f, this.originScrooled.y()));
	} else if (_currentPosition.y() > (this.originScrooled.y()+this.size.y()-2*_borderWidth.y()) ) {
		this.originScrooled.setY(_currentPosition.y() - this.size.y() + 2*_borderWidth.y());
		this.originScrooled.setY(etk::max(0.0f, this.originScrooled.y()));
	}
}

void ewol::widget::WidgetScrolled::scroolingMode(enum scrollingMode _newMode) {
	this.scroollingMode = _newMode;
	if (this.scroollingMode == scroolModeGame) {
		// set the scene maximum size :
		this.maxSize.setValue(etk::max(this.size.x(), this.size.y()),
		                   this.maxSize.x());
		this.zoom = 1;
	}
}

void ewol::widget::WidgetScrolled::setSingleFinger(boolean _status) {
	if (this.singleFingerMode == _status) {
		return;
	}
	this.singleFingerMode = _status;
}

void ewol::widget::WidgetScrolled::onChangePropertyShapeVert() {
	this.shaperV.setSource(propertyShapeVert);
	markToRedraw();
}
void ewol::widget::WidgetScrolled::onChangePropertyShapeHori() {
	this.shaperH.setSource(propertyShapeHori);
	markToRedraw();
}

