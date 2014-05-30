#!/usr/bin/python
# --------------------------------------------------------
# -- Bullet librairy
# --------------------------------------------------------
import lutinModule as module
import lutinTools as tools

def get_desc():
	return "Bullet lib : C++ physic engine"


def create(target):
	myModule = module.Module(__file__, 'bullet', 'LIBRARY')
	
	myModule.add_module_depend(['linearmath'])
	#remove compilation warning (specific for external libs):
	myModule.remove_compile_warning()
	
	myModule.compile_flags_CC([
		'-Wno-write-strings',
		'-DHAVE_CONFIG_H',
		'-O2'])
	
	myModule.add_export_path(tools.get_current_path(__file__)+"/bullet/src/")
	myModule.add_export_path(tools.get_current_path(__file__))
	myModule.add_path(tools.get_current_path(__file__)+"/bullet/Extras/ConvexDecomposition")
	
	# lib BulletCollision
	myModule.add_src_file([
		'bullet/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp',
		'bullet/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btGhostObject.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btUnionFind.cpp',
		'bullet/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btShapeHull.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btCompoundShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConeShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btSphereShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btEmptyShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btCollisionShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConvexShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConcaveShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btBoxShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btBox2dShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btCylinderShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp',
		'bullet/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btDbvt.cpp',
		'bullet/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp',
		'bullet/src/BulletCollision/Gimpact/btGImpactBvh.cpp',
		'bullet/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp',
		'bullet/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp',
		'bullet/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp',
		'bullet/src/BulletCollision/Gimpact/btGImpactShape.cpp',
		'bullet/src/BulletCollision/Gimpact/gim_box_set.cpp',
		'bullet/src/BulletCollision/Gimpact/gim_contact.cpp',
		'bullet/src/BulletCollision/Gimpact/gim_memory.cpp',
		'bullet/src/BulletCollision/Gimpact/gim_tri_collision.cpp'])
	
	
	
	
	
	# lib BulletDynamics
	myModule.add_src_file([
		'bullet/src/BulletDynamics/Dynamics/btRigidBody.cpp',
		'bullet/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp',
		'bullet/src/BulletDynamics/Dynamics/Bullet-C-API.cpp',
		'bullet/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btGearConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp',
		'bullet/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp',
		'bullet/src/BulletDynamics/Vehicle/btWheelInfo.cpp',
		'bullet/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp',
		'bullet/src/BulletDynamics/Character/btKinematicCharacterController.cpp'])
	
	
	# lib BulletSoftBody
	myModule.add_src_file([
		'bullet/src/BulletSoftBody/btDefaultSoftBodySolver.cpp',
		'bullet/src/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp',
		'bullet/src/BulletSoftBody/btSoftBody.cpp',
		'bullet/src/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp',
		'bullet/src/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp',
		'bullet/src/BulletSoftBody/btSoftRigidDynamicsWorld.cpp',
		'bullet/src/BulletSoftBody/btSoftBodyHelpers.cpp',
		'bullet/src/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp'])
	
	# lib gimpactutils
	myModule.add_src_file([
		'bullet/Extras/GIMPACTUtils/btGImpactConvexDecompositionShape.cpp'])
	
	"""
	# lib convexdecomposition
	myModule.add_src_file([
		'bullet/Extras/ConvexDecomposition/concavity.cpp',
		'bullet/Extras/ConvexDecomposition/ConvexDecomposition.cpp',
		'bullet/Extras/ConvexDecomposition/vlookup.cpp',
		'bullet/Extras/ConvexDecomposition/bestfit.cpp',
		'bullet/Extras/ConvexDecomposition/Convexbuilder.cpp',
		'bullet/Extras/ConvexDecomposition/cd_hull.cpp',
		'bullet/Extras/ConvexDecomposition/raytri.cpp',
		'bullet/Extras/ConvexDecomposition/splitplane.cpp',
		'bullet/Extras/ConvexDecomposition/float_math.cpp',
		'bullet/Extras/ConvexDecomposition/planetri.cpp',
		'bullet/Extras/ConvexDecomposition/cd_wavefront.cpp',
		'bullet/Extras/ConvexDecomposition/bestfitobb.cpp',
		'bullet/Extras/ConvexDecomposition/meshvolume.cpp',
		'bullet/Extras/ConvexDecomposition/fitsphere.cpp'])
	"""
	
	"""
	# lib HACD
	myModule.add_src_file([
		'bullet/Extras/HACD/hacdGraph.cpp',
		'bullet/Extras/HACD/hacdHACD.cpp',
		'bullet/Extras/HACD/hacdICHull.cpp',
		'bullet/Extras/HACD/hacdManifoldMesh.cpp'])
	"""
	
	# add the currrent module at the 
	return myModule


