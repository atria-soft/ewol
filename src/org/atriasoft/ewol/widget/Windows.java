package org.atriasoft.ewol.widget;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.DrawProperty;
import org.atriasoft.ewol.object.EwolObject;
import org.atriasoft.ewol.resource.ResourceColorFile;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

/**
 * Windows basic interface
 */
public class Windows extends Widget {
	
	public Uri propertyColorConfiguration; //!< Configuration file of the windows theme
	public String propertyTitle; //!< Current title of the windows
	protected ResourceColorFile resourceColor; //!< theme color property (name of file in @ref propertyColorConfiguration)
	protected int colorBg; //!< Default background color of the windows
	protected List<Widget> popUpWidgetList = new ArrayList<Widget>();
	
	// internal event at ewol system:
	
	protected Widget subWidget;
	
	protected Windows();
	
	@Override
	public void drawWidgetTree(int _level); //!< List of pop-up displayed
	
	@Override
	public EwolObject getSubObjectNamed(String _objectName);
	
	public Widget getWidgetAtPos(Vector2f _pos);
	
	/**
	 * Called when property change: Color configuration file
	 */
	protected void onChangePropertyColor();
	
	/**
	 * Called when property change: Title
	 */
	protected void onChangePropertyTitle();
	
	@Override
	public void onChangeSize();
	
	@Override
	public void onRegenerateDisplay();
	
	/**
	 * Get the number of pop-up
	 * @return Count of pop-up
	 */
	public int popUpCount() {
		return this.popUpWidgetList.size();
	}
	
	/**
	 * Remove the pop-up on top.
	 */
	public void popUpWidgetPop();
	
	/**
	 * Add a pop-up on the Windows.
	 * @param _widget Widget to set on top of the pop-up.
	 */
	public void popUpWidgetPush(Widget _widget);
	
	public void requestDestroyFromChild(EwolObject _child); //!< main sub-widget of the Windows.
	
	/**
	 * Set the main widget of the application.
	 * @param _widget Widget to set in the windows.
	 */
	public void setSubWidget(Widget _widget);
	
	public void sysDraw();
	
	protected void systemDraw(DrawProperty _displayProp);
}
