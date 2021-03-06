/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <appl/debug.hpp>
#include <appl/widget/SizerColor.hpp>

appl::widget::SizerColor::SizerColor() :
  propertyBorderColor(this, "border-color", etk::color::none, "Color of the border"),
  propertyLeftColor(this, "color-left", etk::color::purple),
  propertyRightColor(this, "color-right", etk::color::orange),
  propertyTopColor(this, "color-top", etk::color::cyan),
  propertyButtomColor(this, "color-buttom", etk::color::brown) {
	addObjectType("appl::widget::SizerColor");
}

void appl::widget::SizerColor::init(enum displayMode _mode) {
	ewol::widget::Sizer::init();
}

appl::widget::SizerColor::~SizerColor() {
	
}

void appl::widget::SizerColor::onRegenerateDisplay() {
	ewol::widget::Sizer::onRegenerateDisplay();
	m_draw.clear();
	vec2 tmpBorderSize = propertyBorderSize->getPixel();
	if (tmpBorderSize == vec2(0.0f, 0.0f)) {
		return;
	}
	if (propertyBorderColor->a() == 0) {
		return;
	}
	m_draw.setColor(*propertyBorderColor);
	m_draw.setPos(vec3(0, 0, 0) );
	m_draw.rectangleWidth(vec3(tmpBorderSize.x(), m_size.y(),0) );
	m_draw.setPos(vec3(m_size.x() - tmpBorderSize.x(), 0, 0) );
	m_draw.rectangleWidth(vec3(tmpBorderSize.x(), m_size.y(),0) );
	m_draw.setPos(vec3(tmpBorderSize.x(), 0, 0) );
	m_draw.rectangleWidth(vec3(m_size.x()-tmpBorderSize.x()*2.0f, tmpBorderSize.y(),0) );
	m_draw.setPos(vec3(tmpBorderSize.x(), m_size.y()-tmpBorderSize.y(), 0) );
	m_draw.rectangleWidth(vec3(m_size.x()-tmpBorderSize.x()*2.0f, tmpBorderSize.y(),0) );
	vec2 underSize(0,0);
	vec2 underOrigin(999999999999.0,999999999999.0);
	for (auto &it : m_subWidget) {
		if (it == null) {
			continue;
		}
		vec2 size = it->getSize();
		if (*propertyMode == ewol::widget::Sizer::modeVert) {
			underSize += vec2(0.0f, size.y());
			underSize.setX(etk::max(underSize.x(), size.x()));
		} else {
			underSize += vec2(size.x(), 0.0f);
			underSize.setY(etk::max(underSize.y(), size.y()));
		}
		underOrigin.setX(etk::min(it->getOrigin().x(), underOrigin.x()));
		underOrigin.setY(etk::min(it->getOrigin().y(), underOrigin.y()));
	}
	vec2 localWidgetSize = m_size - tmpBorderSize*2.0f;
	vec2 localWidgetOrigin = m_origin + tmpBorderSize;
	for (auto &it : m_subWidget) {
		if (it == null) {
			continue;
		}
		vec2 origin = it->getOrigin();
		vec2 size = it->getSize();
		// now we display around the widget every element needed
		if (*propertyMode == ewol::widget::Sizer::modeHori) {
			if (size.y() < localWidgetSize.y()) {
				// under
				if ((uint32_t(*propertyGravity) & uint32_t(ewol::gravity_buttom)) == 0) {
					m_draw.setColor(*propertyButtomColor);
					m_draw.setPos(vec2(origin.x(), localWidgetOrigin.y()) - m_origin);
					m_draw.rectangleWidth(vec2(it->getSize().x(), origin.y()-localWidgetOrigin.y()) );
				}
				// upper
				if ((uint32_t(*propertyGravity) & uint32_t(ewol::gravity_top)) == 0) {
					m_draw.setColor(*propertyTopColor);
					float startDraw = origin.y()+it->getSize().y() - m_origin.y();
					m_draw.setPos(vec2(origin.x()-m_origin.x(), startDraw));
					m_draw.rectangleWidth(vec2(it->getSize().x(), localWidgetSize.y()-startDraw+tmpBorderSize.y()) );
				}
			}
		} else {
			if (size.x() < localWidgetSize.x()) {
				// left
				if ((uint32_t(*propertyGravity) & uint32_t(ewol::gravity_left)) == 0) {
					m_draw.setColor(*propertyLeftColor);
					m_draw.setPos(vec2(localWidgetOrigin.x(), origin.y()) - m_origin);
					m_draw.rectangleWidth(vec2(origin.x()-localWidgetOrigin.x(), it->getSize().y()) );
				}
				// right
				if ((uint32_t(*propertyGravity) & uint32_t(ewol::gravity_right)) == 0) {
					m_draw.setColor(*propertyRightColor);
					float startDraw = origin.x()+it->getSize().x() - m_origin.x();
					m_draw.setPos(vec2(startDraw, origin.y()-m_origin.y()));
					m_draw.rectangleWidth(vec2(localWidgetSize.x()-startDraw+tmpBorderSize.x(), it->getSize().y()) );
				}
			}
		}
	}
	// now we do the rest of the sizer:
	if (*propertyMode == ewol::widget::Sizer::modeHori) {
		if (underSize.x() < localWidgetSize.x()) {
			// left
			if ((uint32_t(*propertyGravity) & uint32_t(ewol::gravity_left)) == 0) {
				m_draw.setColor(*propertyLeftColor);
				m_draw.setPos(localWidgetOrigin - m_origin);
				m_draw.rectangleWidth(vec2(underOrigin.x()-localWidgetOrigin.x(), localWidgetSize.y()) );
			}
			// right
			if ((uint32_t(*propertyGravity) & uint32_t(ewol::gravity_right)) == 0) {
				m_draw.setColor(*propertyRightColor);
				float startDraw = underOrigin.x() + underSize.x() - m_origin.x();
				m_draw.setPos(vec2(startDraw, localWidgetOrigin.y()-m_origin.y()));
				m_draw.rectangleWidth(vec2(localWidgetSize.x()-startDraw+tmpBorderSize.x(), localWidgetSize.y()) );
			}
		}
	} else {
		if (underSize.y() < localWidgetSize.y()) {
			// under
			if ((uint32_t(*propertyGravity) & uint32_t(ewol::gravity_buttom)) == 0) {
				m_draw.setColor(*propertyButtomColor);
				m_draw.setPos(localWidgetOrigin - m_origin);
				m_draw.rectangleWidth(vec2(localWidgetSize.x(), underOrigin.y()-localWidgetOrigin.y()) );
			}
			// upper
			if ((uint32_t(*propertyGravity) & uint32_t(ewol::gravity_top)) == 0) {
				m_draw.setColor(*propertyTopColor);
				float startDraw = underOrigin.y() + underSize.y() - m_origin.y();
				m_draw.setPos(vec2(localWidgetOrigin.x()-m_origin.x(), startDraw));
				m_draw.rectangleWidth(vec2(localWidgetSize.x(), localWidgetSize.y()-startDraw+tmpBorderSize.y()) );
			}
		}
	}
}
void appl::widget::SizerColor::onDraw() {
	m_draw.draw();
	ewol::widget::Sizer::onDraw();
}
