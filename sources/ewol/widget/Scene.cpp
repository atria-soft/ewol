/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/widget/Scene.h>
#include <math.h>
#include <ewol/renderer/openGL.h>
#include <etk/math/Matrix4.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPolyhedron.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <LinearMath/btTransformUtil.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletDynamicsCommon.h>

#undef __class__
#define __class__	"Scene"

#define  WALK_FLAG_FORWARD  (1<<0)
#define  WALK_FLAG_BACK     (1<<1)
#define  WALK_FLAG_LEFT     (1<<2)
#define  WALK_FLAG_RIGHT    (1<<3)
#define  WALK_FLAG_CAUTION  (1<<4)


class SceneDebugDrawer : public btIDebugDraw
{
	int m_debugMode;
	public:
		SceneDebugDrawer():m_debugMode(0) {};
		virtual ~SceneDebugDrawer() {};
		virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor)
		{
			EWOL_DEBUG("DebugDisplay : drawLine");
			/*
			glBegin(GL_LINES);
				glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
				glVertex3d(from.getX(), from.getY(), from.getZ());
				glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
				glVertex3d(to.getX(), to.getY(), to.getZ());
			glEnd();
			*/
		}
		virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
		{
			drawLine(from,to,color,color);
		}
		virtual void drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
		{
			EWOL_DEBUG("DebugDisplay : drawSphere");
			/*
			glColor4f (color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
			glPushMatrix ();
			glTranslatef (p.getX(), p.getY(), p.getZ());
			*/
			int lats = 5;
			int longs = 5;
			int i, j;
			for(i = 0; i <= lats; i++) {
				btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
				btScalar z0  = radius*sin(lat0);
				btScalar zr0 =  radius*cos(lat0);
				btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
				btScalar z1 = radius*sin(lat1);
				btScalar zr1 = radius*cos(lat1);
				/*
				glBegin(GL_QUAD_STRIP);
				for(j = 0; j <= longs; j++) {
					btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
					btScalar x = cos(lng);
					btScalar y = sin(lng);
					glNormal3f(x * zr0, y * zr0, z0);
					glVertex3f(x * zr0, y * zr0, z0);
					glNormal3f(x * zr1, y * zr1, z1);
					glVertex3f(x * zr1, y * zr1, z1);
				}
				glEnd();
				*/
			}
			//glPopMatrix();
		}
		
		virtual void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
		{
			EWOL_DEBUG("DebugDisplay : drawTriangle");
		//	if (m_debugMode > 0)
			{
				/*
				const btVector3	n=btCross(b-a,c-a).normalized();
				glBegin(GL_TRIANGLES);		
				glColor4f(color.getX(), color.getY(), color.getZ(),alpha);
				glNormal3d(n.getX(),n.getY(),n.getZ());
				glVertex3d(a.getX(),a.getY(),a.getZ());
				glVertex3d(b.getX(),b.getY(),b.getZ());
				glVertex3d(c.getX(),c.getY(),c.getZ());
				glEnd();
				*/
			}
		}
		
		
		virtual void drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
		{
			EWOL_DEBUG("DebugDisplay : drawContactPoint");
			/*
			btVector3 to=pointOnB+normalOnB*1;//distance;
			const btVector3&from = pointOnB;
			glColor4f(color.getX(), color.getY(), color.getZ(),1.f);
			//glColor4f(0,0,0,1.f);
			glBegin(GL_LINES);
			glVertex3d(from.getX(), from.getY(), from.getZ());
			glVertex3d(to.getX(), to.getY(), to.getZ());
			glEnd();
			*/
		}
		
		virtual void reportErrorWarning(const char* warningString)
		{
			EWOL_DEBUG("DebugDisplay : reportErrorWarning");
			printf("%s\n",warningString);
		}
		
		virtual void draw3dText(const btVector3& location,const char* textString)
		{
			EWOL_DEBUG("DebugDisplay : draw3dText");
			//glRasterPos3f(location.x(),  location.y(),  location.z());
			//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
		}
		
		virtual void setDebugMode(int debugMode)
		{
			m_debugMode = debugMode;
		}
		
		virtual int getDebugMode() const
		{
			return m_debugMode;
		}

};


SceneDebugDrawer completeDebugger;


///create 125 (5x5x5) dynamic object
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_Z 5

//maximum number of objects (and allow user to shoot additional boxes)
#define MAX_PROXIES (ARRAY_SIZE_X*ARRAY_SIZE_Y*ARRAY_SIZE_Z + 1024)

///scaling of the objects (0.1 = 20 centimeter boxes )
#define SCALING 1.
#define START_POS_X -5
#define START_POS_Y -5
#define START_POS_Z -3

widget::Scene::Scene(btDynamicsWorld* gameEngine) :
	m_dynamicsWorld(NULL),
	m_camera(vec3(-6,0,10), vec3(DEG_TO_RAD(0),0,0)),
	//m_gameEngine(gameEngine),
	m_isRunning(true),
	m_lastCallTime(-1),
	m_walk(0),
	m_debugMode(0),
	m_directDrawObject(NULL)
{
	// this permit to display direct element ...
	ewol::resource::Keep(m_directDrawObject);
	
	SetCanHaveFocus(true);
	PeriodicCallSet(true);
	m_zoom = 1.0/1000.0;
	m_ratioTime = 1.0f;
	
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	///use the default collision dispatcher.
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	
	m_broadphase = new btDbvtBroadphase();
	
	///the default constraint solver.
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;
	
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	m_dynamicsWorld->setDebugDrawer(&completeDebugger);
	m_dynamicsWorld->setGravity(btVector3(0,0,-10));
	
	
	// Create The ground
	btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(5.)));
	m_collisionShapes.PushBack(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,0,-5));
	btScalar mass(0.0);
	btVector3 localInertia(0,0,0);
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	if (mass != 0.0f) {
		groundShape->calculateLocalInertia(mass, localInertia);
	}
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	//add the body to the dynamics world
	m_dynamicsWorld->addRigidBody(body);
	
	
	{
		// Create a few dynamic rigidbodies
		// Re-using the same collision is better for memory usage and performance
		btBoxShape* colBoxShape = new btBoxShape(btVector3(SCALING*1,SCALING*1,SCALING*1));
		//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		m_collisionShapes.PushBack(colBoxShape);
		
		// Re-using the same collision is better for memory usage and performance
		btSphereShape* colSphereShape = new btSphereShape(btScalar(SCALING*1));
		//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		m_collisionShapes.PushBack(colSphereShape);
		
		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();
		btScalar mass(1.0f);
		btVector3 localInertia(0,0,0);
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		if (mass != 0.0f) {
			colBoxShape->calculateLocalInertia(mass, localInertia);
			colSphereShape->calculateLocalInertia(mass, localInertia);
		}
		float start_x = START_POS_X - ARRAY_SIZE_X/2;
		float start_y = START_POS_Y;
		float start_z = START_POS_Z - ARRAY_SIZE_Z/2;
		for (int kkk=0 ; kkk<ARRAY_SIZE_Y ; kkk++) {
			for (int iii=0 ; iii<ARRAY_SIZE_X ; iii++) {
				for(int jjj=0 ; jjj<ARRAY_SIZE_Z ; jjj++) {
					btQuaternion tmpAngle(DEG_TO_RAD(45), DEG_TO_RAD(45), 0);
					startTransform.setRotation(tmpAngle);
					startTransform.setOrigin(SCALING*btVector3(btScalar(20+2.2*iii + start_x),
					                                           btScalar(0+2.2*kkk + start_y),
					                                           btScalar(20+3.0*jjj + start_z)     ) );
					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
					btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
					if(jjj%2==0) {
						btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colBoxShape,localInertia);
					btRigidBody* body = new btRigidBody(rbInfo);
					m_dynamicsWorld->addRigidBody(body);
					} else {
						btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colSphereShape,localInertia);
						btRigidBody* body = new btRigidBody(rbInfo);
						m_dynamicsWorld->addRigidBody(body);
					}
				}
			}
		}
	}
}


widget::Scene::~Scene(void)
{
	ewol::resource::Release(m_directDrawObject);
	//cleanup in the reverse order of creation/initialization
	//remove the rigidbodies from the dynamics world and delete them
	for (int32_t iii=m_dynamicsWorld->getNumCollisionObjects()-1; iii>=0 ;iii--) {
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[iii];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int jjj=0;jjj<m_collisionShapes.Size();jjj++) {
		delete m_collisionShapes[jjj];
		m_collisionShapes[jjj]=NULL;
	}
	m_collisionShapes.Clear();
	delete m_dynamicsWorld;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfiguration;
}


void widget::Scene::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
	}
}


void widget::Scene::Pause(void)
{
	m_isRunning = false;
}


void widget::Scene::Resume(void)
{
	m_isRunning = true;
}


void widget::Scene::PauseToggle(void)
{
	if(true==m_isRunning) {
		m_isRunning=false;
	} else {
		m_isRunning=true;
	}
}


void widget::Scene::DrawSphere(btScalar radius, int lats, int longs, mat4& transformationMatrix, draw::Colorf& tmpColor)
{
	int i, j;
	etk::Vector<vec3> EwolVertices;
	for(i = 0; i <= lats; i++) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
		btScalar z0  = radius*sin(lat0);
		btScalar zr0 =  radius*cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
		btScalar z1 = radius*sin(lat1);
		btScalar zr1 = radius*cos(lat1);


		//glBegin(GL_QUAD_STRIP);
		for(j = 0; j < longs; j++) {
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);
			vec3 v1 = vec3(x * zr1, y * zr1, z1);
			vec3 v4 = vec3(x * zr0, y * zr0, z0);
		
			lng = 2 * SIMD_PI * (btScalar) (j) / longs;
			x = cos(lng);
			y = sin(lng);
			vec3 v2 = vec3(x * zr1, y * zr1, z1);
			vec3 v3 = vec3(x * zr0, y * zr0, z0);
			
			EwolVertices.PushBack(v1);
			EwolVertices.PushBack(v2);
			EwolVertices.PushBack(v3);
			
			EwolVertices.PushBack(v1);
			EwolVertices.PushBack(v3);
			EwolVertices.PushBack(v4);
		}
	}
	m_directDrawObject->Draw(EwolVertices, tmpColor, transformationMatrix);
}


inline void glDrawVector(const btVector3& v) { /*glVertex3d(v[0], v[1], v[2]);*/ }

void widget::Scene::DrawOpenGL(btScalar* mmm, 
                const btCollisionShape* shape,
                const btVector3& color,
                int32_t	debugMode,
                const btVector3& worldBoundsMin,
                const btVector3& worldBoundsMax)
{
	mat4 transformationMatrix(mmm);
	transformationMatrix.Transpose();
	/*
	transformationMatrix.Identity();
	transformationMatrix.m_mat[3] = mmm[12];
	transformationMatrix.m_mat[7] = mmm[13];
	transformationMatrix.m_mat[11] = mmm[14];
	*/
	//EWOL_DEBUG("Matrix : " << transformationMatrix);
	
	etk::Vector<vec3> EwolVertices;
	if (shape->getShapeType() == CUSTOM_CONVEX_SHAPE_TYPE) {
		EWOL_DEBUG("    Draw (1): CUSTOM_CONVEX_SHAPE_TYPE");
		btVector3 org(mmm[12], mmm[13], mmm[14]);
		btVector3 dx(mmm[0], mmm[1], mmm[2]);
		btVector3 dy(mmm[4], mmm[5], mmm[6]);
		const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
		btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
		dx *= halfExtent[0];
		dy *= halfExtent[1];
		/*
		glColor3f(1,1,1);
		glDisable(GL_LIGHTING);
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
		glDrawVector(org - dx - dy);
		glDrawVector(org - dx + dy);
		glDrawVector(org + dx + dy);
		glDrawVector(org + dx - dy);
		glEnd();
		*/
		return;
	} else if((shape->getShapeType() == BOX_SHAPE_PROXYTYPE) && (debugMode & btIDebugDraw::DBG_FastWireframe)) {
		EWOL_DEBUG("    Draw (2): BOX_SHAPE_PROXYTYPE");
		btVector3 org(mmm[12], mmm[13], mmm[14]);
		btVector3 dx(mmm[0], mmm[1], mmm[2]);
		btVector3 dy(mmm[4], mmm[5], mmm[6]);
		btVector3 dz(mmm[8], mmm[9], mmm[10]);
		const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
		btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
		dx *= halfExtent[0];
		dy *= halfExtent[1];
		dz *= halfExtent[2];
		/*
		glBegin(GL_LINE_LOOP);
		glDrawVector(org - dx - dy - dz);
		glDrawVector(org + dx - dy - dz);
		glDrawVector(org + dx + dy - dz);
		glDrawVector(org - dx + dy - dz);
		glDrawVector(org - dx + dy + dz);
		glDrawVector(org + dx + dy + dz);
		glDrawVector(org + dx - dy + dz);
		glDrawVector(org - dx - dy + dz);
		glEnd();
		glBegin(GL_LINES);
		glDrawVector(org + dx - dy - dz);
		glDrawVector(org + dx - dy + dz);
		glDrawVector(org + dx + dy - dz);
		glDrawVector(org + dx + dy + dz);
		glDrawVector(org - dx - dy - dz);
		glDrawVector(org - dx + dy - dz);
		glDrawVector(org - dx - dy + dz);
		glDrawVector(org - dx + dy + dz);
		glEnd();
		*/
		return;
	}
	/*
	glPushMatrix(); 
	glMultMatrixf(mmm);
	*/
	if (shape->getShapeType() == UNIFORM_SCALING_SHAPE_PROXYTYPE) {
		EWOL_DEBUG("    Draw (3): UNIFORM_SCALING_SHAPE_PROXYTYPE");
		const btUniformScalingShape* scalingShape = static_cast<const btUniformScalingShape*>(shape);
		const btConvexShape* convexShape = scalingShape->getChildShape();
		float scalingFactor = (float)scalingShape->getUniformScalingFactor();
		{
			btScalar tmpScaling[4][4]={{scalingFactor,0,0,0},
			{0,scalingFactor,0,0},
			{0,0,scalingFactor,0},
			{0,0,0,1}};
			DrawOpenGL((btScalar*)tmpScaling,
			           convexShape,
			           color,
			           debugMode,
			           worldBoundsMin,
			           worldBoundsMax);
		}
		//glPopMatrix();
		return;
	}
	if (shape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE) {
		EWOL_DEBUG("    Draw (4): COMPOUND_SHAPE_PROXYTYPE");
		const btCompoundShape* compoundShape = static_cast<const btCompoundShape*>(shape);
		for (int32_t iii=compoundShape->getNumChildShapes()-1;iii>=0;iii--) {
			btTransform childTrans = compoundShape->getChildTransform(iii);
			const btCollisionShape* colShape = compoundShape->getChildShape(iii);
			ATTRIBUTE_ALIGNED16(btScalar) childMat[16];
			childTrans.getOpenGLMatrix(childMat);
			DrawOpenGL(childMat,
			           colShape,
			           color,
			           debugMode,
			           worldBoundsMin,
			           worldBoundsMax);
		}
	} else {
		draw::Colorf tmpColor(color.x(),color.y(), color.z(), 0.5);
		bool useWireframeFallback = true;
		if (!(debugMode & btIDebugDraw::DBG_DrawWireframe)) {
			//EWOL_DEBUG("        Draw (6): !btIDebugDraw::DBG_DrawWireframe");
			int shapetype=shape->getShapeType();
			switch (shapetype) {
				case SPHERE_SHAPE_PROXYTYPE:
				{
					/** Bounding Sphere ==> model shape **/
					//EWOL_DEBUG("            Draw (101): SPHERE_SHAPE_PROXYTYPE");
					const btSphereShape* sphereShape = static_cast<const btSphereShape*>(shape);
					float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
					DrawSphere(radius, 10, 10, transformationMatrix, tmpColor);
					useWireframeFallback = false;
					break;
				}
				case BOX_SHAPE_PROXYTYPE:
				{
					/** Bounding box ==> model shape **/
					//EWOL_DEBUG("            Draw (102): BOX_SHAPE_PROXYTYPE");
					const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
					btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
					static int indices[36] = {
						0,1,2,
						3,2,1,
						4,0,6,
						6,0,2,
						5,1,4,
						4,1,0,
						7,3,1,
						7,1,5,
						5,4,7,
						7,4,6,
						7,2,3,
						7,6,2};
					vec3 vertices[8]={
						vec3(halfExtent[0],halfExtent[1],halfExtent[2]),
						vec3(-halfExtent[0],halfExtent[1],halfExtent[2]),
						vec3(halfExtent[0],-halfExtent[1],halfExtent[2]),
						vec3(-halfExtent[0],-halfExtent[1],halfExtent[2]),
						vec3(halfExtent[0],halfExtent[1],-halfExtent[2]),
						vec3(-halfExtent[0],halfExtent[1],-halfExtent[2]),
						vec3(halfExtent[0],-halfExtent[1],-halfExtent[2]),
						vec3(-halfExtent[0],-halfExtent[1],-halfExtent[2])};
					EwolVertices.Clear();
					for (int32_t iii=0 ; iii<36 ; iii+=3) {
						// normal calculation :
						//btVector3 normal = (vertices[indices[iii+2]]-vertices[indices[iii]]).cross(vertices[indices[iii+1]]-vertices[indices[iii]]);
						//normal.normalize ();
						EwolVertices.PushBack(vertices[indices[iii]]);
						EwolVertices.PushBack(vertices[indices[iii+1]]);
						EwolVertices.PushBack(vertices[indices[iii+2]]);
					}
					m_directDrawObject->Draw(EwolVertices, tmpColor, transformationMatrix);
					useWireframeFallback = false;
					break;
				}
				case STATIC_PLANE_PROXYTYPE:
				{
					EWOL_DEBUG("            Draw (103): STATIC_PLANE_PROXYTYPE");
					EwolVertices.Clear();
					const btStaticPlaneShape* staticPlaneShape = static_cast<const btStaticPlaneShape*>(shape);
					btScalar planeConst = staticPlaneShape->getPlaneConstant();
					const btVector3& planeNormal = staticPlaneShape->getPlaneNormal();
					btVector3 planeOrigin = planeNormal * planeConst;
					btVector3 vec0,vec1;
					btPlaneSpace1(planeNormal,vec0,vec1);
					btScalar vecLen = 100.f;
					btVector3 pt0 = planeOrigin + vec0*vecLen;
					btVector3 pt1 = planeOrigin - vec0*vecLen;
					btVector3 pt2 = planeOrigin + vec1*vecLen;
					btVector3 pt3 = planeOrigin - vec1*vecLen;
					EwolVertices.PushBack(vec3(pt0.getX(),pt0.getY(),pt0.getZ()));
					EwolVertices.PushBack(vec3(pt1.getX(),pt1.getY(),pt1.getZ()));
					EwolVertices.PushBack(vec3(pt2.getX(),pt2.getY(),pt2.getZ()));
					EwolVertices.PushBack(vec3(pt3.getX(),pt3.getY(),pt3.getZ()));
					draw::Colorf tmpColor(color.x(),color.y(), color.z(), 0.5);
					m_directDrawObject->DrawLine(EwolVertices, tmpColor, transformationMatrix);
					break;
				}
				case MULTI_SPHERE_SHAPE_PROXYTYPE:
				{
					EWOL_DEBUG("            Draw (104): MULTI_SPHERE_SHAPE_PROXYTYPE");
					const btMultiSphereShape* multiSphereShape = static_cast<const btMultiSphereShape*>(shape);
					btTransform childTransform;
					childTransform.setIdentity();
					
					for (int32_t iii = multiSphereShape->getSphereCount()-1; iii>=0;iii--) {
						btSphereShape sc(multiSphereShape->getSphereRadius(iii));
						childTransform.setOrigin(multiSphereShape->getSpherePosition(iii));
						ATTRIBUTE_ALIGNED16(btScalar) childMat[16];
						childTransform.getOpenGLMatrix(childMat);
						DrawOpenGL(childMat,
						           &sc,
						           color,
						           debugMode,
						           worldBoundsMin,
						           worldBoundsMax);
					}
					break;
				}
				default:
				{
					EWOL_DEBUG("            Draw (105): default");
					if (shape->isConvex()) {
						EWOL_DEBUG("                shape->isConvex()");
						const btConvexPolyhedron* poly = shape->isPolyhedral() ? ((btPolyhedralConvexShape*) shape)->getConvexPolyhedron() : 0;
						if (NULL!=poly) {
							//glBegin(GL_TRIANGLES);
							for (int32_t iii=0 ; iii<poly->m_faces.size() ; iii++) {
								btVector3 centroid(0,0,0);
								int numVerts = poly->m_faces[iii].m_indices.size();
								if (numVerts>2) {
									btVector3 v1 = poly->m_vertices[poly->m_faces[iii].m_indices[0]];
									for (int32_t vvv=0;vvv<poly->m_faces[iii].m_indices.size()-2;vvv++) {
										btVector3 v2 = poly->m_vertices[poly->m_faces[iii].m_indices[vvv+1]];
										btVector3 v3 = poly->m_vertices[poly->m_faces[iii].m_indices[vvv+2]];
										btVector3 normal = (v3-v1).cross(v2-v1);
										normal.normalize ();
										/*
										glNormal3f(normal.getX(),normal.getY(),normal.getZ());
										glVertex3f (v1.x(), v1.y(), v1.z());
										glVertex3f (v2.x(), v2.y(), v2.z());
										glVertex3f (v3.x(), v3.y(), v3.z());
										*/
									}
								}
							}
							//glEnd();
						} else {
							// TODO : Set it back ...
							/*
							ShapeCache*	sc=cache((btConvexShape*)shape);
							//glutSolidCube(1.0);
							btShapeHull* hull = &sc->m_shapehull;
							if (hull->numTriangles () > 0) {
								int index = 0;
								const unsigned int* idx = hull->getIndexPointer();
								const btVector3* vtx = hull->getVertexPointer();
								glBegin (GL_TRIANGLES);
								for (int i = 0; i < hull->numTriangles (); i++) {
									int i1 = index++;
									int i2 = index++;
									int i3 = index++;
									btAssert(i1 < hull->numIndices () &&
										i2 < hull->numIndices () &&
										i3 < hull->numIndices ());
									int index1 = idx[i1];
									int index2 = idx[i2];
									int index3 = idx[i3];
									btAssert(index1 < hull->numVertices () &&
										index2 < hull->numVertices () &&
										index3 < hull->numVertices ());
									btVector3 v1 = vtx[index1];
									btVector3 v2 = vtx[index2];
									btVector3 v3 = vtx[index3];
									btVector3 normal = (v3-v1).cross(v2-v1);
									normal.normalize ();
									glNormal3f(normal.getX(),normal.getY(),normal.getZ());
									glVertex3f (v1.x(), v1.y(), v1.z());
									glVertex3f (v2.x(), v2.y(), v2.z());
									glVertex3f (v3.x(), v3.y(), v3.z());
								}
								glEnd ();
							}
							*/
						}
					} else {
						EWOL_DEBUG("                !!! shape->isConvex() !!!");
					}
				}
			}
		}
		/*
		glNormal3f(0,1,0);
		/// for polyhedral shapes
		if (debugMode==btIDebugDraw::DBG_DrawFeaturesText && (shape->isPolyhedral())) {
			btPolyhedralConvexShape* polyshape = (btPolyhedralConvexShape*) shape;
			glColor3f(1.f, 1.f, 1.f);
			for (int32_t iii=0 ; iii<polyshape->getNumVertices() ; iii++) {
				btVector3 vtx;
				polyshape->getVertex(iii, vtx);
			}
			for (int32_t iii=0 ; iii<polyshape->getNumPlanes() ; iii++) {
				btVector3 normal;
				btVector3 vtx;
				polyshape->getPlane(normal,vtx,iii);
			}
		}
		*/
	}
}

void DrawShadow(btScalar* mmm, 
                const btVector3& extrusion,
                const btCollisionShape* shape,
                const btVector3& worldBoundsMin,
                const btVector3& worldBoundsMax)
{
	//glPushMatrix(); 
	//glMultMatrixf(mmm);
	if(shape->getShapeType() == UNIFORM_SCALING_SHAPE_PROXYTYPE) {
		const btUniformScalingShape* scalingShape = static_cast<const btUniformScalingShape*>(shape);
		const btConvexShape* convexShape = scalingShape->getChildShape();
		float	scalingFactor = (float)scalingShape->getUniformScalingFactor();
		btScalar tmpScaling[4][4]={	{scalingFactor,0,0,0},
		{0,scalingFactor,0,0},
		{0,0,scalingFactor,0},
		{0,0,0,1}};
		DrawShadow((btScalar*)tmpScaling,
		           extrusion,
		           convexShape,
		           worldBoundsMin,
		           worldBoundsMax);
		//glPopMatrix();
		return;
	} else if(shape->getShapeType()==COMPOUND_SHAPE_PROXYTYPE) {
		const btCompoundShape* compoundShape = static_cast<const btCompoundShape*>(shape);
		for (int i=compoundShape->getNumChildShapes()-1;i>=0;i--) {
			btTransform childTrans = compoundShape->getChildTransform(i);
			const btCollisionShape* colShape = compoundShape->getChildShape(i);
			ATTRIBUTE_ALIGNED16(btScalar) childMat[16];
			childTrans.getOpenGLMatrix(childMat);
			DrawShadow(childMat,
			           extrusion*childTrans.getBasis(),
			           colShape,
			           worldBoundsMin,
			           worldBoundsMax);
		}
	} else {
		if (shape->isConvex()) {
			// TODO : Set it back ...
			/*
			ShapeCache*	sc=cache((btConvexShape*)shape);
			btShapeHull* hull =&sc->m_shapehull;
			glBegin(GL_QUADS);
			for(int32_t iii=0 ; iii<sc->m_edges.size() ; ++ii) {
				const btScalar d=btDot(sc->m_edges[iii].n[0],extrusion);
				if((d*btDot(sc->m_edges[iii].n[1],extrusion))<0) {
					const int			q=	d<0?1:0;
					const btVector3&	a=	hull->getVertexPointer()[sc->m_edges[iii].v[q]];
					const btVector3&	b=	hull->getVertexPointer()[sc->m_edges[iii].v[1-q]];
					glVertex3f(a[0],a[1],a[2]);
					glVertex3f(b[0],b[1],b[2]);
					glVertex3f(b[0]+extrusion[0],b[1]+extrusion[1],b[2]+extrusion[2]);
					glVertex3f(a[0]+extrusion[0],a[1]+extrusion[1],a[2]+extrusion[2]);
				}
			}
			glEnd();
			*/
		}
	}
	if (shape->isConcave()) {
		// TODO : Set it back ...
		/*
		btConcaveShape* concaveMesh = (btConcaveShape*) shape;
		GlDrawcallback drawCallback;
		drawCallback.m_wireframe = false;
		concaveMesh->processAllTriangles(&drawCallback,worldBoundsMin,worldBoundsMax);
		*/
	}
	//glPopMatrix();

}


void widget::Scene::renderscene(int pass)
{
	//glPushMatrix();
	//EWOL_DEBUG("Render Scene pass=" << pass);
	//mat4& myMatrix = ewol::openGL::GetMatrix();
	//myMatrix = m_camera.GetMatrix() * myMatrix;
	//myMatrix.Transpose();
	//glLoadMatrixf(myMatrix.m_mat);
	btScalar mmm[16];
	btMatrix3x3 rot;
	rot.setIdentity();
	const int32_t numObjects=m_dynamicsWorld->getNumCollisionObjects();
	btVector3 wireColor(1,0,0);
	for(int32_t iii=0;iii<numObjects;iii++)
	{
		//EWOL_DEBUG("    obj id=" << iii << "/" << numObjects );
		
		btCollisionObject* colObj=m_dynamicsWorld->getCollisionObjectArray()[iii];
		btRigidBody* body=btRigidBody::upcast(colObj);
		if(    NULL != body
		    && body->getMotionState() ) {
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(mmm);
			rot=myMotionState->m_graphicsWorldTrans.getBasis(); // ==> for the sun ...
		} else {
			colObj->getWorldTransform().getOpenGLMatrix(mmm);
			rot=colObj->getWorldTransform().getBasis(); // ==> for the sun ...
		}
		btVector3 wireColor(1.f,1.0f,0.5f); //wants deactivation
		if(iii&1) {
			wireColor=btVector3(0.f,0.0f,1.f);
		}
		///color differently for active, sleeping, wantsdeactivation states
		if (colObj->getActivationState() == 1) {
			//active
			if (iii & 1) {
				wireColor += btVector3 (1.f,0.f,0.f);
			} else {
				wireColor += btVector3 (.5f,0.f,0.f);
			}
		}
		if(colObj->getActivationState()==2) {
			//ISLAND_SLEEPING
			if(iii&1) {
				wireColor += btVector3 (0.f,1.f, 0.f);
			} else {
				wireColor += btVector3 (0.f,0.5f,0.f);
			}
		}

		btVector3 aabbMin,aabbMax;
		m_dynamicsWorld->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
		
		aabbMin-=btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
		aabbMax+=btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);

		btVector3 m_sundirection(btVector3(1,-2,1)*1000);
		if (!(m_debugMode & btIDebugDraw::DBG_DrawWireframe))
		{
			switch(pass)
			{
				case 0:
					DrawOpenGL(mmm,
					           colObj->getCollisionShape(),
					           wireColor,
					           m_debugMode,
					           aabbMin,
					           aabbMax);
					break;
				case 1:
					DrawShadow(mmm,
					           m_sundirection*rot,
					           colObj->getCollisionShape(),
					           aabbMin,
					           aabbMax);
					break;
				case 2:
					DrawOpenGL(mmm,
					           colObj->getCollisionShape(),
					           wireColor*btScalar(0.3),
					           0,
					           aabbMin,
					           aabbMax);
					break;
			}
		}
	}
	//glPopMatrix();
}


void widget::Scene::OnDraw(ewol::DrawProperty& displayProp)
{
	if (m_dynamicsWorld) {
		/*
		GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
		GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
		GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
		//	light_position is NOT default value
		GLfloat light_position0[] = { btScalar(1.0), btScalar(10.0), btScalar(1.0), btScalar(0.0 )};
		GLfloat light_position1[] = { btScalar(-1.0), btScalar(-10.0), btScalar(-1.0), btScalar(0.0) };
		
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
		
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		
		
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		*/
		//updateCamera();
		if(false/*m_enableshadows*/) {
			/*
			glClear(GL_STENCIL_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			renderscene(0);
			
			glDisable(GL_LIGHTING);
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_STENCIL_TEST);
			glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
			glFrontFace(GL_CCW);
			glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
			renderscene(1);
			glFrontFace(GL_CW);
			glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
			renderscene(1);
			glFrontFace(GL_CCW);
			
			glPolygonMode(GL_FRONT,GL_FILL);
			glPolygonMode(GL_BACK,GL_FILL);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_LIGHTING);
			glDepthMask(GL_TRUE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			
			glDepthFunc(GL_LEQUAL);
			glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFFL );
			glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
			glDisable(GL_LIGHTING);
			renderscene(2);
			glEnable(GL_LIGHTING);
			glDepthFunc(GL_LESS);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_CULL_FACE);
			*/
		} else {
			//glDisable(GL_CULL_FACE);
			renderscene(0);
		}
		/*
		int32_t	xOffset = 10;
		int32_t yStart = 20;
		int32_t yIncr = 20;
		glDisable(GL_LIGHTING);
		
		glColor3f(0, 0, 0);
		if ((m_debugMode & btIDebugDraw::DBG_NoHelpText)==0) {
			setOrthographicProjection();
			showProfileInfo(xOffset,yStart,yIncr);
			resetPerspectiveProjection();
		}
		glDisable(GL_LIGHTING);
		*/
		//updateCamera();
		
		// only for the bebug :
		m_dynamicsWorld->debugDrawWorld();
	}
}

#define  WALK_FLAG_FORWARD  (1<<0)
#define  WALK_FLAG_BACK     (1<<1)
#define  WALK_FLAG_LEFT     (1<<2)
#define  WALK_FLAG_RIGHT    (1<<3)
#define  WALK_FLAG_CAUTION  (1<<4)

static const float l_walkRatio = 6.666f;
static const float l_walkLateralRatio = 3.333f;

void widget::Scene::PeriodicCall(int64_t localTime)
{
	double curentTime=(double)localTime/1000000.0;
	// small hack to change speed ...
	if (m_ratioTime != 1) {
		double ellapseTime = curentTime - m_lastCallTime;
		ellapseTime *= m_ratioTime;
		m_lastCallTime = curentTime - ellapseTime;
	}
	// First time : 
	if (-1 == m_lastCallTime) {
		m_lastCallTime = curentTime;
	}
	// check if the processing is availlable
	if (false == m_isRunning) {
		m_lastCallTime = curentTime;
		MarkToRedraw();
		return;
	}
	// cut the processing in small slot of time to prevent error in the real-time Display (Android call us between 30 to 60 fps)
	float deltaTime = (float) (curentTime - m_lastCallTime);
	//EWOL_DEBUG("Time: m_lastCallTime=" << m_lastCallTime << " deltaTime=" << deltaTime);
	
	///step the simulation
	if (m_dynamicsWorld)
	{
		m_dynamicsWorld->stepSimulation(deltaTime);
		//optional but useful: debug drawing
		m_dynamicsWorld->debugDrawWorld();
	}
	m_lastCallTime = curentTime;
	MarkToRedraw();
	if (m_walk!=0) {
		if(    (m_walk&WALK_FLAG_FORWARD)!=0
		    && (m_walk&WALK_FLAG_BACK)!=0) {
			// request back and forward in the same time ... this is really bad ....
		} else if ( (m_walk&WALK_FLAG_FORWARD)!=0) {
			vec3 angles = m_camera.GetAngle();
			angles.x = cosf(angles.z);
			angles.y = -sinf(angles.z);
			angles.z = 0;
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			vec3 pos = m_camera.GetPosition();
			// walk is 6 km/h
			pos += angles*l_walkRatio*deltaTime;
			m_camera.SetPosition(pos);
		} else if ( (m_walk&WALK_FLAG_BACK)!=0) {
			vec3 angles = m_camera.GetAngle();
			angles.x = -cosf(angles.z);
			angles.y = sinf(angles.z);
			angles.z = 0;
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			vec3 pos = m_camera.GetPosition();
			// walk is 6 km/h
			pos += angles*l_walkRatio*deltaTime;
			m_camera.SetPosition(pos);
		}
		
		if(    (m_walk&WALK_FLAG_LEFT)!=0
		    && (m_walk&WALK_FLAG_RIGHT)!=0) {
			// request left and right in the same time ... this is really bad ....
		} else if ( (m_walk&WALK_FLAG_LEFT)!=0) {
			vec3 angles = m_camera.GetAngle();
			angles.x = cosf(angles.z-M_PI/2.0);
			angles.y = -sinf(angles.z-M_PI/2.0);
			angles.z = 0;
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			vec3 pos = m_camera.GetPosition();
			// lateral walk is 4 km/h
			pos += angles*l_walkLateralRatio*deltaTime;
			m_camera.SetPosition(pos);
		} else if ( (m_walk&WALK_FLAG_RIGHT)!=0) {
			vec3 angles = m_camera.GetAngle();
			angles.x = -cosf(angles.z-M_PI/2.0);
			angles.y = sinf(angles.z-M_PI/2.0);
			angles.z = 0;
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			vec3 pos = m_camera.GetPosition();
			// lateral walk is 4 km/h
			pos += angles*l_walkLateralRatio*deltaTime;
			m_camera.SetPosition(pos);
		}
	}
}


void widget::Scene::GenDraw(ewol::DrawProperty displayProp)
{
	ewol::openGL::Push();
	// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
	glViewport( m_origin.x,
	            m_origin.y,
	            m_size.x,
	            m_size.y);
	float ratio = m_size.x / m_size.y;
	//EWOL_INFO("ratio : " << ratio);
	mat4 tmpProjection = etk::matPerspective( M_PI/3.0, ratio, 1, 4000);
	ewol::openGL::SetCameraMatrix(m_camera.GetMatrix());
	//mat4 tmpMat = tmpProjection * m_camera.GetMatrix();
	// set internal matrix system :
	//ewol::openGL::SetMatrix(tmpMat);
	ewol::openGL::SetMatrix(tmpProjection);
	
	// Call the widget drawing methode
	displayProp.m_origin = m_origin;
	displayProp.m_size = m_size;
	// Call the widget drawing methode
	OnDraw(displayProp);
	
	ewol::openGL::Pop();
}


vec2 widget::Scene::RelativePosition(vec2  pos)
{
	// Remove origin of the widget
	pos.x -= m_origin.x;
	pos.y -= m_origin.y;
	// move the position at the center (openGl system
	pos.x -= m_size.x/2;
	pos.y -= m_size.y/2;
	// scale the position with the ratio display of the screen
	float ratio = m_size.x / m_size.y;
	if (ratio >= 1.0) {
		pos.x /= m_size.x;
		pos.x *= ratio;
		pos.y /= m_size.y;
	} else {
		ratio = 1.0/ratio;
		pos.x /= m_size.x;
		pos.y /= m_size.y;
		pos.y *= ratio;
	}
	// integrate zoom
	pos.x /= m_zoom;
	pos.y /= m_zoom;
	// all the position are half the size due to the fact -1 --> 1
	pos.x *= 2;
	pos.y *= 2;
	
	return pos;
};


bool widget::Scene::OnEventInput(ewol::keyEvent::type_te type, int32_t IdInput, ewol::keyEvent::status_te statusEvent, vec2 pos)
{
	//EWOL_DEBUG("type : " << type << " IdInput=" << IdInput << " " << "status=" << statusEvent << " RelPos=" << relativePos);
	
	if (type == ewol::keyEvent::typeMouse) {
		if (0 != IdInput) {
			KeepFocus();
			GrabCursor();
			SetCursor(ewol::cursorNone);
		}
		if (true == GetGrabStatus() ) {
			if (ewol::keyEvent::statusMove == statusEvent) {
				pos *= M_PI/(360.0f*6);
				vec3 oldAngles = m_camera.GetAngle();
				oldAngles.z += pos.x;
				oldAngles.y += pos.y;
				m_camera.SetAngle(oldAngles);
			}
		}
	} else if (type == ewol::keyEvent::typeFinger) {
		KeepFocus();
	}
	// note : we did not parse the oether media ...

	return false;
}


bool widget::Scene::OnEventKb(ewol::keyEvent::status_te statusEvent, uniChar_t unicodeData)
{
	
	EWOL_DEBUG("KB EVENT : \"" << unicodeData << "\"" << "type=" << statusEvent);
	
	// escape case :
	if(unicodeData == 27) {
		if (statusEvent == ewol::keyEvent::statusDown) {
			UnGrabCursor();
			SetCursor(ewol::cursorArrow);
		}
		return false;
	}
	if (false == GetGrabStatus()) {
		GrabCursor();
		SetCursor(ewol::cursorNone);
	}
	if(    unicodeData == 'z'
	    || unicodeData == 'Z') {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_FORWARD;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_FORWARD) != 0) {
				m_walk -= WALK_FLAG_FORWARD;
			}
		}
	}
	if(    unicodeData == 's'
	    || unicodeData == 'S') {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_BACK;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_BACK) != 0) {
				m_walk -= WALK_FLAG_BACK;
			}
		}
	}
	if(    unicodeData == 'q'
	    || unicodeData == 'Q') {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_LEFT;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_LEFT) != 0) {
				m_walk -= WALK_FLAG_LEFT;
			}
		}
	}
	if(    unicodeData == 'd'
	    || unicodeData == 'D') {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_RIGHT;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_RIGHT) != 0) {
				m_walk -= WALK_FLAG_RIGHT;
			}
		}
	}
	EWOL_DEBUG("m_walk=" << m_walk);
	return false;
}


bool widget::Scene::OnEventKbMove(ewol::keyEvent::status_te statusEvent, ewol::keyEvent::keyboard_te specialKey)
{
	if (specialKey == ewol::keyEvent::keyboardUp) {
		EWOL_DEBUG("test ..." << specialKey << "  " << statusEvent);
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_FORWARD;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_FORWARD) != 0) {
				m_walk -= WALK_FLAG_FORWARD;
			}
		}
	}
	if (specialKey == ewol::keyEvent::keyboardDown) {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_BACK;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_BACK) != 0) {
				m_walk -= WALK_FLAG_BACK;
			}
		}
	}
	if (specialKey == ewol::keyEvent::keyboardLeft) {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_LEFT;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_LEFT) != 0) {
				m_walk -= WALK_FLAG_LEFT;
			}
		}
	}
	if (specialKey == ewol::keyEvent::keyboardRight) {
		if (statusEvent == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_RIGHT;
		}
		if (statusEvent == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_RIGHT) != 0) {
				m_walk -= WALK_FLAG_RIGHT;
			}
		}
	}
	EWOL_DEBUG("m_walk=" << m_walk);
	return false;
}


void widget::Scene::OnGetFocus(void)
{
	//GrabCursor();
}

void widget::Scene::OnLostFocus(void)
{
	UnGrabCursor();
}


