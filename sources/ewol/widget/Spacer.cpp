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

static ewol::Widget* Create(void)
{
	return new widget::Spacer();
}

void widget::Spacer::Init(ewol::WidgetManager& _widgetManager)
{
	_widgetManager.AddWidgetCreator(__class__,&Create);
}


widget::Spacer::Spacer(void)
{
	m_userMinSize = ewol::Dimension(vec2(10,10));
	SetCanHaveFocus(false);
	m_color = etk::color::black;
	m_color.SetA(0);
	RegisterConfig(configColor, "color", NULL, "background of the spacer");
}

widget::Spacer::~Spacer(void)
{
	
}

void widget::Spacer::OnDraw(void)
{
	m_draw.Draw();
}


#define BORDER_SIZE_TMP  (4)
void widget::Spacer::OnRegenerateDisplay(void)
{
	if (false == NeedRedraw()) {
		return;
	}
	m_draw.Clear();
	
	if (m_color.a() == 0) {
		return;
	}
	m_draw.SetColor(m_color);
	m_draw.SetPos(vec3(0, 0, 0) );
	m_draw.RectangleWidth(vec3(m_size.x(), m_size.y(),0) );
}

bool widget::Spacer::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == ewol::Widget::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == configColor) {
		m_color = _conf.GetData();
		MarkToRedraw();
		return true;
	}
	return false;
}

bool widget::Spacer::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == ewol::Widget::OnGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configColor) {
		_result = m_color.GetString();
		return true;
	}
	return false;
}


