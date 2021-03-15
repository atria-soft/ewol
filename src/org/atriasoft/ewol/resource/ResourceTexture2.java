/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import org.atriasoft.echrono.Steady;
import org.atriasoft.egami.Image;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.internal.Tools;
import org.atriasoft.gale.backend3d.OpenGL;
import org.atriasoft.gale.resource.Resource;
import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL13;

public class ResourceTexture2 extends Resource {
	public enum TextureColorMode {
		rgb, //!< red/green/blue data
		rgba //!< red/green/blue/alpha data
	}
	
	private static int[] textureIdBinding = { GL13.GL_TEXTURE0, GL13.GL_TEXTURE1, GL13.GL_TEXTURE2, GL13.GL_TEXTURE3, GL13.GL_TEXTURE4, GL13.GL_TEXTURE5, GL13.GL_TEXTURE6, GL13.GL_TEXTURE7,
			GL13.GL_TEXTURE8, GL13.GL_TEXTURE9, GL13.GL_TEXTURE10, GL13.GL_TEXTURE11, GL13.GL_TEXTURE12, GL13.GL_TEXTURE13, GL13.GL_TEXTURE14, GL13.GL_TEXTURE15, GL13.GL_TEXTURE16, GL13.GL_TEXTURE17,
			GL13.GL_TEXTURE18, GL13.GL_TEXTURE19, GL13.GL_TEXTURE20, GL13.GL_TEXTURE21, GL13.GL_TEXTURE22, GL13.GL_TEXTURE23, GL13.GL_TEXTURE24, GL13.GL_TEXTURE25, GL13.GL_TEXTURE26,
			GL13.GL_TEXTURE27, GL13.GL_TEXTURE28, GL13.GL_TEXTURE29, GL13.GL_TEXTURE30, GL13.GL_TEXTURE31 };;
	
	/*
	public static ResourceTexture2 createFromPng(final Uri uriTexture) {
		return createFromPng(uriTexture, 1);
	}
	
	public static ResourceTexture2 createFromPng(final Uri uriTexture, final int textureUnit) {
		ResourceTexture2 resource;
		Resource resource2;
		final String name = uriTexture.getValue();
		if (name.isEmpty() == false && name != "---") {
			resource2 = getManager().localKeep(name);
		} else {
			Log.error("Can not create a shader without a filaname");
			return null;
		}
		if (resource2 != null) {
			if (resource2 instanceof ResourceTexture2) {
				resource2.keep();
				return (ResourceTexture2) resource2;
			}
			Log.critical("Request resource file : '" + name + "' With the wrong type (dynamic cast error)");
			return null;
		}
		resource = new ResourceTexture2(uriTexture, textureUnit);
		final ImageRawData decodedData = ImageLoader.decodePngFile(uriTexture);
		resource.setTexture(decodedData.getBuffer(), new Vector2i(decodedData.getWidth(), decodedData.getHeight()), (decodedData.isHasAlpha() == true ? TextureColorMode.rgba : TextureColorMode.rgb),
				textureUnit);
		resource.flush();
		return resource;
	}
	*/
	
	/**
	 * @brief get the next power 2 if the input
	 * @param value Value that we want the next power of 2
	 * @return result value
	 */
	private static int nextP2(final int value) {
		int val = 1;
		for (int iii = 1; iii < 31; iii++) {
			if (value <= val) {
				return val;
			}
			val *= 2;
		}
		Log.critical("impossible CASE....");
		return val;
	}
	
	protected int texId = -1; //!< openGl textureID.
	// openGl Context properties :
	protected Image data = new Image(32, 32);
	//! Last loaded size in the system openGL
	protected Vector2i lastSize = new Vector2i(1, 1);
	//! some image are not square == > we need to sqared it to prevent some openGl api error the the displayable size is not all the time 0.0 . 1.0
	protected Vector2i realImageSize = new Vector2i(1, 1);
	// internal state of the openGl system.
	protected boolean loaded = false;
	protected int lastTypeObject = 0;
	protected int lastSizeObject = 0;
	// repeat mode of the image (repeat the image if out of range [0..1])
	protected boolean repeat = false;
	
	// Filter apply at the image when rendering it
	protected TextureFilter filter = TextureFilter.linear;
	
	//!< Color space of the image.
	private final TextureColorMode dataColorSpace = TextureColorMode.rgba;
	
	public ResourceTexture2() {
		super();
	}
	
	public ResourceTexture2(final String filename) {
		super(filename);
	}
	
	/*
	 public void bindForRendering(final int idTexture) {
		if (this.loaded == false) {
			return;
		}
		GL13.glActiveTexture(textureIdBinding[idTexture]);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, this.texId);
		if (this.dataColorSpace == TextureColorMode.rgb) {
			OpenGL.enable(OpenGL.Flag.flag_cullFace);
			OpenGL.enable(OpenGL.Flag.flag_back);
		}
	}
	*/
	
	public ResourceTexture2(final Uri filename) {
		super(filename);
	}
	
	public void bindForRendering(final int idTexture) {
		if (this.loaded == false) {
			return;
		}
		GL13.glActiveTexture(textureIdBinding[idTexture]);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, this.texId);
		if (this.dataColorSpace == TextureColorMode.rgb) {
			OpenGL.enable(OpenGL.Flag.flag_cullFace);
			OpenGL.enable(OpenGL.Flag.flag_back);
		}
	}
	
	@Override
	public void cleanUp() {
		removeContext();
	}
	
	// Flush the data to send it at the openGl system
	public synchronized void flush() {
		// request to the manager to be call at the next update ...
		Log.verbose("Request UPDATE of Element");
		getManager().update(this);
	};
	
	// Get the reference on this image to draw something on it ...
	public Image get() {
		return this.data;
	}
	
	public Vector2i getOpenGlSize() {
		return this.data.getSize();
	}
	
	public int getRendererId() {
		return this.texId;
	}
	
	public Vector2i getUsableSize() {
		return this.realImageSize;
	}
	
	@Override
	public synchronized void removeContext() {
		if (this.loaded == true) {
			// Request remove texture ...
			Log.info("TEXTURE: Rm [" + getId() + "] texId=" + this.texId);
			// TODO Check if we are in the correct thread
			OpenGL.glDeleteTextures(this.texId);
			this.loaded = false;
		}
	}
	
	@Override
	public synchronized void removeContextToLate() {
		this.loaded = false;
		this.texId = -1;
	};
	
	/**
	 * @brief Set the image in the texture system
	 * @note It will resize in square2 if needed by the system.
	 * @param[in] _image Image to set.
	 */
	public synchronized void set(final Image _image) {
		Log.debug("Set a new image in a texture:");
		Log.debug("    size=" + _image.getSize());
		this.data = _image;
		this.realImageSize = this.data.getSize().clone();
		final Vector2i compatibilityHWSize = new Vector2i(Tools.nextP2(this.realImageSize.x), Tools.nextP2(this.realImageSize.y));
		if (this.realImageSize != compatibilityHWSize) {
			Log.verbose("RESIZE Image for HArwareCompatibility:" + this.realImageSize + " => " + compatibilityHWSize);
			this.data.resize(compatibilityHWSize.x, compatibilityHWSize.y);
		}
		flush();
	};
	
	/**
	 * @brief Set the Filter mode to apply at the image when display with a scale (not 1:1 ratio)
	 * @param[in] _value Value of the new filter mode
	 */
	public void setFilterMode(final TextureFilter _filter) {
		this.filter = _filter;
	}
	
	// You must set the size here, because it will be set in multiple of pow(2)
	public synchronized void setImageSize(final Vector2i _newSize) {
		_newSize.setValue(Tools.nextP2(_newSize.x), Tools.nextP2(_newSize.y));
		this.data.resize(_newSize.x, _newSize.y);
	};
	
	/**
	 * @brief Set the repeate mode of the images if UV range is out of [0..1]
	 * @param[in] _value Value of the new repeate mode
	 */
	public void setRepeat(final boolean _value) {
		this.repeat = _value;
	}
	
	public void unBindForRendering() {
		if (this.loaded == false) {
			return;
		}
		if (this.dataColorSpace == TextureColorMode.rgb) {
			OpenGL.disable(OpenGL.Flag.flag_cullFace);
			OpenGL.disable(OpenGL.Flag.flag_back);
		}
	}
	
	@Override
	public synchronized boolean updateContext() {
		Log.verbose("updateContext [START]");
		final Steady tic = Steady.now();
		/* TODO : use unlockable synchronized ... 
		if (lock.tryLock() == false) {
			//Lock error ==> try later ...
			return false;
		}
		*/
		final int typeObject = OpenGL.GL_RGBA;
		final int sizeObject = OpenGL.GL_UNSIGNED_BYTE;
		if (this.loaded == true) {
			if (this.lastTypeObject != typeObject || this.lastSizeObject != sizeObject || this.lastSize.equals(this.data.getSize()) == false) {
				Log.warning("TEXTURE: Rm [" + getId() + "] texId=" + this.texId);
				OpenGL.glDeleteTextures(this.texId);
				this.loaded = false;
			}
		}
		if (this.loaded == false) {
			// Request a new texture at openGl :
			this.texId = OpenGL.glGenTextures();
			this.lastSize = this.data.getSize();
			this.lastTypeObject = typeObject;
			this.lastSizeObject = sizeObject;
			Log.debug("TEXTURE: add [" + getId() + "]=" + this.data.getSize() + "=>" + this.data.getGPUSize() + " OGl_Id=" + this.texId + " type=" + this.data.getClass().getCanonicalName());
		} else {
			Log.debug("TEXTURE: update [" + getId() + "]=" + this.data.getSize() + "=>" + this.data.getGPUSize() + " OGl_Id=" + this.texId + " type=" + this.data.getClass().getCanonicalName());
		}
		// in all case we set the texture properties :
		// TODO : check error ???
		OpenGL.bindTexture2D(this.texId);
		
		if (this.loaded == false) {
			if (this.repeat == false) {
				OpenGL.setTexture2DWrapClampToEdge();
			} else {
				OpenGL.setTexture2DWrapRepeat();
			}
			if (this.filter == TextureFilter.linear) {
				OpenGL.setTexture2DFilterLinear();
			} else {
				OpenGL.setTexture2DFilterNearest();
			}
		}
		//glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		final Steady toc1 = Steady.now();
		Log.verbose("    BIND                 ==> " + toc1.less(tic));
		//egami::store(this.data, String("~/texture_") + etk::toString(getId()) + ".bmp");
		/*if (false) {
			// On some embended target, the texture size must be square of 2:
			if (this.loaded == false) {
				// 1: Create the square 2 texture:
				final int bufferSize = this.data.getGPUSize().x() * this.data.getGPUSize().y() * 8;
				static List<float> tmpData;
				if (tmpData.size() < bufferSize) {
					tmpData.resize(bufferSize, 0.0f);
				}
				Log.debug("    CREATE texture ==> " + this.data.getGPUSize());
				// 2 create a new empty texture:
				OpenGL.glTexImage2D(GL_TEXTURE_2D, // Target
				             0, // Level
				             typeObject, // Format internal
				             this.data.getGPUSize().x(),
				             this.data.getGPUSize().y(),
				             0, // Border
				             typeObject, // format
				             sizeObject, // type
				             tmpData[0] );
			
			}
			//3 Flush all time the data:
			Steady tic1 = Steady.now();
			glTexSubImage2D(GL_TEXTURE_2D, // Target
			                0, // Level
			                0, // x offset
			                0, // y offset
			                this.data.getWidth(),
			                this.data.getHeight(),
			                typeObject, // format
			                sizeObject, // type
			                (void*)((char*)this.data.getTextureDataPointer()) );
			Steady toc2 = Steady.now();
			Log.info("    updateContext [STOP] ==> " + toc2.less(tic1));
		} else */if (this.loaded == false) {
			OpenGL.glTexImage2D(0, // Level
					typeObject, // Format internal
					this.data.getWidth(), this.data.getHeight(), 0, // Border
					typeObject, // format
					sizeObject, // type
					this.data.GetRaw());
			
		} else {
			OpenGL.glTexSubImage2D(0, // Level
					0, // x offset
					0, // y offset
					this.data.getWidth(), this.data.getHeight(), typeObject, // format
					sizeObject, // type
					this.data.GetRaw());
		}
		// now the data is loaded
		this.loaded = true;
		final Steady toc = Steady.now();
		//Log.error("    updateContext [STOP] ==> " + (toc - toc1));
		return true;
	}
};

enum TextureFilter {
	nearest,
	linear
}
