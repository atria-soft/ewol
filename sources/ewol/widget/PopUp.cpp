/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/PopUp.h>
#include <ewol/widget/WidgetManager.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>



#undef __class__
#define __class__	"PopUp"

const char* const widget::PopUp::configShaper="shaper";
const char* const widget::PopUp::configRemoveOnExternClick="out-click-remove";

static ewol::Widget* Create(void)
{
	return new widget::PopUp();
}

void widget::PopUp::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::PopUp::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}


widget::PopUp::PopUp(const etk::UString& _shaperName) :
	m_shaper(_shaperName),
	m_closeOutEvent(false)
{
	m_userExpand.setValue(false, false);
	SetMinSize(ewol::Dimension(vec2(80,80),ewol::Dimension::Pourcent));
	RegisterConfig(configShaper, "string", NULL, "The shaper properties");
	RegisterConfig(configRemoveOnExternClick, "bool", NULL, "Remove the widget if the use click outside");
}

widget::PopUp::~PopUp(void)
{
	
}

void widget::PopUp::SetShaperName(const etk::UString& _shaperName)
{
	m_shaper.SetSource(_shaperName);
	MarkToRedraw();
}


void widget::PopUp::CalculateSize(const vec2& _availlable)
{
	ewol::Widget::CalculateSize(_availlable);
	m_size = _availlable;
	if (NULL != m_subWidget) {
		vec2 subWidgetSize = m_subWidget->GetCalculateMinSize();
		if (true == m_subWidget->CanExpand().x()) {
			subWidgetSize.setX(m_minSize.x());
		}
		if (true == m_subWidget->CanExpand().y()) {
			subWidgetSize.setY(m_minSize.y());
		}
		// limit the size of the element :
		subWidgetSize.setMin(m_minSize);
		// posiition at a int32_t pos :
		subWidgetSize = vec2ClipInt32(subWidgetSize);
		
		// set config to the Sub-widget
		vec2 subWidgetOrigin = m_origin + (m_size-subWidgetSize)/2.0f;
		subWidgetOrigin = vec2ClipInt32(subWidgetOrigin);
		
		m_subWidget->SetOrigin(subWidgetOrigin);
		m_subWidget->CalculateSize(subWidgetSize);
	}
	MarkToRedraw();
}

void widget::PopUp::OnDraw(void)
{
	m_shaper.Draw();
}

void widget::PopUp::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		m_shaper.Clear();
		vec2 padding = m_shaper.GetPadding();
		vec2 tmpSize(0,0);
		if (NULL != m_subWidget) {
			vec2 tmpSize = m_subWidget->GetSize();
		}
		tmpSize.setMax(m_minSize);
		vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
		
		m_shaper.SetOrigin(vec2(0,0));
		m_shaper.SetSize(vec2ClipInt32(m_size));
		m_shaper.SetInsidePos(vec2ClipInt32(tmpOrigin-padding));
		m_shaper.SetInsideSize(vec2ClipInt32(tmpSize + padding*2.0f));
	}
	// SUBwIDGET GENERATION ...
	if (NULL != m_subWidget) {
		m_subWidget->OnRegenerateDisplay();
	}
}

ewol::Widget* widget::PopUp::GetWidgetAtPos(const vec2& pos)
{
	ewol::Widget* val = widget::Container::GetWidgetAtPos(pos);
	if (NULL != val) {
		return val;
	}
	return this;
}


bool widget::PopUp::OnSetConfig(const ewol::EConfig& _conf)
{
	if (true == widget::Container::OnSetConfig(_conf)) {
		return true;
	}
	if (_conf.GetConfig() == configShaper) {
		SetShaperName(_conf.GetData());
		return true;
	}
	if (_conf.GetConfig() == configRemoveOnExternClick) {
		SetRemoveOnExternClick(_conf.GetData().ToBool());
		return true;
	}
	return false;
}

bool widget::PopUp::OnGetConfig(const char* _config, etk::UString& _result) const
{
	if (true == widget::Container::OnGetConfig(_config, _result)) {
		return true;
	}
	if (_config == configShaper) {
		_result = m_shaper.GetSource();
		return true;
	}
	if (_config == configRemoveOnExternClick) {
		if (GetRemoveOnExternClick()==true) {
			_result = "true";
		} else {
			_result = "false";
		}
		return true;
	}
	return false;
}


bool widget::PopUp::OnEventInput(const ewol::EventInput& _event)
{
	if (0 != _event.GetId()) {
		if (true==m_closeOutEvent) {
			vec2 padding = m_shaper.GetPadding();
			vec2 tmpSize(0,0);
			if (NULL != m_subWidget) {
				vec2 tmpSize = m_subWidget->GetSize();
			}
			tmpSize.setMax(m_minSize);
			vec2 tmpOrigin = (m_size-tmpSize)/2.0f;
			
			tmpOrigin-=padding;
			tmpSize += padding*2.0f;
			vec2 pos=RelativePosition(_event.GetPos());
			if(    pos.x() < tmpOrigin.x()
			    || pos.y() < tmpOrigin.y()
			    || pos.x() > tmpOrigin.x()+tmpSize.x()
			    || pos.y() > tmpOrigin.y()+tmpSize.y() ) {
				AutoDestroy();
				return true;
			}
		}
	}
	return false;
}

