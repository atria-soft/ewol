/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ewol/widget/Spacer.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Manager.h>

ewol::widget::Spacer::Spacer() :
  propertyColor(this, "color",
                      etk::color::none,
                      "background of the spacer",
                      &ewol::widget::Spacer::onChangePropertyColor) {
	addObjectType("ewol::widget::Spacer");
	propertyMinSize.setDirectCheck(gale::Dimension(vec2(10,10)));
	propertyCanFocus.setDirectCheck(true);
}

ewol::widget::Spacer::~Spacer() {
	
}

void ewol::widget::Spacer::onDraw() {
	m_draw.draw();
}

#define BORDER_SIZE_TMP  (4)
void ewol::widget::Spacer::onRegenerateDisplay() {
	if (false == needRedraw()) {
		return;
	}
	m_draw.clear();
	
	if (propertyColor->a() == 0) {
		return;
	}
	m_draw.setColor(propertyColor);
	m_draw.setPos(vec3(0, 0, 0) );
	m_draw.rectangleWidth(vec3(m_size.x(), m_size.y(),0) );
}

void ewol::widget::Spacer::onChangePropertyColor() {
	markToRedraw();
}

