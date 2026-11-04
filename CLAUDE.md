# EWOL - Widget Framework

Java widget framework with OpenGL rendering via GALE. Provides a complete UI toolkit with layout, input widgets, event system, and fluent API.

## Quick Start

### Minimal Application

```java
// 1. Main entry point
public class Main {
    public static void main(String[] args) {
        Ewol.init();
        Uri.setApplication(Main.class, "my-app/");
        Ewol.run(new MyApp(), args);
    }
}

// 2. Application lifecycle (implement EwolApplication)
public class MyApp implements EwolApplication {
    @Override
    public void onCreate(EwolContext context) {
        context.setSize(new Vector2f(800, 600));
        Configs.getConfigFonts().set("FreeSherif", 12);
        context.setWindows(new MyWindows());
    }
    @Override public void onStart(EwolContext context) {}
    @Override public void onStop(EwolContext context) {}
    @Override public void onPause(EwolContext context) {}
    @Override public void onResume(EwolContext context) {}
    @Override public void onDestroy(EwolContext context) {}
}

// 3. Main window
public class MyWindows extends Windows {
    public MyWindows() {
        setPropertyTitle("My Application");
        setSubWidget(
            Sizer.vertical()
                .expand(true, true)
                .fill(true, true)
                .add(new Label("Hello World!"))
                .add(Button.create("Click me").onClick(() -> System.out.println("Clicked!")))
        );
    }
}
```

## Build / test

Maven module of the multi-module project (see the root `CLAUDE.md` for the build order and commit format):
`. .island/manifest/java-home.sh && /usr/bin/mvn install -DskipTests`. Known failing tests: ComposerTest, ProgressBarTest, etc.

## Where to find what (`doc/`, read on demand: `cs PATTERN doc/`)

| File | Content |
|------|---------|
| `doc/ARCHITECTURE.md` | Class hierarchy, key packages, source file structure |
| `doc/WIDGETS.md` | Widget properties (gravity, Vector2b) and the reference widget by widget: Windows, Sizer, Label, Button, Entry, CheckBox, RadioButton/RadioGroup, FieldSet, Slider, ColorGradient, ColorPickerPopup, Select, ScrollView, SplitPane, ImageDisplay, Icon, ProgressBar, Container, Box, PopUp, Dialog, Spacer, TreeView, TreeFileSystem, Toast, Menu system (context menu, menu bar) |
| `doc/SIGNALS_AND_CUSTOM_WIDGETS.md` | Signal system (esignal), widget lifecycle, creating a custom widget, key override methods, rendering with compositing, dimension/distance types, URI system, keyboard shortcuts |
| `doc/PITFALLS.md` | Full detail (with code) of the pitfalls below |

## Dependencies

| Library | Purpose |
|---------|---------|
| `gale` | Windowing wrapper, OpenGL rendering, input events |
| `etk` | Core utilities (Vector2f, Color, Dimension2f, Uri, etc.) |
| `esignal` | Signal/slot event system (weak pointer based) |
| `aknot` | Annotation system for properties (@AknotAttribute, @AknotSignal, etc.) |
| `esvg` | SVG rendering |
| `egami` | Image handling (ToolImage wraps javax.imageio) |
| `ejson` | JSON parsing (configs, color files) |

## Common Pitfalls (details: `doc/PITFALLS.md`)

- `getWidgetAtPos()` decides who receives events: an overlay returning `this` swallows all mouse events; return `null` to be transparent. → doc/PITFALLS.md#event-system
- `KeyStatus.leave` fires when leaving the target widget, not the parent: check the mouse is not still inside the parent. → doc/PITFALLS.md#event-system
- Override `systemEventInput()` (not `onEventInput()`) to observe events without consuming them; `inputId == 0` = cursor movement. → doc/PITFALLS.md#event-system
- Children use absolute coordinates: call `parent.onChangeSize()` after `setOrigin()`; but it is heavy, use a fast path for per-frame moves. → doc/PITFALLS.md#layout-and-positioning
- The popup stack triggers `onChangeSize()` on all widgets: not for lightweight overlays (use a dedicated layer). → doc/PITFALLS.md#layout-and-positioning
- Never reuse a Widget instance across show/dismiss cycles; use a `Supplier<Widget>` factory. → doc/PITFALLS.md#widget-reuse
- `systemDrawWidget()` sets the viewport outside push/pop: clip overlays with `displayProp.withLimit()`; layer order is main, popups, popovers, toasts. → doc/PITFALLS.md#rendering
- Split static (cached `CompositingGC`) and dynamic rendering; cache `Uri.getAllDataString()` results. → doc/PITFALLS.md#rendering
- Resources (gale `create()`/`keep()`/`release()`): widgets have no destruction step, give each resource an owner (`OwnedResources`, `own(...)`), use `ResourceColorFile.create(...)` not `new`. → doc/PITFALLS.md#resources-gale-createkeeprelease
- `Container(Widget)` constructor does NOT call `setParent()`: use `setSubWidget()`. → doc/PITFALLS.md#container
