/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.resource;

import org.atriasoft.egami.Image;
import org.atriasoft.etk.Uri;
import org.atriasoft.etk.math.Vector2i;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.ewol.internal.Tools;
import org.atriasoft.gale.resource.Resource;

// TODO : Change tis file name ...

public class ResourceTextureFile extends ResourceTexture2 {
	public static Vector2i sizeAuto = new Vector2i(-1, -1);
	public static Vector2i sizeDefault = new Vector2i(0, 0);

	/**
	 * keep the resource pointer.
	 * @note Never free this pointer by your own...
	 * @param _filename Name of the image file.
	 * @param _requested size of the image (usefull when loading .svg to
	 *            automatic rescale)
	 * @param _sizeRegister size register in named (When you preaload the images
	 *            the size write here will be )
	 * @return pointer on the resource or null if an error occured.
	 */
	public static ResourceTextureFile create(final Uri _filename) {
		return create(_filename, sizeAuto);
	}

	public static ResourceTextureFile create(final Uri _filename, final Vector2i _size) {
		return create(_filename, _size, sizeAuto);
	}

	public static ResourceTextureFile create(final Uri _uri, final Vector2i _size, final Vector2i _sizeRegister) {
		Log.verbose("KEEP: TextureFile: '" + _uri + "' size=" + _size + " sizeRegister=" + _sizeRegister);
		Vector2i size = _size;
		if (_uri == null) {
			final ResourceTextureFile object = new ResourceTextureFile();
			return object;
		}
		if (size.x() == 0) {
			size = size.withX(-1);
			// Log.error("Error Request the image size.x() =0 ???");
		}
		if (size.y() == 0) {
			size = size.withY(-1);
			// Log.error("Error Request the image size.y() =0 ???");
		}
		final Uri tmpFilename = _uri;
		if (_uri.getExtention().toLowerCase().contentEquals("svg") == false) {
			size = sizeAuto;
		}
		if (size.x() > 0 && size.y() > 0) {
			Log.verbose("     == > specific size : " + size);
			size = new Vector2i(Tools.nextP2(size.x()), Tools.nextP2(size.y()));
			if (_sizeRegister.equals(sizeAuto) == false) {
				if (_sizeRegister.equals(sizeDefault) == false) {
					// tmpFilename.getQuery().set("x", "" + size.x));
					// tmpFilename.getQuery().set("y", "" + size.y));
				}
			}
		}

		Log.verbose("KEEP: TextureFile: '" + tmpFilename + "' new size=" + size);
		ResourceTextureFile object = null;
		final Resource object2 = getManager().localKeep(tmpFilename.toString());
		if (object2 != null) {
			object = (ResourceTextureFile) object2;
			if (object == null) {
				Log.critical("Request resource file : '" + tmpFilename + "' With the wrong type (dynamic cast error)");
				return null;
			}
		}
		if (object != null) {
			return object;
		}
		Log.debug("CREATE: TextureFile: '" + tmpFilename + "' size=" + size);
		// need to crate a new one ...
		object = new ResourceTextureFile(tmpFilename.toString(), _uri, size);
		getManager().localAdd(object);
		return object;
	}

	protected ResourceTextureFile() {
		super();
	}

	protected ResourceTextureFile(final String _genName, final Uri _uri, final Vector2i _size) {
		super(_genName);
		Log.debug("create a new resource::Image : _genName=" + _genName + " _uri=" + _uri + " size=" + _size);
		final Image tmp = Egami.load(_uri, _size);
		set(tmp);
	}

	public Vector2i getRealSize() {
		return this.realImageSize;
	}

}
