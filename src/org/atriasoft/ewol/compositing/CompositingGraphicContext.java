/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.compositing;

import org.atriasoft.egami.ImageByte;
import org.atriasoft.esvg.CapMode;
import org.atriasoft.esvg.GraphicContext;
import org.atriasoft.esvg.JoinMode;
import org.atriasoft.etk.Color;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.gale.resource.ResourceTexture2;

public class CompositingGraphicContext extends Compositing {
	GraphicContext context = new GraphicContext();
	private final ResourceTexture2 texture = new ResourceTexture2();
	
	public CompositingGraphicContext() {
		
	}
	
	public Vector2i calculateTextSize(final String data) {
		return this.context.calculateTextSize(data);
	}
	
	public void circle(final Vector2f position, final float radius) {
		this.context.circle(position, radius);
	}
	
	/**
	 * clear alll tre registered element in the current element
	 */
	@Override
	public void clear() {
		// call upper class
		super.clear();
		// reset Buffer :
		this.context.clear();
	}
	
	/**
	 * Clear the fill color (disable fill ==> better that set it transparent)
	 */
	public void clearColorFill() {
		this.context.clearColorFill();
	}
	
	/**
	 * Clear the Stroke color (disable stroke)
	 */
	public void clearColorStroke() {
		this.context.clearColorStroke();
	}
	
	@Override
	public void draw(final boolean disableDepthTest) {
		// TODO Auto-generated method stub
		
	}
	
	public void ellipse(final Vector2f center, final Vector2f radius) {
		this.context.ellipse(center, radius);
	}
	
	@Override
	public void flush() {
		if (this.texture == null) {
			Log.warning("texture is null");
			return;
		}
		ImageByte img = this.context.render();
		//IOgami.storePNG(new Uri("/home/heero/000000000aaaaplopppp222.png"), img);
		this.texture.set(img);
		this.texture.flush();
	}
	
	/**
	 * Get the fill color.
	 * @return fill color.
	 */
	public Color getColorFill() {
		return this.context.getColorFill();
	}
	
	/**
	 * Get the stroke color.
	 * @return Stroke color.
	 */
	public Color getColorStroke() {
		return this.context.getColorStroke();
	}
	
	public CapMode getLineCap() {
		return this.context.getLineCap();
	}
	
	public JoinMode getLineJoin() {
		return this.context.getLineJoin();
	}
	
	public float getMiterLimit() {
		return this.context.getMiterLimit();
	}
	
	public float getOpacity() {
		return this.context.getOpacity();
	}
	
	/**
	 * get the source image registered size in the file (<0 when multiple size image)
	 * @return tre image registered size
	 */
	public Vector2i getRealSize() {
		return this.texture.get().getSize();
	}
	
	public int getRendererId() {
		return this.texture.getRendererId();
	}
	
	public ResourceTexture2 getResourceTexture() {
		return this.texture;
	}
	
	public float getStrokeWidth() {
		return this.context.getStrokeWidth();
	}
	
	public int getTextHeight() {
		return this.context.getTextHeight();
	}

	public float getTextSize() {
		return this.context.getTextSize();
	}
	
	/**
	 * Sometimes the user declare an image but not allocate the ressources all the time, this is to know it ..
	 * @return the validity od the resources.
	 */
	public boolean hasSources() {
		return true;
	}
	
	public void line(final Vector2f origin, final Vector2f destination) {
		this.context.line(origin, destination);
	}
	
	public void lineRel(final Vector2f origin, final Vector2f relativeDestination) {
		this.context.lineRel(origin, relativeDestination);
	}
	
	public void pathLine(final Vector2f pos) {
		this.context.pathLine(pos);
	}
	
	public void pathLineTo(final Vector2f pos) {
		this.context.pathLineTo(pos);
		
	}
	
	public void pathMove(final Vector2f pos) {
		this.context.pathMove(pos);
	}
	
	public void pathMoveTo(final Vector2f pos) {
		this.context.pathMoveTo(pos);
	}
	
	public void pathStart() {
		this.context.pathStart();
	}
	
	public void pathStop() {
		this.context.pathStop();
	}
	
	public void pathStopLinked() {
		this.context.pathStopLinked();
	}
	
	public void rectangle(final Vector2f position, final Vector2f destination) {
		this.context.rectangle(position, destination);
	}
	
	public void rectangleRounded(final Vector2f position, final Vector2f destination, final Vector2f ruound) {
		this.context.rectangleRounded(position, destination, ruound);
	}
	
	public void rectangleRoundedWidth(final Vector2f position, final Vector2f width, final Vector2f ruound) {
		this.context.rectangleRoundedWidth(position, width, ruound);
	}
	
	public void rectangleWidth(final Vector2f position, final Vector2f width) {
		this.context.rectangleWidth(position, width);
	}
	
	/**
	 * set the fill color
	 * @param color Color to set on fill
	 * @apiNote use clearFill() if you want to remove drawing of fill
	 */
	public void setColorFill(final Color color) {
		this.context.setColorFill(color);
	}
	
	/**
	 * set the stroke color
	 * @param color Color to set on stroke
	 * @apiNote use clearStroke() if you want to remove drawing of stroke
	 */
	public void setColorStroke(final Color color) {
		this.context.setColorStroke(color);
	}
	
	public void setLineCap(final CapMode lineCap) {
		this.context.setLineCap(lineCap);
	}
	
	public void setLineJoin(final JoinMode lineJoin) {
		this.context.setLineJoin(lineJoin);
	}
	
	public void setMiterLimit(final float miterLimit) {
		this.context.setMiterLimit(miterLimit);
	}
	
	public void setOpacity(final float opacity) {
		this.context.setOpacity(opacity);
	}
	
	/**
	 * Set global size of the Graphic context (output render size)
	 * @param xxx Width of the image
	 * @param yyy Height of the image
	 * @apiNote It will clear the current context.
	 */
	public void setSize(final int xxx, final int yyy) {
		this.context.setSize(xxx, yyy);
	}
	
	/**
	 * Set global size of the Graphic contexct (output render size)
	 * @param vector2i New size of the image
	 * @apiNote It will clear the current context.
	 */
	public void setSize(final Vector2i size) {
		this.context.setSize(size.x(), size.y());
	}
	
	public void setStrokeWidth(final float strokeWidth) {
		this.context.setStrokeWidth(strokeWidth);
	}
	
	public void text(final Vector2f position, final float height, final String data) {
		this.context.text(position, height, data);
	}
	
	public void text(final Vector2f position, final String data) {
		this.context.text(position, data);
	}
}
