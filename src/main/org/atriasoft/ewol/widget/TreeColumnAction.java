/*
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.widget;

/**
 * Signal payload for column action events in a TreeView.
 * Carries the node and the column index that was clicked.
 * @param <T> The type of data stored in the tree node.
 */
public record TreeColumnAction<T>(TreeNode<T> node, int columnIndex) {
}
