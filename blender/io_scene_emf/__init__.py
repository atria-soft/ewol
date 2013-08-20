# add this folder in ~/.config/blender/2.66/scripts/addons


bl_info = {
    "name": "Ewol Mesh file format emf",
    "author": "Edouard DUPIN",
    "blender": (2, 53, 0),
    "location": "File > Import-Export",
    "description": "Import-Export emf, Import EMF mesh, UV's, materials and textures",
    "warning": "",
    "wiki_url": "http://wiki.blender.org/index.php/Extensions:2.6/Py/Scripts/Import-Export/EwolMechFile_EMF",
    "tracker_url": "",
    "support": 'OFFICIAL',
    "category": "Import-Export"}

if "bpy" in locals():
    import imp
    if "import_emf" in locals():
        imp.reload(import_emf)
    if "export_emf" in locals():
        imp.reload(export_emf)


import bpy
from bpy.props import (BoolProperty,
                       FloatProperty,
                       StringProperty,
                       EnumProperty,
                       )
from bpy_extras.io_utils import (ExportHelper,
                                 ImportHelper,
                                 path_reference_mode,
                                 axis_conversion,
                                 )


class ImportEMF(bpy.types.Operator, ImportHelper):
    """Load a Wavefront EMF File"""
    bl_idname = "import_scene.emf"
    bl_label = "Import EMF"
    bl_options = {'PRESET', 'UNDO'}

    filename_ext = ".emf"
    filter_glob = StringProperty(
            default="*.emf",
            options={'HIDDEN'},
            )

    axis_forward = EnumProperty(
            name="Forward",
            items=(('X', "X Forward", ""),
                   ('Y', "Y Forward", ""),
                   ('Z', "Z Forward", ""),
                   ('-X', "-X Forward", ""),
                   ('-Y', "-Y Forward", ""),
                   ('-Z', "-Z Forward", ""),
                   ),
            default='-X',
            )

    axis_up = EnumProperty(
            name="Up",
            items=(('X', "X Up", ""),
                   ('Y', "Y Up", ""),
                   ('Z', "Z Up", ""),
                   ('-X', "-X Up", ""),
                   ('-Y', "-Y Up", ""),
                   ('-Z', "-Z Up", ""),
                   ),
            default='Z',
            )

    def execute(self, context):
        # print("Selected: " + context.active_object.name)
        from . import import_obj

        keywords = self.as_keywords(ignore=("axis_forward",
                                            "axis_up",
                                            "filter_glob",
                                            "split_mode",
                                            ))

        global_matrix = axis_conversion(from_forward=self.axis_forward,
                                        from_up=self.axis_up,
                                        ).to_4x4()
        keywords["global_matrix"] = global_matrix

        return import_obj.load(self, context, **keywords)

    def draw(self, context):
        layout = self.layout

        row = layout.row(align=True)

        row = layout.split(percentage=0.67)
        row.prop(self, "global_clamp_size")
        layout.prop(self, "axis_forward")
        layout.prop(self, "axis_up")

        layout.prop(self, "use_image_search")


class ExportEMF(bpy.types.Operator, ExportHelper):
	"""Save a Wavefront EMF File"""
	
	bl_idname = "export_scene.emf"
	bl_label = 'Export EMF'
	bl_options = {'PRESET'}
	
	filename_ext = ".emf"
	filter_glob = StringProperty(
	    default="*.emf",
	    options={'HIDDEN'},
	    )
	
	# context group
	use_selection = BoolProperty(
	    name="Selection Only",
	    description="Export selected objects only",
	    default=True,
	    )
	# generate binary file
	use_binary = BoolProperty(
	    name="Binary",
	    description="Export the filein binary mode",
	    default=False,
	    )
	
	global_scale = FloatProperty(
	    name="Scale",
	    description="Scale all data",
	    min=0.01, max=1000.0,
	    soft_min=0.01,
	    soft_max=1000.0,
	    default=1.0,
	    )
	
	collision_object_name = StringProperty(
	    name="Collision root name (strat with)",
	    description="The top-level name that will contain the physics shapes",
	    default="phys"
	    )
	
	axis_forward = EnumProperty(
	    name="Forward",
	    items=(('X', "X Forward", ""),
	           ('Y', "Y Forward", ""),
	           ('Z', "Z Forward", ""),
	           ('-X', "-X Forward", ""),
	           ('-Y', "-Y Forward", ""),
	           ('-Z', "-Z Forward", ""),
	           ),
	    default='-Z',
	    )
	
	axis_up = EnumProperty(
	    name="Up",
	    items=(('X', "X Up", ""),
	           ('Y', "Y Up", ""),
	           ('Z', "Z Up", ""),
	           ('-X', "-X Up", ""),
	           ('-Y', "-Y Up", ""),
	           ('-Z', "-Z Up", ""),
	           ),
	    default='Y',
	    )
	
	path_mode = path_reference_mode
	
	check_extension = True
	
	def execute(self, context):
		from . import export_emf
		
		from mathutils import Matrix
		keywords = self.as_keywords(ignore=("axis_forward",
		                                    "axis_up",
		                                    "global_scale",
		                                    "check_existing",
		                                    "filter_glob",
		                                    ))
		
		global_matrix = Matrix()
		
		global_matrix[0][0] = \
		global_matrix[1][1] = \
		global_matrix[2][2] = self.global_scale
		
		global_matrix = (global_matrix *
		                 axis_conversion(to_forward=self.axis_forward,
		                                 to_up=self.axis_up,
		                                 ).to_4x4())
		
		keywords["global_matrix"] = global_matrix
		return export_emf.save(self, context, **keywords)


def menu_func_import(self, context):
	self.layout.operator(ImportEMF.bl_idname, text="Ewol mesh file (.emf)")


def menu_func_export(self, context):
	self.layout.operator(ExportEMF.bl_idname, text="Ewol mesh File (.emf)")


def register():
	bpy.utils.register_module(__name__)
	bpy.types.INFO_MT_file_import.append(menu_func_import)
	bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
	bpy.utils.unregister_module(__name__)
	bpy.types.INFO_MT_file_import.remove(menu_func_import)
	bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
	register()
