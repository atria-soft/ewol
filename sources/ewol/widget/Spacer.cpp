/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Spacer.h>

#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__ "Spacer"

const char* const widget::Spacer::configColor = "color";

static ewol::Widget* create(void)
{
	return new widget::Spacer();
}

void widget::Spacer::init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.addWidgetCreator(__class__,&create);
}


widget::Spacer::Spacer(void)
{
	m_userMinSize = ewol::Dimension(vec2(10,10));
	setCanHaveFocus(false);
	m_color = etk::color::black;
	m_color.setA(0);
	registerConfig(configColor, "color", NULL, "background of the spacer");
}

widget::Spacer::~Spacer(void)
{
	
}

void widget::Spacer::onDraw(void)
{
	m_draw.draw();
}


#define BORDER_SIZE_TMP  (4)
void widget::Spacer::onRegenerateDisplay(void)
{
	if (false == needRedraw()) {
		return;
	}
	m_draw.clear();
	
	if (m_color.a() == 0) {
		return;
	}
	m_draw.setColor(m_color);
	m_draw.setPos(vec3(0, 0, 0) );
	m_draw.rectangleWidth(vec3(m_size.x(), m_size.y(),0) );
}

bool widget::Spacer::onSetConfig(const ewol::EConfig& _conf)
{
	if (true == ewol::Widget::onSetConfig(_conf)) {
		return true;
	}
	if (_conf.getConfig() == configColor) {
		m_color = _conf.getData();
		markToRedraw();
		return true;
	}
	return false;
}

bool widget::Spacer::onGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == ewol::Widget::onGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configColor) {
		_result = m_color.getString();
		return true;
	}
	return false;
}


