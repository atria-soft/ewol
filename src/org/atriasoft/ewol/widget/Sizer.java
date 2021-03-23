/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
/**
 * @ingroup ewolWidgetGroup
 */
public class Sizer extends ContainerN {
	public enum displayMode {
			modeVert, //!< Vertical mode
			modeHori, //!< Horizontal mode
		};
	public displayMode propertyMode; //!< Methode to display the widget list (vert/hory ...)
	public Dimension propertyBorderSize; //!< Border size needed for all the display
	/**
	 * Constructor
	 * @param _mode The mode to display the elements
	 */
	public Sizer();
	public 	void onChangeSize() ;
	public void calculateMinMaxSize() ;
		// overwrite the set fuction to start annimations ...
		public 		int subWidgetAdd(Widget _newWidget) ;
	public int subWidgetAddStart(Widget _newWidget) ;
	public void subWidgetRemove(Widget _newWidget) ;
	public void subWidgetUnLink(Widget _newWidget) ;
	protected void onChangePropertyMode();
	protected  void onChangePropertyBorderSize();
}

