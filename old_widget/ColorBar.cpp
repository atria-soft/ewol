/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ewol/widget/ColorBar.hpp>

#include <ewol/compositing/Drawing.hpp>
#include <ewol/widget/Manager.hpp>

#include <etk/Color.hpp>
#include <etk/typeInfo.hpp>
ETK_DECLARE_TYPE(ewol::widget::ColorBar);

ewol::widget::ColorBar::ColorBar() :
  signalChange(this, "change", "Color value change"),
  propertyValue(this, "color",
                      etk::color::black,
                      "Current color",
                      ewol::widget::ColorBar::onChangePropertyValue) {
	addObjectType("ewol::widget::ColorBar");
	this.currentUserPos.setValue(0,0);
	propertyCanFocus.setDirectCheck(true);
	setMouseLimit(1);
}

ewol::widget::ColorBar::~ColorBar() {
	
}


void ewol::widget::ColorBar::calculateMinMaxSize() {
	this.minSize.setValue(160, 80);
	markToRedraw();
}

static etk::Color<> s_listColorWhite(0xFF, 0xFF, 0xFF, 0xFF);
static etk::Color<> s_listColorBlack(0x00, 0x00, 0x00, 0xFF);
#define NB_BAND_COLOR		(6)
static etk::Color<> s_listColor[NB_BAND_COLOR+1] = {
	etk::Color<>(0xFF, 0x00, 0x00, 0xFF),
	etk::Color<>(0xFF, 0xFF, 0x00, 0xFF),
	etk::Color<>(0x00, 0xFF, 0x00, 0xFF),
	etk::Color<>(0x00, 0xFF, 0xFF, 0xFF),
	etk::Color<>(0x00, 0x00, 0xFF, 0xFF),
	etk::Color<>(0xFF, 0x00, 0xFF, 0xFF),
	etk::Color<>(0xFF, 0x00, 0x00, 0xFF)};


void ewol::widget::ColorBar::onChangePropertyValue() {
	propertyValue.getDirect().setA(0xFF);
	// estimate the cursor position:
	LOGGER.info("[TODO] Later when really needed ...");
}

void ewol::widget::ColorBar::onDraw() {
	this.draw.draw();
}


void ewol::widget::ColorBar::onRegenerateDisplay() {
	if (needRedraw() == true) {
		return;
	}
	// clean the object list ...
	this.draw.clear();
	
	int tmpSizeX = this.minSize.x();
	int tmpSizeY = this.minSize.y();
	int tmpOriginX = (this.size.x() - this.minSize.x()) / 2;
	int tmpOriginY = (this.size.y() - this.minSize.y()) / 2;
	
	if (propertyFill.x() == true) {
		tmpSizeX = this.size.x();
		tmpOriginX = 0;
	}
	if (propertyFill.y() == true) {
		tmpSizeY = this.size.y();
		tmpOriginY = 0;
	}
	
	for(int iii=0; iii<NB_BAND_COLOR ; iii++) {
		/* Step 1 : 
		 *              
		 *   **         
		 *   ****       
		 *   ******     
		 *   ********   
		 */
		this.draw.setColor(s_listColorWhite);
		this.draw.setPos(Vector3f(tmpOriginX + (iii)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
		this.draw.addVertex();
		this.draw.setColor(s_listColor[iii+1]);
		this.draw.setPos(Vector3f(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
		this.draw.addVertex();
		this.draw.setColor(s_listColor[iii]);
		this.draw.setPos(Vector3f(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
		this.draw.addVertex();
		/* Step 2 : 
		 *   ********     
		 *     ******     
		 *       ****     
		 *         **     
		 *                
		 */
		this.draw.setColor(s_listColorWhite);
		this.draw.setPos(Vector3f(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
		this.draw.addVertex();
		this.draw.setColor(s_listColorWhite);
		this.draw.setPos(Vector3f(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY, 0) );
		this.draw.addVertex();
		this.draw.setColor(s_listColor[iii+1]);
		this.draw.setPos(Vector3f(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0) );
		this.draw.addVertex();
		/* Step 3 : 
		 *              
		 *   **         
		 *   ****       
		 *   ******     
		 *   ********   
		 */
		this.draw.setColor(s_listColor[iii]);
		this.draw.setPos(Vector3f(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
		this.draw.addVertex();
		this.draw.setColor(s_listColorBlack);
		this.draw.setPos(Vector3f(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
		this.draw.addVertex();
		this.draw.setColor(s_listColorBlack);
		this.draw.setPos(Vector3f(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
		this.draw.addVertex();
		/* Step 4 : 
		 *   ********     
		 *     ******     
		 *       ****     
		 *         **     
		 *                
		 */
		this.draw.setColor(s_listColor[iii]);
		this.draw.setPos(Vector3f(tmpOriginX + iii*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
		this.draw.addVertex();
		this.draw.setColor(s_listColor[iii+1]);
		this.draw.setPos(Vector3f(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY/2, 0));
		this.draw.addVertex();
		this.draw.setColor(s_listColorBlack);
		this.draw.setPos(Vector3f(tmpOriginX + (iii+1)*(tmpSizeX/NB_BAND_COLOR), tmpOriginY+tmpSizeY, 0));
		this.draw.addVertex();
	}
	if (this.currentUserPos.y() > 0.5) {
		this.draw.setColor(etk::color::white);
	} else {
		this.draw.setColor(etk::color::black);
	}
	this.draw.setPos(Vector3f(this.currentUserPos.x()*this.size.x(), this.currentUserPos.y()*this.size.y(), 0) );
	this.draw.setThickness(1);
	this.draw.circle(3.0);
}


boolean ewol::widget::ColorBar::onEventInput( ewol::event::Input _event) {
	Vector2f relativePos = relativePosition(_event.getPos());
	//LOGGER.debug("Event on BT ...");
	if (1 == _event.getId()) {
		relativePos.setValue( etk::avg(0.0f, this.size.x(),relativePos.x()),
		                      etk::avg(0.0f, this.size.y(),relativePos.y()) );
		if(    KeyStatus::pressSingle == _event.getStatus()
		    || KeyStatus::move   == _event.getStatus()) {
			// nothing to do ...
			this.currentUserPos.setValue( relativePos.x()/this.size.x(),
			                           relativePos.y()/this.size.y() );
			markToRedraw();
			// == > try to estimate color
			LOGGER.trace("event on (" + relativePos.x() + "," + relativePos.y() + ")");
			int bandID = (int)(relativePos.x()/(this.size.x()/6));
			float localPos = relativePos.x() - (this.size.x()/6) * bandID;
			float poroportionnalPos = localPos/(this.size.x()/6);
			LOGGER.trace("bandId=" + bandID + "  relative pos=" + localPos);
			etk::Color<> estimateColor = etk::color::white;
			if (s_listColor[bandID].r() == s_listColor[bandID+1].r()) {
				estimateColor.setR(s_listColor[bandID].r());
			} else if (s_listColor[bandID].r() < s_listColor[bandID+1].r()) {
				estimateColor.setR(s_listColor[bandID].r() + (s_listColor[bandID+1].r()-s_listColor[bandID].r())*poroportionnalPos);
			} else {
				estimateColor.setR(s_listColor[bandID+1].r() + (s_listColor[bandID].r()-s_listColor[bandID+1].r())*(1-poroportionnalPos));
			}
			if (s_listColor[bandID].g() == s_listColor[bandID+1].g()) {
				estimateColor.setG(s_listColor[bandID].g());
			} else if (s_listColor[bandID].g() < s_listColor[bandID+1].g()) {
				estimateColor.setG(s_listColor[bandID].g() + (s_listColor[bandID+1].g()-s_listColor[bandID].g())*poroportionnalPos);
			} else {
				estimateColor.setG(s_listColor[bandID+1].g() + (s_listColor[bandID].g()-s_listColor[bandID+1].g())*(1-poroportionnalPos));
			}
			if (s_listColor[bandID].b() == s_listColor[bandID+1].b()) {
				estimateColor.setB(s_listColor[bandID].b());
			} else if (s_listColor[bandID].b() < s_listColor[bandID+1].b()) {
				estimateColor.setB(s_listColor[bandID].b() + (s_listColor[bandID+1].b()-s_listColor[bandID].b())*poroportionnalPos);
			} else {
				estimateColor.setB(s_listColor[bandID+1].b() + (s_listColor[bandID].b()-s_listColor[bandID+1].b())*(1-poroportionnalPos));
			}
			// step 2 generate the white and black ...
			if (this.currentUserPos.y() == 0.5) {
				// nothing to do ... just get the current color ...
			} else if (this.currentUserPos.y() < 0.5) {
				float poroportionnalWhite = (0.5-this.currentUserPos.y())*2.0;
				estimateColor.setR(estimateColor.r() + (0xFF-estimateColor.r())*poroportionnalWhite);
				estimateColor.setG(estimateColor.g() + (0xFF-estimateColor.g())*poroportionnalWhite);
				estimateColor.setB(estimateColor.b() + (0xFF-estimateColor.b())*poroportionnalWhite);
			} else {
				float poroportionnalBlack = (this.currentUserPos.y()-0.5)*2.0;
				estimateColor.setR(estimateColor.r() - estimateColor.r()*poroportionnalBlack);
				estimateColor.setG(estimateColor.g() - estimateColor.g()*poroportionnalBlack);
				estimateColor.setB(estimateColor.b() - estimateColor.b()*poroportionnalBlack);
			}
			if(*propertyValue != estimateColor) {
				propertyValue.set(estimateColor);
				signalChange.emit(*propertyValue);
			}
			return true;
		}
	}
	return false;
}

