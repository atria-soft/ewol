/**
 * @ingroup ewolWidgetGroup
 */
class Menu:public ewol::widget::Sizer
{
	public:
		esignal::Signal<String> signalSelect; // event on a menu button or ...
	protected:
		Menu();
	public:
		DECLARE_WIDGET_FACTORY(Menu, "Menu");
		 ~Menu();
	private:
	
	void subWidgetRemoveAll();
	
	int subWidgetAdd(Widget _newWidget);
	
	void subWidgetRemove(Widget _newWidget);
	
	void subWidgetUnLink(Widget _newWidget);
	
	boolean loadXML( exml::Element _node) ;
	private:
		List<ewol::widget::MenuElement> this.listElement;
		int this.staticObjectId; // unique ID for every element of the menu ...
		ewol::widget::ContextMenuWeak this.widgetContextMenu;
	
	int get(String _label);
	
	public:
		void clear();
	
	int addTitle( final String _label,  final String _image="",  String _message = "");
	
	int add(final int _parent,  final String _label,  final String _image="",  String _message = "");
	
	int addSpacer(final int _parent=-1);
	
	void remove(int _id);
	
	private:
		void onButtonPressed(ewol::widget::ButtonWeak _button);
};
	
	;
	
	/** @file
	 * @author Edouard DUPIN
	 * @copyright 2011, Edouard DUPIN, all right reserved
	 * @license MPL v2.0 (see license file)
	 */
	class MenuElement {
		public MenuElement() { };int this.localId;int this.parentId;WeakReference<Widget>this.widgetPointer;String this.label;String this.image;String this.message;
}