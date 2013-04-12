/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Layer.h>
#include <ewol/widget/WidgetManager.h>


#undef __class__
#define __class__	"Layer"

static ewol::Widget* Create(void)
{
	return new widget::Layer();
}

void widget::Layer::Init(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,&Create);
}

void widget::Layer::UnInit(void)
{
	ewol::widgetManager::AddWidgetCreator(__class__,NULL);
}


widget::Layer::Layer(void)
{
	// nothing to do ...
}

widget::Layer::~Layer(void)
{
	EWOL_DEBUG("[" << GetId() << "] Layer : destroy");
}

