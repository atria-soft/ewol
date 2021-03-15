import org.atriasoft.etk.math.Matrix4f;
import org.atriasoft.etk.math.Vector3f;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public abstract class Compositing {
	protected Matrix4f matrixApply = Matrix4f.identity();
	
	/**
	 * @brief clear alll tre registered element in the current element
	 */
	public void clear() {
		this.matrixApply.setIdentity();
	}
	
	/**
	 * @brief Virtal pure function that request the draw of all openGl elements
	 */
	public void draw() {
		draw(true);
	}
	
	public abstract void draw(final boolean _disableDepthTest);
	
	/**
	 * @brief reset to the eye matrix the openGL mouving system
	 */
	public void resetMatrix() {
		this.matrixApply.setIdentity();
	}
	
	/**
	 * @brief rotate the curent display of this element
	 * @param[in] _vect The rotation vector to apply at the transformation matrix
	 */
	public void rotate(final Vector3f _vect, final float _angle) {
		this.matrixApply.multiply(Matrix4f.createMatrixRotate(_vect, _angle));
	}
	
	/**
	 * @brief scale the current diaplsy of this element
	 * @param[in] _vect The scaling vector to apply at the transformation matrix
	 */
	public void scale(final Vector3f _vect) {
		this.matrixApply.multiply(Matrix4f.createMatrixScale(_vect));
	}
	
	/**
	 * @brief set the transformation matrix
	 * @param[in] _mat The new matrix.
	 */
	public void setMatrix(final Matrix4f _mat) {
		this.matrixApply = _mat;
	}
	
	/**
	 * @brief translate the current display of this element
	 * @param[in] _vect The translation vector to apply at the transformation matrix
	 */
	public void translate(final Vector3f _vect) {
		this.matrixApply.multiply(Matrix4f.createMatrixTranslate(_vect));
	}
}
