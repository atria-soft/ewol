package sample.atriasoft.ewol;

import org.atriasoft.ewol.widget.TreeNode;
import org.atriasoft.ewol.widget.TreeView;
import org.atriasoft.ewol.widget.Widget;

public class TestWidgetTreeView implements TestWidgetInterface {

	@Override
	public Widget getWidget() {
		// Build a static tree
		final TreeNode<String> root = new TreeNode<>("root", "Animals");

		final TreeNode<String> mammals = new TreeNode<>("mammals", "Mammals");
		mammals.addChild(new TreeNode<>("dog", "Dog", true));
		mammals.addChild(new TreeNode<>("cat", "Cat", true));
		mammals.addChild(new TreeNode<>("elephant", "Elephant", true));
		root.addChild(mammals);

		final TreeNode<String> birds = new TreeNode<>("birds", "Birds");
		birds.addChild(new TreeNode<>("eagle", "Eagle", true));
		birds.addChild(new TreeNode<>("sparrow", "Sparrow", true));

		final TreeNode<String> raptors = new TreeNode<>("raptors", "Raptors");
		raptors.addChild(new TreeNode<>("hawk", "Hawk", true));
		raptors.addChild(new TreeNode<>("falcon", "Falcon", true));
		birds.addChild(raptors);
		root.addChild(birds);

		final TreeNode<String> reptiles = new TreeNode<>("reptiles", "Reptiles");
		reptiles.addChild(new TreeNode<>("snake", "Snake", true));
		reptiles.addChild(new TreeNode<>("lizard", "Lizard", true));
		root.addChild(reptiles);

		root.setExpanded(true);
		mammals.setExpanded(true);

		// Create TreeView with 2 columns
		final TreeView treeView = TreeView.create()
				.rootNode(root)
				.column("Name", 250.0f, TreeView.defaultLabelRenderer())
				.column("Data", 150.0f, (gc, text, node, pos, size, sel) -> {
					text.setPos(pos);
					final Object data = node.getData();
					if (data != null) {
						text.print(data.toString());
					}
				})
				.showRoot(true)
				.showHeaders(true);

		treeView.expand(true, true).fill(true, true);

		return treeView;
	}

	@Override
	public String getTitle() {
		return "TreeView";
	}

	@Override
	public String getDescription() {
		return "Generic tree view with expand/collapse and columns";
	}

	@Override
	public String getCategory() {
		return "Data";
	}
}
