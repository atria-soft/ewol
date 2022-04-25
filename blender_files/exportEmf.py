import bpy

##To export manualy a emf... (for test ...)
#      blender --background -P ./exportEmf.py



list_elem = [
	"Entry",
	"CheckBox",
	"ScrollBar"
	]

for elem in list_elem:
	bpy.ops.wm.open_mainfile(filepath="/home/heero/dev/WORKSPACE-game/jatria-soft/ewol/blender_files/" + elem + ".blend")
	bpy.ops.export_scene.emf(filepath="/home/heero/dev/WORKSPACE-game/jatria-soft/ewol/resources/resources/ewol/theme/shape/" + elem + ".emf")
