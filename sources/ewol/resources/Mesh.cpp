/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/resources/Mesh.h>
#include <ewol/resources/ResourceManager.h>
#include <etk/os/FSNode.h>

#undef __class__
#define __class__	"resource::Mesh"

ewol::resource::Mesh::Mesh(const std::string& _fileName, const std::string& _shaderName) :
  ewol::resource::Resource(_fileName),
  m_normalMode(normalModeNone),
  m_checkNormal(false),
  m_pointerShape(NULL),
  m_functionFreeShape(NULL) {
	addObjectType("ewol::Mesh");
	EWOL_VERBOSE("Load a new mesh : '" << _fileName << "'");
	// get the shader resource :
	m_GLPosition = 0;
	
	m_light.setDirection(vec3(0,-cos(M_PI/4),0));
	m_light.setHalfPlane(vec3(1,0,0));
	m_light.setAmbientColor(vec4(1,1,1,1));
	m_light.setDiffuseColor(vec4(1.0,1.0,1.0,1));
	m_light.setSpecularColor(vec4(0.0,0.0,0.0,1));
	
	//EWOL_DEBUG(m_name << "  " << m_light);
	m_GLprogram = ewol::Program::keep(_shaderName);
	if (NULL != m_GLprogram ) {
		m_GLPosition = m_GLprogram->getAttribute("EW_coord3d");
		m_GLtexture = m_GLprogram->getAttribute("EW_texture2d");
		m_GLNormal = m_GLprogram->getAttribute("EW_normal");
		m_GLMatrix = m_GLprogram->getUniform("EW_MatrixTransformation");
		m_GLMatrixPosition = m_GLprogram->getUniform("EW_MatrixPosition");
		// Link material and Lights
		m_GLMaterial.link(m_GLprogram, "EW_material");
		m_light.link(m_GLprogram, "EW_directionalLight");
	}
	// this is the properties of the buffer requested : "r"/"w" + "-" + buffer type "f"=flaot "i"=integer
	m_verticesVBO = ewol::VirtualBufferObject::keep(4);
	
	// load the curent file :
	std::string tmpName = to_lower(_fileName);
	// select the corect loader :
	if (end_with(tmpName, ".obj") == true) {
		if (loadOBJ(_fileName) == false) {
			EWOL_ERROR("Error To load OBJ file " << tmpName );
			return;
		}
	} else if (end_with(tmpName, ".emf") ) {
		if (loadEMF(_fileName) == false) {
			EWOL_ERROR("Error To load EMF file " << tmpName );
			return;
		}
		//EWOL_CRITICAL("Load a new mesh : '" << _fileName << "' (end)");
	} else {
		// nothing to do  == > reqiest an enmpty mesh ==> user manage it ...
	}
}

ewol::resource::Mesh::~Mesh(void) {
	// remove dynamics dependencies :
	ewol::resource::Program::release(m_GLprogram);
	ewol::resource::VirtualBufferObject::release(m_verticesVBO);
	if (m_functionFreeShape!=NULL) {
		m_functionFreeShape(m_pointerShape);
		m_pointerShape = NULL;
	}
}

//#define DISPLAY_NB_VERTEX_DISPLAYED

void ewol::resource::Mesh::draw(mat4& _positionMatrix,
                                bool _enableDepthTest,
                                bool _enableDepthUpdate) {
	if (m_GLprogram == NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	//EWOL_DEBUG(m_name << "  " << m_light);
	if (_enableDepthTest == true) {
		ewol::openGL::enable(ewol::openGL::FLAG_DEPTH_TEST);
		if (false == _enableDepthUpdate) {
			glDepthMask(GL_FALSE);
		}
	} else {
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	//EWOL_DEBUG("    display " << m_coord.size() << " elements" );
	m_GLprogram->use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::getMatrix();
	mat4 camMatrix = ewol::openGL::getCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->uniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	m_GLprogram->uniformMatrix4fv(m_GLMatrixPosition, 1, _positionMatrix.m_mat);
	// position :
	m_GLprogram->sendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->sendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->sendAttributePointer(m_GLNormal, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
	// draw lights :
	m_light.draw(m_GLprogram);
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
	int32_t nbElementDrawTheoric = 0;
	int32_t nbElementDraw = 0;
	#endif
	for (int32_t kkk=0; kkk<m_listFaces.size(); kkk++) {
		if (false == m_materials.exist(m_listFaces.getKey(kkk))) {
			EWOL_WARNING("missing materials : '" << m_listFaces.getKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.getKey(kkk)]->draw(m_GLprogram, m_GLMaterial);
		if (m_checkNormal == false) {
			ewol::openGL::drawElements(GL_TRIANGLES, m_listFaces.getValue(kkk).m_index);
			#ifdef DISPLAY_NB_VERTEX_DISPLAYED
				nbElementDraw += m_listFaces.getValue(kkk).m_index.size();
				nbElementDrawTheoric += m_listFaces.getValue(kkk).m_index.size();
			#endif
		} else {
			mat4 mattttt = (projMatrix * camMatrix) * _positionMatrix;
			mattttt.m_mat[3] = 0;
			mattttt.m_mat[7] = 0;
			mattttt.m_mat[11] = 0;
			//vec3 cameraNormal = vec3(-mattttt.m_mat[2], -mattttt.m_mat[6], -mattttt.m_mat[10]);
			vec3 cameraNormal = vec3(0,0,-1);
			cameraNormal.normalized();
			// remove face that is notin the view ...
			std::vector<uint32_t> tmpIndexResult;
			std::vector<ewol::Face>& tmppFaces = m_listFaces.getValue(kkk).m_faces;
			//std::vector<uint32_t>& tmppIndex = m_listFaces.getValue(kkk).m_index;
			if (normalModeFace == m_normalMode) {
				for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
					if(btDot(mattttt * m_listFacesNormal[tmppFaces[iii].m_normal[0]], cameraNormal) >= 0.0f) {
						tmpIndexResult.push_back(iii*3);
						tmpIndexResult.push_back(iii*3+1);
						tmpIndexResult.push_back(iii*3+2);
					}
				}
			} else {
				for(size_t iii=0; iii<tmppFaces.size() ; ++iii) {
					if(    (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[0]], cameraNormal) >= -0.2f)
					    || (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[1]], cameraNormal) >= -0.2f)
					    || (btDot(mattttt * m_listVertexNormal[tmppFaces[iii].m_normal[2]], cameraNormal) >= -0.2f) ) {
						tmpIndexResult.push_back(iii*3);
						tmpIndexResult.push_back(iii*3+1);
						tmpIndexResult.push_back(iii*3+2);
					}
				}
			}
			ewol::openGL::drawElements(GL_TRIANGLES, tmpIndexResult);
			#ifdef DISPLAY_NB_VERTEX_DISPLAYED
				nbElementDraw += tmpIndexResult.size();
				nbElementDrawTheoric += m_listFaces.getValue(kkk).m_index.size();
			#endif
		}
	}
	#ifdef DISPLAY_NB_VERTEX_DISPLAYED
		EWOL_DEBUG(((float)nbElementDraw/(float)nbElementDrawTheoric*100.0f) << "% Request draw : " << m_listFaces.size() << ":" << nbElementDraw << "/" << nbElementDrawTheoric << " elements [" << m_name << "]");
	#endif
	m_GLprogram->unUse();
	if (_enableDepthTest == true){
		if (false == _enableDepthUpdate) {
			glDepthMask(GL_TRUE);
		}
		ewol::openGL::disable(ewol::openGL::FLAG_DEPTH_TEST);
	}
	// TODO : UNDERSTAND why ... it is needed
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

// normal calculation of the normal face is really easy :
void ewol::resource::Mesh::calculateNormaleFace(void) {
	m_listFacesNormal.clear();
	if (m_normalMode != ewol::Mesh::normalModeFace) {
		std::vector<Face>& tmpFaceList = m_listFaces.getValue(0).m_faces;
		for(size_t iii=0 ; iii<tmpFaceList.size() ; iii++) {
			// for all case, We use only the 3 vertex for quad element, in theory 3D modeler export element in triangle if it is not a real plane.
			vec3 normal = btCross(m_listVertex[tmpFaceList[iii].m_vertex[0]]-m_listVertex[tmpFaceList[iii].m_vertex[1]],
			                      m_listVertex[tmpFaceList[iii].m_vertex[1]]-m_listVertex[tmpFaceList[iii].m_vertex[2]]);
			m_listFacesNormal.push_back(normal.normalized());
		}
		m_normalMode = ewol::Mesh::normalModeFace;
	}
}

void ewol::resource::Mesh::calculateNormaleEdge(void) {
	m_listVertexNormal.clear();
	if (m_normalMode != ewol::Mesh::normalModeVertex) {
		for(size_t iii=0 ; iii<m_listVertex.size() ; iii++) {
			std::vector<Face>& tmpFaceList = m_listFaces.getValue(0).m_faces;
			vec3 normal(0,0,0);
			// add the vertex from all the element in the list for face when the element in the face ...
			for(size_t jjj=0 ; jjj<tmpFaceList.size() ; jjj++) {
				m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES_NORMAL, normal);
				if(    tmpFaceList[jjj].m_vertex[0] == (int32_t)iii
				    || tmpFaceList[jjj].m_vertex[1] == (int32_t)iii
				    || tmpFaceList[jjj].m_vertex[2] == (int32_t)iii) {
					normal += m_listFacesNormal[jjj];
				}
			}
			if (normal == vec3(0,0,0)) {
				m_listVertexNormal.push_back(vec3(1,1,1));
			} else {
				m_listVertexNormal.push_back(normal.normalized());
			}
		}
		m_normalMode = ewol::Mesh::normalModeVertex;
	}
}

// for debugging ...
//#define PRINT_HALF (1)
//#define TRY_MINIMAL_VBO

void ewol::resource::Mesh::generateVBO(void) {
	// calculate the normal of all faces if needed
	if (m_normalMode == ewol::Mesh::normalModeNone) {
		// when no normal detected  == > auto generate Face normal ....
		calculateNormaleFace();
	}
	// generate element in 2 pass : 
	//    - create new index dependeng a vertex is a unique componenet of position, texture, normal
	//    - the index list generation (can be dynamic ... (TODO later)
	for (int32_t kkk=0; kkk<m_listFaces.size(); kkk++) {
		// clean faces indexes :
		m_listFaces.getValue(kkk).m_index.clear();
		#ifdef TRY_MINIMAL_VBO
			int64_t tmpppppp=0;
		#endif
		FaceIndexing& tmpFaceList = m_listFaces.getValue(kkk);
		for (size_t iii=0; iii<tmpFaceList.m_faces.size() ; iii++) {
			int32_t vertexVBOId[3];
			for(size_t indice=0 ; indice<3; indice++) {
				vec3 position = m_listVertex[tmpFaceList.m_faces[iii].m_vertex[indice]];
				vec3 normal;
				if (m_normalMode == ewol::Mesh::normalModeVertex) {
					normal = m_listVertexNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
				} else {
					normal = m_listFacesNormal[tmpFaceList.m_faces[iii].m_normal[indice]];
				}
				vec2 texturepos(m_listUV[tmpFaceList.m_faces[iii].m_uv[indice]].x(),1.0f-m_listUV[tmpFaceList.m_faces[iii].m_uv[indice]].y());
				// try to find it in the list :
				bool elementFind = false;
				#ifdef TRY_MINIMAL_VBO
				for (int32_t jjj=0; jjj<m_verticesVBO->sizeOnBufferVec3(MESH_VBO_VERTICES); jjj++) {
					if(    m_verticesVBO->getOnBufferVec3(MESH_VBO_VERTICES,jjj) == position
					    && m_verticesVBO->getOnBufferVec3(MESH_VBO_VERTICES_NORMAL,jjj) == normal
					    && m_verticesVBO->getOnBufferVec2(MESH_VBO_TEXTURE,jjj) == texturepos) {
						vertexVBOId[indice] = jjj;
						elementFind = true;
						//EWOL_DEBUG("search indice : " << jjj);
						tmpppppp += jjj;
						// stop searching ...
						break;
					}
				}
				#endif
				if (false == elementFind) {
					m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES, position);
					m_verticesVBO->pushOnBuffer(MESH_VBO_VERTICES_NORMAL, normal);
					m_verticesVBO->pushOnBuffer(MESH_VBO_TEXTURE, texturepos);
					vertexVBOId[indice] = m_verticesVBO->sizeOnBufferVec3(MESH_VBO_VERTICES)-1;
				}
			}
			for(size_t indice=0 ; indice<3; indice++) {
				tmpFaceList.m_index.push_back(vertexVBOId[indice]);
			}
		}
		#ifdef TRY_MINIMAL_VBO
			EWOL_DEBUG("nb cycle ? : " << tmpppppp);
		#endif
	}
	// update all the VBO elements ...
	m_verticesVBO->flush();
}


void ewol::resource::Mesh::createViewBox(const std::string& _materialName,float _size) {
	m_normalMode = ewol::Mesh::normalModeNone;
	// This is the direct generation basis on the .obj system
	/*
			           5                       6  
			            o---------------------o   
			           /.                    /|   
			          / .                   / |   
			         /  .                  /  |   
			        /   .                 /   |   
			       /    .                /    |   
			    4 /     .               /     |   
			     o---------------------o      |   
			     |      .              |7     |   
			     |      .              |      |   
			     |      .              |      |   
			     |      .              |      |   
			     |      o..............|......o   
			     |     . 1             |     / 2  
			     |    .                |    /     
			     |   .                 |   /      
			     |  .                  |  /       
			     | .                   | /        
			     |.                    |/         
			     o---------------------o          
			    0                       3         
	*/
	m_listVertex.push_back(vec3( _size, -_size, -_size)); // 0
	m_listVertex.push_back(vec3( _size, -_size,  _size)); // 1
	m_listVertex.push_back(vec3(-_size, -_size,  _size)); // 2
	m_listVertex.push_back(vec3(-_size, -_size, -_size)); // 3
	m_listVertex.push_back(vec3( _size,  _size, -_size)); // 4
	m_listVertex.push_back(vec3( _size,  _size,  _size)); // 5
	m_listVertex.push_back(vec3(-_size,  _size,  _size)); // 6
	m_listVertex.push_back(vec3(-_size,  _size, -_size)); // 7
	/*
		     o----------o----------o----------o
		     |8         |9         |10        |11
		     |          |          |          |
		     |          |          |          |
		     |    0     |    1     |    2     |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     o----------o----------o----------o
		     |4         |5         |6         |7
		     |          |          |          |
		     |          |          |          |
		     |    3     |    4     |    5     |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     o----------o----------o----------o
		     0          1          2          3
	*/
	m_listUV.push_back(vec2(0.0    , 0.0    )); // 0
	m_listUV.push_back(vec2(1.0/3.0, 0.0    )); // 1
	m_listUV.push_back(vec2(2.0/3.0, 0.0    )); // 2
	m_listUV.push_back(vec2(1.0    , 0.0    )); // 3
	m_listUV.push_back(vec2(0.0    , 0.5    )); // 4
	m_listUV.push_back(vec2(1.0/3.0, 0.5    )); // 5
	m_listUV.push_back(vec2(2.0/3.0, 0.5    )); // 6
	m_listUV.push_back(vec2(1.0    , 0.5    )); // 7
	m_listUV.push_back(vec2(0.0    , 1.0    )); // 8
	m_listUV.push_back(vec2(1.0/3.0, 1.0    )); // 9
	m_listUV.push_back(vec2(2.0/3.0, 1.0    )); // 10
	m_listUV.push_back(vec2(1.0    , 1.0    )); // 11
	
	if (m_listFaces.exist(_materialName) == false) {
		FaceIndexing empty;
		m_listFaces.add(_materialName, empty);
	}
	{
		FaceIndexing& tmpElement = m_listFaces[_materialName];
		tmpElement.m_faces.push_back(Face(0,1, 1,5,  2,6)); // 4
		tmpElement.m_faces.push_back(Face(0,1, 2,6,  3,2)); // 4
		tmpElement.m_faces.push_back(Face(4,4, 0,0,  3,1)); // 3
		tmpElement.m_faces.push_back(Face(4,4, 3,1,  7,5)); // 3
		tmpElement.m_faces.push_back(Face(2,6, 6,10, 7,11)); // 2
		tmpElement.m_faces.push_back(Face(2,6, 7,11, 3,7)); // 2
		tmpElement.m_faces.push_back(Face(4,2, 7,3,  6,7)); // 5
		tmpElement.m_faces.push_back(Face(4,2, 6,7,  5,6)); // 5
		tmpElement.m_faces.push_back(Face(1,5, 5,9,  6,10)); // 1
		tmpElement.m_faces.push_back(Face(1,5, 6,10, 2,6)); // 1
		tmpElement.m_faces.push_back(Face(0,4, 4,8,  5,9)); // 0
		tmpElement.m_faces.push_back(Face(0,4, 5,9,  1,5)); // 0
	}
	calculateNormaleFace();
}


bool ewol::resource::Mesh::loadOBJ(const std::string& _fileName) {
	m_normalMode = ewol::Mesh::normalModeNone;
#if 0
	etk::FSNode fileName(_fileName);
	// get the fileSize ...
	int32_t size = fileName.fileSize();
	if (size == 0 ) {
		EWOL_ERROR("No data in the file named=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.fileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	
	int32_t lineID = 0;
	while (NULL != fileName.fileGets(inputDataLine, 2048) )
	{
		lineID++;
		if (inputDataLine[0] == 'v') {
			if (inputDataLine[1] == 'n') {
				// Vertice normal   : vn 0.000000 0.000000 -1.000000
				// we did not use normal  == > we recalculated it if needed (some .obj does not export normal, then it is simple like this ...
				// TODO : Use the normal provided ... => can be smooth or not ... (cf check "s 1")
			} else if (inputDataLine[1] == 't') {
				// Texture position : vt 0.748573 0.750412
				vec2 vertex(0,0);
				sscanf(&inputDataLine[3], "%f %f", &vertex.m_floats[0], &vertex.m_floats[1]);
				m_listUV.push_back(vertex);
			} else {
				// Vertice position : v 1.000000 -1.000000 -1.000000
				vec3 vertex(0,0,0);
				sscanf(&inputDataLine[2], "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
				m_listVertex.push_back(vertex);
			}
		} else if (inputDataLine[0] == 'f') {
			// face : f 5/1/1 1/2/1 4/3/1*
			uint32_t vertexIndex[4], uvIndex[4], normalIndex[4];
			bool quadMode = true;
			int32_t matches = sscanf(&inputDataLine[2], "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
			                         &vertexIndex[0], &uvIndex[0], &normalIndex[0],
			                         &vertexIndex[1], &uvIndex[1], &normalIndex[1],
			                         &vertexIndex[2], &uvIndex[2], &normalIndex[2],
			                         &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
			if (12 != matches){
				// no normal mode :
				matches = sscanf(&inputDataLine[2], "%d/%d %d/%d %d/%d %d/%d\n",
				                 &vertexIndex[0], &uvIndex[0],
				                 &vertexIndex[1], &uvIndex[1],
				                 &vertexIndex[2], &uvIndex[2],
				                 &vertexIndex[3], &uvIndex[3] );
				if (8 != matches){
					quadMode = false;
					matches = sscanf(&inputDataLine[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
					                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
					                 &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
					if (9 != matches){
						// no normal mode :
						matches = sscanf(&inputDataLine[2], "%d/%d %d/%d %d/%d\n",
						                 &vertexIndex[0], &uvIndex[0],
						                 &vertexIndex[1], &uvIndex[1],
						                 &vertexIndex[2], &uvIndex[2] );
						if (6 != matches){
							EWOL_ERROR("Parsing error in the .obj files : " << fileName << " (l=" << lineID << ") in 'f' section : \"" << &inputDataLine[2] << "\" expected : %d/%d(/%d) %d/%d(/%d) %d/%d(/%d) (%d/%d(/%d)) () for option");
							continue;
						}
					}
				}
			}
			if (true == quadMode) {
				m_listFaces.push_back(Face(vertexIndex[0]-1, uvIndex[0]-1,
				                          vertexIndex[1]-1, uvIndex[1]-1,
				                          vertexIndex[2]-1, uvIndex[2]-1,
				                          vertexIndex[3]-1, uvIndex[3]-1));
			} else {
				m_listFaces.push_back(Face(vertexIndex[0]-1, uvIndex[0]-1,
				                          vertexIndex[1]-1, uvIndex[1]-1,
				                          vertexIndex[2]-1, uvIndex[2]-1));
			}
			/*
			EWOL_DEBUG(" plop : " << tmpFace.m_nbElement << " ? " << m_listFaces[m_listFaces.size()-1].m_nbElement);
			EWOL_DEBUG("      : " << tmpFace.m_vertex[0] << " ? " << m_listFaces[m_listFaces.size()-1].m_vertex[0]);
			EWOL_DEBUG("      : " << tmpFace.m_uv[0] << " ? " << m_listFaces[m_listFaces.size()-1].m_uv[0]);
			*/
		} else if (inputDataLine[0] == 's') {
			// ??? : s off
		} else if (inputDataLine[0] == '#') {
			// comment
			// nothing to do ... just go to the new line ...
		} else if(    inputDataLine[0] == 'u'
		           && inputDataLine[1] == 's'
		           && inputDataLine[2] == 'e'
		           && inputDataLine[3] == 'm'
		           && inputDataLine[4] == 't'
		           && inputDataLine[5] == 'l' ) {
			// Use Material : usemtl imageName.xxx
			while(    inputDataLine[strlen(inputDataLine)-1] == '\n'
			       || inputDataLine[strlen(inputDataLine)-1] == '\r'
			       || inputDataLine[strlen(inputDataLine)-1] == ' ') {
				if (1 == strlen(inputDataLine) ){
					break;
				}
				inputDataLine[strlen(inputDataLine)-1] = '\0';
			}
			std::string tmpVal(&inputDataLine[7]);
			setTexture(fileName.getRelativeFolder() + tmpVal);
		} else if(    inputDataLine[0] == 'm'
		           && inputDataLine[1] == 't'
		           && inputDataLine[2] == 'l'
		           && inputDataLine[3] == 'l'
		           && inputDataLine[4] == 'i'
		           && inputDataLine[5] == 'b' ) {
			// ???? : mtllib cube.mtl
		}
	}
	fileName.fileClose();
	generateVBO();
#endif
	return true;
}

void jumpEndLine(etk::FSNode& _file) {
	char current=_file.fileGet();
	while(    current != '\0'
	       && current != '\n') {
		//printf("%c", current);
		current=_file.fileGet();
	}
}

int32_t countIndent(etk::FSNode& _file) {
	int32_t nbIndent=0;
	int32_t nbSpacesTab=0;
	int32_t nbChar=0;
	//EWOL_DEBUG(" start count Indent");
	for(char current=_file.fileGet(); current != '\0'; current=_file.fileGet()) {
		nbChar++;
		//EWOL_DEBUG("parse : " << current);
		if (current == '\t') {
			nbSpacesTab = 0;
			nbIndent++;
		} else if (current == ' ') {
			nbSpacesTab++;
			if (nbSpacesTab == 4) {
				nbSpacesTab = 0;
				nbIndent++;
			}
		} else if (current == '#') {
			// Auto remove comment ...
			jumpEndLine(_file);
			return countIndent(_file);
		} else {
			
			break;
		}
	}
	//EWOL_DEBUG("indent : " << nbIndent);
	_file.fileSeek(-nbChar, etk::FSN_SEEK_CURRENT);
	return nbIndent;
}

char* loadNextData(char* _elementLine,
                   int64_t _maxData,
                   etk::FSNode& _file,
                   bool _removeTabs=false,
                   bool _stopColomn=false,
                   bool _stopPipe=true) {
	memset(_elementLine, 0, _maxData);
	char * element = _elementLine;
	int64_t outSize = 0;
	/*
	if (m_zipReadingOffset >= m_zipContent->size()) {
		element[0] = '\0';
		return NULL;
	}
	*/
	char current = _file.fileGet();
	while (current != '\0') {
		if(    _removeTabs == false
		    || element != _elementLine) {
			*element = current;
			element++;
		}
		if(    current == '\n'
		    || current == '\r'
		    || (    current == '|'
		         && _stopPipe == true)
		    || (    current == ':'
		         && _stopColomn == true) )
		{
			*element = '\0';
			//EWOL_DEBUG(" plop : '" << _elementLine << "'" );
			return _elementLine;
		} else if(    element == _elementLine
		           && current != '\t') {
			*element = current;
			element++;
		}
		// check maxData size ...
		if (outSize >= _maxData-1) {
			*element = '\0';
			return _elementLine;
		}
		current = _file.fileGet();
	}
	if (outSize == 0) {
		return NULL;
	} else {
		// send last line
		return _elementLine;
	}
	return NULL;
}

void removeEndLine(char* _val) {
	int32_t len = strlen(_val);
	if(    len>0
	    && (    _val[len-1] == '\n'
	         || _val[len-1] == '\r' ) ) {
		_val[len-1] = '\0';
	}
	len--;
	if(    len>0
	    && (    _val[len-1] == '\n'
	         || _val[len-1] == '\r') ) {
		_val[len-1] = '\0';
	}
}

enum emfModuleMode {
	EMFModuleNone,
	EMFModuleMesh,
	EMFModuleMeshNamed,
	EMFModuleMeshVertex,
	EMFModuleMeshUVMapping,
	EMFModuleMeshNormalVertex,
	EMFModuleMeshNormalFace,
	EMFModuleMeshFace,
	EMFModuleMeshFaceMaterial,
	EMFModuleMeshPhysics,
	EMFModuleMeshPhysicsNamed,
	EMFModuleMesh_END,
	EMFModuleMaterial,
	EMFModuleMaterialNamed,
	EMFModuleMaterial_END,
};

bool ewol::resource::Mesh::loadEMF(const std::string& _fileName) {
	m_checkNormal = true;
	m_normalMode = ewol::Mesh::normalModeNone;
	etk::FSNode fileName(_fileName);
	// get the fileSize ...
	int32_t size = fileName.fileSize();
	if (size == 0 ) {
		EWOL_ERROR("No data in the file named=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.fileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	// load the first line :
	fileName.fileGets(inputDataLine, 2048);
	if(0 == strncmp(inputDataLine, "EMF(STRING)", 11)) {
		// parse in string mode ...
	} else if (0 == strncmp(inputDataLine, "EMF(BINARY)", 11)) {
		EWOL_ERROR(" file binary mode is not supported now : 'EMF(BINARY)'");
		return false;
	} else {
		EWOL_ERROR(" file mode is not supported now : 'EMF(? ? ?)' = '" << inputDataLine << "'");
		return false;
	}
	enum emfModuleMode currentMode = EMFModuleNone;
	EWOL_VERBOSE("Start parsing Mesh file : " << fileName);
	// mesh global param :
	std::string currentMeshName = "";
	int32_t meshFaceMaterialID = -1;
	// material global param :
	std::string materialName = "";
	ewol::Material* material = NULL;
	// physical shape:
	ewol::PhysicsShape* physics = NULL;
	while(1) {
		int32_t level = countIndent(fileName);
		if (level == 0) {
			// new section ...
			if (NULL == loadNextData(inputDataLine, 2048, fileName)) {
				// reach end of file ...
				break;
			}
			if(0 == strncmp(inputDataLine, "Mesh :", 6) ) {
				currentMode = EMFModuleMesh;
				EWOL_VERBOSE("Parse Mesh :");
			} else if(0 == strncmp(inputDataLine, "Materials : ", 11) ) {
				currentMode = EMFModuleMaterial;
				EWOL_VERBOSE("Parse Material :");
			} else {
				currentMode = EMFModuleNone;
			}
		} else {
			if (currentMode >= EMFModuleMesh && currentMode <= EMFModuleMesh_END) {
				if (level == 1) {
					//Find mesh name ...
					if (NULL == loadNextData(inputDataLine, 2048, fileName, true)) {
						// reach end of file ...
						break;
					}
					removeEndLine(inputDataLine);
					currentMeshName = inputDataLine;
					currentMode = EMFModuleMeshNamed;
					EWOL_VERBOSE("    "<< currentMeshName);
					continue;
				}
				if (level == 2) {
					// In the mesh level 2 the line size must not exced 2048
					if (NULL == loadNextData(inputDataLine, 2048, fileName, true)) {
						// reach end of file ...
						break;
					}
					removeEndLine(inputDataLine);
					if(0 == strncmp(inputDataLine, "Vertex", 6) ) {
						currentMode = EMFModuleMeshVertex;
						EWOL_VERBOSE("        Vertex ...");
					} else if(0 == strncmp(inputDataLine, "UV-mapping", 10) ) {
						currentMode = EMFModuleMeshUVMapping;
						EWOL_VERBOSE("        UV-mapping ...");
					} else if(0 == strncmp(inputDataLine, "Normal(vertex)", 14) ) {
						currentMode = EMFModuleMeshNormalVertex;
						EWOL_VERBOSE("        Normal(vertex) ...");
					} else if(0 == strncmp(inputDataLine, "Normal(face)", 12) ) {
						currentMode = EMFModuleMeshNormalFace;
						EWOL_VERBOSE("        Normal(face) ...");
					} else if(0 == strncmp(inputDataLine, "Face", 4) ) {
						currentMode = EMFModuleMeshFace;
						EWOL_VERBOSE("        Face ...");
					} else if(0 == strncmp(inputDataLine, "Physics", 7) ) {
						currentMode = EMFModuleMeshPhysics;
						EWOL_VERBOSE("        Physics ...");
					} else {
						EWOL_ERROR("        Unknow mesh property '"<<inputDataLine<<"'");
						currentMode = EMFModuleMeshNamed;
					}
					continue;
				}
				// level > 2
				switch (currentMode) {
					default:
						EWOL_ERROR("Unknow ... "<< level);
						jumpEndLine(fileName);
						break;
					case EMFModuleMeshVertex: {
						vec3 vertex(0,0,0);
						while (NULL != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
							m_listVertex.push_back(vertex);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EWOL_VERBOSE("            " << m_listVertex.size() << " vertex");
						break;
					}
					case EMFModuleMeshUVMapping: {
						vec2 uvMap(0,0);
						while (NULL != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f", &uvMap.m_floats[0], &uvMap.m_floats[1]);
							m_listUV.push_back(uvMap);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EWOL_VERBOSE("            " << m_listUV.size() << " coord");
						break;
					}
					case EMFModuleMeshNormalVertex: {
						m_normalMode = ewol::Mesh::normalModeVertex;
						vec3 normal(0,0,0);
						// find the vertex Normal list.
						while (NULL != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
							m_listVertexNormal.push_back(normal);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EWOL_VERBOSE("            " << m_listVertexNormal.size() << " Normals");
						break;
					}
					case EMFModuleMeshNormalFace: {
						m_normalMode = ewol::Mesh::normalModeFace;
						vec3 normal(0,0,0);
						// find the face Normal list.
						while (NULL != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
							m_listFacesNormal.push_back(normal);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EWOL_VERBOSE("            " << m_listFacesNormal.size() << " Normals");
						break;
					}
					case EMFModuleMeshFace:
					case EMFModuleMeshFaceMaterial:
						if (level == 3) {
							//Find mesh name ...
							if (NULL == loadNextData(inputDataLine, 2048, fileName, true)) {
								// reach end of file ...
								break;
							}
							removeEndLine(inputDataLine);
							// new maretial selection
							currentMode = EMFModuleMeshFaceMaterial;
							FaceIndexing empty;
							m_listFaces.add(inputDataLine, empty);
							meshFaceMaterialID = m_listFaces.getId(inputDataLine);
							EWOL_VERBOSE("            " << inputDataLine);
						} else if (currentMode == EMFModuleMeshFaceMaterial) {
							while (NULL != loadNextData(inputDataLine, 2048, fileName, true, true) ) {
								if (inputDataLine[0] == '\0') {
									// end of line
									break;
								}
								if (meshFaceMaterialID < 0) {
									continue;
								}
								uint32_t vertexIndex[3], uvIndex[3], normalIndex[3];
								vertexIndex[0] = 0;
								vertexIndex[1] = 0;
								vertexIndex[2] = 0;
								uvIndex[0] = 0;
								uvIndex[1] = 0;
								uvIndex[2] = 0;
								normalIndex[0] = 0;
								normalIndex[1] = 0;
								normalIndex[2] = 0;
								scanf(inputDataLine, "%d/%d/%d %d/%d/%d %d/%d/%d",
								      &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								      &vertexIndex[1], &uvIndex[1], &normalIndex[1],
								      &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
								m_listFaces.getValue(meshFaceMaterialID).m_faces.push_back(Face(vertexIndex[0], uvIndex[0], normalIndex[0],
								                                                               vertexIndex[1], uvIndex[1], normalIndex[1],
								                                                               vertexIndex[2], uvIndex[2], normalIndex[2]));
								/*
								EWOL_DEBUG("face :" << vertexIndex[0] << "/" << uvIndex[0] << "/" << normalIndex[0] <<
								           " " << vertexIndex[1] << "/" << uvIndex[1] << "/" << normalIndex[1] <<
								           " " << vertexIndex[2] << "/" << uvIndex[2] << "/" << normalIndex[2]);
								*/
								int32_t len = strlen(inputDataLine)-1;
								if(    inputDataLine[len] == '\n'
								    || inputDataLine[len] == '\r') {
									break;
								}
							}
							EWOL_VERBOSE("                " << m_listFaces.getValue(meshFaceMaterialID).m_faces.size() << " faces");
						} else {
							// insert element without material ...
							EWOL_ERROR(" try to add face without material selection ...");
							jumpEndLine(fileName);
						}
						break;
					case EMFModuleMeshPhysics:
					case EMFModuleMeshPhysicsNamed:
						if (NULL == loadNextData(inputDataLine, 2048, fileName, true, false, false)) {
							// reach end of file ...
							break;
						}
						removeEndLine(inputDataLine);
						if (level == 3) {
							physics = ewol::PhysicsShape::create(inputDataLine);
							if (physics == NULL) {
								EWOL_ERROR("Allocation error when creating physical shape ...");
								continue;
							}
							m_physics.push_back(physics);
							EWOL_VERBOSE("            " << m_physics.size() << " " << inputDataLine);
							currentMode = EMFModuleMeshPhysicsNamed;
						} else if (currentMode == EMFModuleMeshPhysicsNamed) {
							if (physics == NULL) {
								EWOL_ERROR("Can not parse :'" << inputDataLine << "' in physical shape ...");
								continue;
							}
							if (false == physics->parse(inputDataLine)) {
								EWOL_ERROR("ERROR when parsing :'" << inputDataLine << "' in physical shape ...");
							}
						}
						break;
				}
				continue;
			} else if (currentMode >= EMFModuleMaterial && currentMode <= EMFModuleMaterial_END) {
				// all material element is stored on 1 line (size < 2048)
				if (NULL == loadNextData(inputDataLine, 2048, fileName, true)) {
					// reach end of file ...
					break;
				}
				removeEndLine(inputDataLine);
				if (level == 1) {
					// add previous material :
					if(    materialName != ""
					    && material!=NULL) {
						m_materials.add(materialName, material);
						materialName = "";
						material = NULL;
					}
					material = new ewol::Material();
					materialName = inputDataLine;
					currentMode = EMFModuleMaterialNamed;
					EWOL_VERBOSE("    "<< materialName);
					continue;
				}
				// level >1
				if (currentMode != EMFModuleMaterialNamed) {
					EWOL_WARNING("        Unknow element ..."<< level);
					jumpEndLine(fileName);
					continue;
				}
				if (NULL == material) {
					EWOL_ERROR("material allocation error");
					jumpEndLine(fileName);
					continue;
				}
				if(0 == strncmp(inputDataLine,"Ns ",3)) {
					float tmpVal=0;
					sscanf(&inputDataLine[3], "%f", &tmpVal);
					material->setShininess(tmpVal);
					EWOL_VERBOSE("        Shininess " << tmpVal);
				} else if(0 == strncmp(inputDataLine,"Ka ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setAmbientFactor(tmp);
					EWOL_VERBOSE("        AmbientFactor " << tmp);
				} else if(0 == strncmp(inputDataLine,"Kd ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setDiffuseFactor(tmp);
					EWOL_VERBOSE("        DiffuseFactor " << tmp);
				} else if(0 == strncmp(inputDataLine,"Ks ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->setSpecularFactor(tmp);
					EWOL_VERBOSE("        SpecularFactor " << tmp);
				} else if(0 == strncmp(inputDataLine,"Ni ",3)) {
					float tmpVal=0;
					sscanf(&inputDataLine[3], "%f", &tmpVal);
					// TODO : ...
					EWOL_VERBOSE("        Ni " << tmpVal);
				} else if(0 == strncmp(inputDataLine,"d ",2)) {
					float tmpVal=0;
					sscanf(&inputDataLine[2], "%f", &tmpVal);
					// TODO : ...
					EWOL_VERBOSE("        d " << tmpVal);
				} else if(0 == strncmp(inputDataLine,"illum ",6)) {
					int tmpVal=0;
					sscanf(&inputDataLine[6], "%d", &tmpVal);
					// TODO : ...
					EWOL_VERBOSE("        illum " << tmpVal);
				} else if(0 == strncmp(inputDataLine,"map_Kd ",7)) {
					material->setTexture0(fileName.getRelativeFolder() + &inputDataLine[7]);
					EWOL_VERBOSE("        Texture " << &inputDataLine[7]);
				} else {
					EWOL_ERROR("unknow material property ... : '" << inputDataLine << "'");
				}
			} else {
				// unknow ...
				EWOL_WARNING("Unknow type of line  == > jump end of line ... ");
				jumpEndLine(fileName);
			}
		}
	}
	// add last material ...
	if(    materialName != ""
	    && material!=NULL) {
		m_materials.add(materialName, material);
		materialName = "";
		material = NULL;
	}
	EWOL_VERBOSE("Stop parsing Mesh file");
	
	fileName.fileClose();
	generateVBO();
	return true;
}

void ewol::resource::Mesh::addMaterial(const std::string& _name, ewol::Material* _data) {
	if (NULL == _data) {
		EWOL_ERROR(" can not add material with null pointer");
		return;
	}
	if (_name == "") {
		EWOL_ERROR(" can not add material with no name");
		return;
	}
	// really add the material :
	m_materials.add(_name, _data);
}

void ewol::resource::Mesh::setShape(void* _shape) {
	if (m_functionFreeShape!=NULL) {
		m_functionFreeShape(m_pointerShape);
		m_pointerShape = NULL;
	}
	m_pointerShape=_shape;
}

ewol::resource::Mesh* ewol::resource::Mesh::keep(const std::string& _meshName) {
	ewol::resource::Mesh* object = static_cast<ewol::resource::Mesh*>(getManager().localKeep(_meshName));
	if (NULL != object) {
		return object;
	}
	EWOL_DEBUG("CREATE: Mesh: '" << _meshName << "'");
	object = new ewol::resource::Mesh(_meshName);
	if (NULL == object) {
		EWOL_ERROR("allocation error of a resource : ??Mesh??" << _meshName);
		return NULL;
	}
	getManager().localAdd(object);
	return object;
}

void ewol::resource::Mesh::release(ewol::Mesh*& _object) {
	if (NULL == _object) {
		return;
	}
	ewol::resource::Resource* object2 = static_cast<ewol::resource::Resource*>(_object);
	getManager().release(object2);
	_object = NULL;
}
