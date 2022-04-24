
/**
 * a composed Select is a Select with an inside composed with the specify XML element 
 * ==> this permit to generate standard element simple
 */
class Select extends SpinBase {
	public Signal<int> signalValue = new Signal<int>();
	protected int propertyValue; //!< Current state of the Select.
		/**
		 * Constructor
		 * @param _shaperName Shaper file properties
		 */
	public Select();
	protected class Element {
			public:
				int value;
				String name;
				boolean selected;
			public:
				// TODO: Remove this: due to the fact my List is not full implemented
				Element() {}
				Element(int _value, String _name, boolean _selected=false);
		};
		protected List<ewol::widget::Select::Element> this.listElement;
	public void optionSelectDefault();
	public void optionRemove(int _value);
	public void optionClear();
	public void optionAdd(int _value, String _name);
	protected boolean loadXML( exml::Element _node) ;
	protected void updateGui() ;
	protected void onCallbackOpenMenu();
	protected void onCallbackLabelPressed(int _value);
	protected esignal::Connection connectionEntry = null;
	protected esignal::Connection connectionButton = null;
	protected void onChangePropertyValue();
}
