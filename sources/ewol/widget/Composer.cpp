/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ewol/ewol.h>
#include <ewol/widget/XmlComposer.h>

/*
			ewol::Widget* m_widget;
			etk::UString composerString;
			etk::Vector<ComposerWidgetListNamed> m_list;
*/
widget::XmlComposer::XmlComposer(const etk::UString& composerXmlString)
{
	// TODO ...
	
}

widget::XmlComposer::~XmlComposer(void)
{
	if (NULL != m_widget) {
		delete(m_widget);
		// might have been destroy first here : 
		if (m_widget!=NULL) {
			EWOL_ERROR("Composer : An error Occured when removing main node");
		}
	}
	// nothing else to do .. all node in the list might have been removed now ...
	if (0!=m_list.Size()) {
		EWOL_ERROR("the subName element in the list are incorect...");
	}
}

ewol::Widget* widget::XmlComposer::GetMainWidget(void)
{
	// nothing else to do ...
	return m_widget;
}

ewol::Widget* widget::XmlComposer::GetWidgetNamed(const etk::UString& widgetName)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (m_list[iii].widgetName == widgetName) {
			return m_list[iii].widget;
		}
	}
	return NULL;
}

virtual void widget::XmlComposer::OnObjectRemove(ewol::EObject* removeObject)
{
	for (int32_t iii=0; iii<m_list.Size(); iii++) {
		if (m_list[iii].widgetName == widgetName) {
			m_list.Erase(iii);
		}
	}
}
