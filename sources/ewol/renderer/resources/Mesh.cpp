/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/renderer/resources/Mesh.h>
#include <ewol/renderer/ResourceManager.h>
#include <etk/os/FSNode.h>

#undef __class__
#define __class__	"Mesh"

ewol::Mesh::Mesh(const etk::UString& _fileName, const etk::UString& _shaderName) :
	ewol::Resource(_fileName),
	m_normalMode(normalModeNone)
{
	EWOL_DEBUG("Load a new mesh : '" << _fileName << "'");
	// get the shader resource :
	m_GLPosition = 0;
	
	m_light.SetDirection(vec3(0,cos(M_PI/4),sin(M_PI/4)));
	m_light.SetHalfPlane(vec3(1,0,0));
	m_light.SetAmbientColor(vec4(1,1,1,1));
	m_light.SetDiffuseColor(vec4(1.0,1.0,1.0,1));
	m_light.SetSpecularColor(vec4(0.0,0.0,0.0,1));
	
	if (true == ewol::resource::Keep(_shaderName, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord3d");
		m_GLtexture = m_GLprogram->GetAttribute("EW_texture2d");
		m_GLNormal = m_GLprogram->GetAttribute("EW_normal");
		m_GLMatrix = m_GLprogram->GetUniform("EW_MatrixTransformation");
		m_GLMatrixPosition = m_GLprogram->GetUniform("EW_MatrixPosition");
		// Link material and Lights
		m_GLMaterial.Link(m_GLprogram, "EW_material");
		m_light.Link(m_GLprogram, "EW_directionalLight");
	}
	// this is the properties of the buffer requested : "r"/"w" + "-" + buffer type "f"=flaot "i"=integer
	ewol::resource::Keep("w-fff", m_verticesVBO);
	
	// load the curent file :
	etk::UString tmpName = _fileName.ToLower();
	// select the corect Loader :
	if (true == tmpName.EndWith(".obj") ) {
		if (false == LoadOBJ(_fileName)) {
			EWOL_ERROR("Error To load OBJ file " << tmpName );
			return;
		}
	} else if (true == tmpName.EndWith(".emf") ) {
		if (false == LoadEMF(_fileName)) {
			EWOL_ERROR("Error To load EMF file " << tmpName );
			return;
		}
		//EWOL_CRITICAL("Load a new mesh : '" << _fileName << "' (end)");
	} else {
		// nothing to do ==> reqiest an enmpty mesh ==> user manage it ...
	}
}

ewol::Mesh::~Mesh(void)
{
	// remove dynamics dependencies :
	ewol::resource::Release(m_GLprogram);
	ewol::resource::Release(m_verticesVBO);
}


void ewol::Mesh::Draw(mat4& positionMatrix)
{
	//EWOL_DEBUG("Request Draw : " << m_listFaces.GetValue(0).m_index.Size() << " elements");
	/*
	if (m_listIndexFaces.Size()<=0) {
		return;
	}
	*/
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 camMatrix = ewol::openGL::GetCameraMatrix();
	mat4 tmpMatrix = projMatrix * camMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	m_GLprogram->UniformMatrix4fv(m_GLMatrixPosition, 1, positionMatrix.m_mat);
	// position :
	m_GLprogram->SendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->SendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->SendAttributePointer(m_GLNormal, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
	// draw lights :
	m_light.Draw(m_GLprogram);
	
	int32_t nbElementDraw = 0;
	for (esize_t kkk=0; kkk<m_listFaces.Size(); kkk++) {
		if (false == m_materials.Exist(m_listFaces.GetKey(kkk))) {
			EWOL_WARNING("missing materials : '" << m_listFaces.GetKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.GetKey(kkk)]->Draw(m_GLprogram, m_GLMaterial);
		ewol::openGL::DrawElements(GL_TRIANGLES, m_listFaces.GetValue(kkk).m_index);
		nbElementDraw += m_listFaces.GetValue(kkk).m_index.Size();
	}
	EWOL_DEBUG("Request Draw : " << m_listFaces.Size() << ":" << nbElementDraw << " elements [" << m_name << "]");
	m_GLprogram->UnUse();
	ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	// TODO : UNDERSTAND why ... it is needed
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void ewol::Mesh::Draw2(mat4& positionMatrix)
{
	//
	/*
	if (m_listIndexFaces.Size()<=0) {
		return;
	}
	*/
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	ewol::openGL::Enable(ewol::openGL::FLAG_DEPTH_TEST);
	//EWOL_DEBUG("    Display " << m_coord.Size() << " elements" );
	m_GLprogram->Use();
	// set Matrix : translation/positionMatrix
	mat4 projMatrix = ewol::openGL::GetMatrix();
	mat4 tmpMatrix = projMatrix;
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	m_GLprogram->UniformMatrix4fv(m_GLMatrixPosition, 1, positionMatrix.m_mat);
	// position :
	m_GLprogram->SendAttributePointer(m_GLPosition, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES);
	// Texture :
	m_GLprogram->SendAttributePointer(m_GLtexture, 2/*u,v*/, m_verticesVBO, MESH_VBO_TEXTURE);
	// position :
	m_GLprogram->SendAttributePointer(m_GLNormal, 3/*x,y,z*/, m_verticesVBO, MESH_VBO_VERTICES_NORMAL);
	m_light.Draw(m_GLprogram);
	
	// draw materials :
	int32_t nbElementDraw = 0;
	for (esize_t kkk=0; kkk<m_listFaces.Size(); kkk++) {
		if (false == m_materials.Exist(m_listFaces.GetKey(kkk))) {
			EWOL_WARNING("missing materials : '" << m_listFaces.GetKey(kkk) << "'");
			continue;
		}
		m_materials[m_listFaces.GetKey(kkk)]->Draw(m_GLprogram, m_GLMaterial);
		ewol::openGL::DrawElements(GL_TRIANGLES, m_listFaces.GetValue(kkk).m_index);
		nbElementDraw += m_listFaces.GetValue(kkk).m_index.Size();
	}
	EWOL_DEBUG("Request Draw : " << m_listFaces.Size() << ":" << nbElementDraw << " elements [" << m_name << "]");
	m_GLprogram->UnUse();
	ewol::openGL::Disable(ewol::openGL::FLAG_DEPTH_TEST);
	// TODO : UNDERSTAND why ... it is needed
	glBindBuffer(GL_ARRAY_BUFFER,0);
}


// normal calculation of the normal face is really easy :
void ewol::Mesh::CalculateNormaleFace(void)
{
	m_listFacesNormal.Clear();
	if (m_normalMode != ewol::Mesh::normalModeFace) {
		etk::Vector<Face>& tmpFaceList = m_listFaces.GetValue(0).m_faces;
		for(int32_t iii=0 ; iii<tmpFaceList.Size() ; iii++) {
			// for all case, We use only the 3 vertex for quad element, in theory 3D modeler export element in triangle if it is not a real plane.
			vec3 normal = btCross(m_listVertex[tmpFaceList[iii].m_vertex[0]]-m_listVertex[tmpFaceList[iii].m_vertex[1]],
			                      m_listVertex[tmpFaceList[iii].m_vertex[1]]-m_listVertex[tmpFaceList[iii].m_vertex[2]]);
			m_listFacesNormal.PushBack(normal.normalized());
		}
		m_normalMode = ewol::Mesh::normalModeFace;
	}
}

void ewol::Mesh::CalculateNormaleEdge(void)
{
	m_listVertexNormal.Clear();
	if (m_normalMode != ewol::Mesh::normalModeVertex) {
		for(int32_t iii=0 ; iii<m_listVertex.Size() ; iii++) {
			etk::Vector<Face>& tmpFaceList = m_listFaces.GetValue(0).m_faces;
			vec3 normal(0,0,0);
			// add the vertex from all the element in the list for face when the element in the face ...
			for(int32_t jjj=0 ; jjj<tmpFaceList.Size() ; jjj++) {
				if(    tmpFaceList[jjj].m_vertex[0] == iii
				    || tmpFaceList[jjj].m_vertex[1] == iii
				    || tmpFaceList[jjj].m_vertex[2] == iii) {
					normal += m_listFacesNormal[jjj];
				}
			}
			if (normal == vec3(0,0,0)) {
				m_listVertexNormal.PushBack(vec3(1,1,1));
			} else {
				m_listVertexNormal.PushBack(normal.normalized());
			}
		}
		m_normalMode = ewol::Mesh::normalModeVertex;
	}
}

// for debugging ...
//#define PRINT_HALF (1)
//#define TRY_MINIMAL_VBO

void ewol::Mesh::GenerateVBO(void)
{
	// calculate the normal of all faces if needed
	if (m_normalMode == ewol::Mesh::normalModeNone) {
		// when no normal detected ==> auto Generate Face normal ....
		CalculateNormaleFace();
	}
	// Generate element in 2 pass : 
	//    - create new index dependeng a vertex is a unique componenet of position, texture, normal
	//    - the index list generation (can be dynamic ... (TODO later)
	for (esize_t kkk=0; kkk<m_listFaces.Size(); kkk++) {
		// clean faces indexes :
		m_listFaces.GetValue(kkk).m_index.Clear();
		#ifdef TRY_MINIMAL_VBO
			int64_t tmpppppp=0;
		#endif
		FaceIndexing& tmpFaceList = m_listFaces.GetValue(kkk);
		for (int32_t iii=0; iii<tmpFaceList.m_faces.Size() ; iii++) {
			int32_t vertexVBOId[3];
			for(int32_t indice=0 ; indice<3; indice++) {
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
				for (int32_t jjj=0; jjj<m_verticesVBO->SizeOnBufferVec3(MESH_VBO_VERTICES); jjj++) {
					if(    m_verticesVBO->GetOnBufferVec3(MESH_VBO_VERTICES,jjj) == position
					    && m_verticesVBO->GetOnBufferVec3(MESH_VBO_VERTICES_NORMAL,jjj) == normal
					    && m_verticesVBO->GetOnBufferVec2(MESH_VBO_TEXTURE,jjj) == texturepos) {
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
					m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES, position);
					m_verticesVBO->PushOnBuffer(MESH_VBO_VERTICES_NORMAL, normal);
					m_verticesVBO->PushOnBuffer(MESH_VBO_TEXTURE, texturepos);
					vertexVBOId[indice] = m_verticesVBO->SizeOnBufferVec3(MESH_VBO_VERTICES)-1;
				}
			}
			for(int32_t indice=0 ; indice<3; indice++) {
				tmpFaceList.m_index.PushBack(vertexVBOId[indice]);
			}
		}
		#ifdef TRY_MINIMAL_VBO
			EWOL_DEBUG("nb cycle ? : " << tmpppppp);
		#endif
	}
	// update all the VBO elements ...
	m_verticesVBO->Flush();
}


void ewol::Mesh::CreateViewBox(const etk::UString& _materialName,float _size)
{
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
	m_listVertex.PushBack(vec3( _size, -_size, -_size)); // 0
	m_listVertex.PushBack(vec3( _size, -_size,  _size)); // 1
	m_listVertex.PushBack(vec3(-_size, -_size,  _size)); // 2
	m_listVertex.PushBack(vec3(-_size, -_size, -_size)); // 3
	m_listVertex.PushBack(vec3( _size,  _size, -_size)); // 4
	m_listVertex.PushBack(vec3( _size,  _size,  _size)); // 5
	m_listVertex.PushBack(vec3(-_size,  _size,  _size)); // 6
	m_listVertex.PushBack(vec3(-_size,  _size, -_size)); // 7
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
	m_listUV.PushBack(vec2(0.0    , 0.0    )); // 0
	m_listUV.PushBack(vec2(1.0/3.0, 0.0    )); // 1
	m_listUV.PushBack(vec2(2.0/3.0, 0.0    )); // 2
	m_listUV.PushBack(vec2(1.0    , 0.0    )); // 3
	m_listUV.PushBack(vec2(0.0    , 0.5    )); // 4
	m_listUV.PushBack(vec2(1.0/3.0, 0.5    )); // 5
	m_listUV.PushBack(vec2(2.0/3.0, 0.5    )); // 6
	m_listUV.PushBack(vec2(1.0    , 0.5    )); // 7
	m_listUV.PushBack(vec2(0.0    , 1.0    )); // 8
	m_listUV.PushBack(vec2(1.0/3.0, 1.0    )); // 9
	m_listUV.PushBack(vec2(2.0/3.0, 1.0    )); // 10
	m_listUV.PushBack(vec2(1.0    , 1.0    )); // 11
	
	if (m_listFaces.Exist(_materialName)==false) {
		FaceIndexing empty;
		m_listFaces.Add(_materialName, empty);
	}
	{
		FaceIndexing& tmpElement = m_listFaces[_materialName];
		tmpElement.m_faces.PushBack(Face(0,1, 1,5,  2,6)); // 4
		tmpElement.m_faces.PushBack(Face(0,1, 2,6,  3,2)); // 4
		tmpElement.m_faces.PushBack(Face(4,4, 0,0,  3,1)); // 3
		tmpElement.m_faces.PushBack(Face(4,4, 3,1,  7,5)); // 3
		tmpElement.m_faces.PushBack(Face(2,6, 6,10, 7,11)); // 2
		tmpElement.m_faces.PushBack(Face(2,6, 7,11, 3,7)); // 2
		tmpElement.m_faces.PushBack(Face(4,2, 7,3,  6,7)); // 5
		tmpElement.m_faces.PushBack(Face(4,2, 6,7,  5,6)); // 5
		tmpElement.m_faces.PushBack(Face(1,5, 5,9,  6,10)); // 1
		tmpElement.m_faces.PushBack(Face(1,5, 6,10, 2,6)); // 1
		tmpElement.m_faces.PushBack(Face(0,4, 4,8,  5,9)); // 0
		tmpElement.m_faces.PushBack(Face(0,4, 5,9,  1,5)); // 0
	}
	CalculateNormaleFace();
}


bool ewol::Mesh::LoadOBJ(const etk::UString& _fileName)
{
	m_normalMode = ewol::Mesh::normalModeNone;
#if 0
	etk::FSNode fileName(_fileName);
	// Get the fileSize ...
	int32_t size = fileName.FileSize();
	if (size == 0 ) {
		EWOL_ERROR("No data in the file named=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.FileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	
	int32_t lineID = 0;
	while (NULL != fileName.FileGets(inputDataLine, 2048) )
	{
		lineID++;
		if (inputDataLine[0]=='v') {
			if (inputDataLine[1]=='n') {
				// Vertice normal   : vn 0.000000 0.000000 -1.000000
				// we did not use normal ==> we recalculated it if needed (some .obj does not export normal, then it is simple like this ...
				// TODO : Use the normal provided ... => can be smooth or not ... (cf check "s 1")
			} else if (inputDataLine[1]=='t') {
				// Texture position : vt 0.748573 0.750412
				vec2 vertex(0,0);
				sscanf(&inputDataLine[3], "%f %f", &vertex.m_floats[0], &vertex.m_floats[1]);
				m_listUV.PushBack(vertex);
			} else {
				// Vertice position : v 1.000000 -1.000000 -1.000000
				vec3 vertex(0,0,0);
				sscanf(&inputDataLine[2], "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
				m_listVertex.PushBack(vertex);
			}
		} else if (inputDataLine[0]=='f') {
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
			if (true==quadMode) {
				m_listFaces.PushBack(Face(vertexIndex[0]-1, uvIndex[0]-1,
				                          vertexIndex[1]-1, uvIndex[1]-1,
				                          vertexIndex[2]-1, uvIndex[2]-1,
				                          vertexIndex[3]-1, uvIndex[3]-1));
			} else {
				m_listFaces.PushBack(Face(vertexIndex[0]-1, uvIndex[0]-1,
				                          vertexIndex[1]-1, uvIndex[1]-1,
				                          vertexIndex[2]-1, uvIndex[2]-1));
			}
			/*
			EWOL_DEBUG(" plop : " << tmpFace.m_nbElement << " ? " << m_listFaces[m_listFaces.Size()-1].m_nbElement);
			EWOL_DEBUG("      : " << tmpFace.m_vertex[0] << " ? " << m_listFaces[m_listFaces.Size()-1].m_vertex[0]);
			EWOL_DEBUG("      : " << tmpFace.m_uv[0] << " ? " << m_listFaces[m_listFaces.Size()-1].m_uv[0]);
			*/
		} else if (inputDataLine[0]=='s') {
			// ??? : s off
		} else if (inputDataLine[0]=='#') {
			// comment
			// nothing to do ... just go to the new line ...
		} else if(    inputDataLine[0]=='u'
		           && inputDataLine[1]=='s'
		           && inputDataLine[2]=='e'
		           && inputDataLine[3]=='m'
		           && inputDataLine[4]=='t'
		           && inputDataLine[5]=='l' ) {
			// Use Material : usemtl imageName.xxx
			while(    inputDataLine[strlen(inputDataLine)-1] == '\n'
			       || inputDataLine[strlen(inputDataLine)-1] == '\r'
			       || inputDataLine[strlen(inputDataLine)-1] == ' ') {
				if (1 == strlen(inputDataLine) ){
					break;
				}
				inputDataLine[strlen(inputDataLine)-1] = '\0';
			}
			etk::UString tmpVal(&inputDataLine[7]);
			SetTexture(fileName.GetRelativeFolder() + tmpVal);
		} else if(    inputDataLine[0]=='m'
		           && inputDataLine[1]=='t'
		           && inputDataLine[2]=='l'
		           && inputDataLine[3]=='l'
		           && inputDataLine[4]=='i'
		           && inputDataLine[5]=='b' ) {
			// ???? : mtllib cube.mtl
		}
	}
	fileName.FileClose();
	GenerateVBO();
#endif
	return true;
}

void JumpEndLine(etk::FSNode& _file)
{
	char current=_file.FileGet();
	while(    current != '\0'
	       && current != '\n') {
		//printf("%c", current);
		current=_file.FileGet();
	}
}

int32_t CountIndent(etk::FSNode& _file)
{
	int32_t nbIndent=0;
	int32_t nbSpacesTab=0;
	int32_t nbChar=0;
	//EWOL_DEBUG(" start count Indent");
	for(char current=_file.FileGet(); current != '\0'; current=_file.FileGet()) {
		nbChar++;
		//EWOL_DEBUG("parse : " << current);
		if (current=='\t') {
			nbSpacesTab = 0;
			nbIndent++;
		} else if (current==' ') {
			nbSpacesTab++;
			if (nbSpacesTab==4) {
				nbSpacesTab = 0;
				nbIndent++;
			}
		} else if (current=='#') {
			// Auto remove comment ...
			JumpEndLine(_file);
			return CountIndent(_file);
		} else {
			
			break;
		}
	}
	//EWOL_DEBUG("indent : " << nbIndent);
	_file.FileSeek(-nbChar, etk::FSN_SEEK_CURRENT);
	return nbIndent;
}

char* LoadNextData(char* _elementLine, int64_t _maxData, etk::FSNode& _file, bool _removeTabs=false, bool _stopColomn=false, bool _stopPipe=true)
{
	memset(_elementLine, 0, _maxData);
	char * element = _elementLine;
	int64_t outSize = 0;
	/*
	if (m_zipReadingOffset>=m_zipContent->Size()) {
		element[0] = '\0';
		return NULL;
	}
	*/
	char current = _file.FileGet();
	while (current != '\0') {
		if(    _removeTabs==false
		    || element != _elementLine) {
			*element = current;
			element++;
		}
		if(    current == '\n'
		    || current == '\r'
		    || (    current == '|'
		         && _stopPipe==true)
		    || (    current == ':'
		         && _stopColomn==true) )
		{
			*element = '\0';
			//EWOL_DEBUG(" plop : '" << _elementLine << "'" );
			return _elementLine;
		} else if(    element == _elementLine
		           && current != '\t') {
			*element = current;
			element++;
		}
		// check maxData Size ...
		if (outSize>=_maxData-1) {
			*element = '\0';
			return _elementLine;
		}
		current = _file.FileGet();
	}
	if (outSize==0) {
		return NULL;
	} else {
		// send last line
		return _elementLine;
	}
	return NULL;
}

void RemoveEndLine(char* _val)
{
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
typedef enum {
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
} emfModuleMode_te;

bool ewol::Mesh::LoadEMF(const etk::UString& _fileName)
{
	m_normalMode = ewol::Mesh::normalModeNone;
	etk::FSNode fileName(_fileName);
	// Get the fileSize ...
	int32_t size = fileName.FileSize();
	if (size == 0 ) {
		EWOL_ERROR("No data in the file named=\"" << fileName << "\"");
		return false;
	}
	if(false == fileName.FileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return false;
	}
	char inputDataLine[2048];
	// load the first line :
	fileName.FileGets(inputDataLine, 2048);
	if(0==strncmp(inputDataLine, "EMF(STRING)", 11)) {
		// parse in string mode ...
	} else if (0==strncmp(inputDataLine, "EMF(BINARY)", 11)) {
		EWOL_ERROR(" file binary mode is not supported now : 'EMF(BINARY)'");
		return false;
	} else {
		EWOL_ERROR(" file mode is not supported now : 'EMF(? ? ?)' = '" << inputDataLine << "'");
		return false;
	}
	emfModuleMode_te currentMode = EMFModuleNone;
	EWOL_DEBUG("Start parsing Mesh file : " << fileName);
	// mesh global param :
	etk::UString currentMeshName = "";
	int32_t meshFaceMaterialID = -1;
	// material global param :
	etk::UString materialName = "";
	ewol::Material* material = NULL;
	// physical shape:
	ewol::PhysicsShape* physics = NULL;
	while(1) {
		int32_t level = CountIndent(fileName);
		if (level==0) {
			// new section ...
			if (NULL == LoadNextData(inputDataLine, 2048, fileName)) {
				// reach end of file ...
				break;
			}
			if(0==strncmp(inputDataLine, "Mesh :", 6) ) {
				currentMode = EMFModuleMesh;
				EWOL_DEBUG("Parse Mesh :");
			} else if(0==strncmp(inputDataLine, "Materials : ", 11) ) {
				currentMode = EMFModuleMaterial;
				EWOL_DEBUG("Parse Material :");
			} else {
				currentMode = EMFModuleNone;
			}
		} else {
			if (currentMode>=EMFModuleMesh && currentMode<=EMFModuleMesh_END) {
				if (level == 1) {
					//Find mesh name ...
					if (NULL == LoadNextData(inputDataLine, 2048, fileName, true)) {
						// reach end of file ...
						break;
					}
					RemoveEndLine(inputDataLine);
					currentMeshName = inputDataLine;
					currentMode = EMFModuleMeshNamed;
					EWOL_DEBUG("    "<< currentMeshName);
					continue;
				}
				if (level == 2) {
					// In the mesh level 2 the line size must not exced 2048
					if (NULL == LoadNextData(inputDataLine, 2048, fileName, true)) {
						// reach end of file ...
						break;
					}
					RemoveEndLine(inputDataLine);
					if(0==strncmp(inputDataLine, "Vertex", 6) ) {
						currentMode = EMFModuleMeshVertex;
						EWOL_DEBUG("        Vertex ...");
					} else if(0==strncmp(inputDataLine, "UV-mapping", 10) ) {
						currentMode = EMFModuleMeshUVMapping;
						EWOL_DEBUG("        UV-mapping ...");
					} else if(0==strncmp(inputDataLine, "Normal(vertex)", 14) ) {
						currentMode = EMFModuleMeshNormalVertex;
						EWOL_DEBUG("        Normal(vertex) ...");
					} else if(0==strncmp(inputDataLine, "Normal(face)", 12) ) {
						currentMode = EMFModuleMeshNormalFace;
						EWOL_DEBUG("        Normal(face) ...");
					} else if(0==strncmp(inputDataLine, "Face", 4) ) {
						currentMode = EMFModuleMeshFace;
						EWOL_DEBUG("        Face ...");
					} else if(0==strncmp(inputDataLine, "Physics", 7) ) {
						currentMode = EMFModuleMeshPhysics;
						EWOL_DEBUG("        Physics ...");
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
						JumpEndLine(fileName);
						break;
					case EMFModuleMeshVertex: {
						vec3 vertex(0,0,0);
						while (NULL != LoadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &vertex.m_floats[0], &vertex.m_floats[1], &vertex.m_floats[2] );
							m_listVertex.PushBack(vertex);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EWOL_DEBUG("            " << m_listVertex.Size() << " vertex");
						break;
					}
					case EMFModuleMeshUVMapping: {
						vec2 uvMap(0,0);
						while (NULL != LoadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f", &uvMap.m_floats[0], &uvMap.m_floats[1]);
							m_listUV.PushBack(uvMap);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EWOL_DEBUG("            " << m_listUV.Size() << " coord");
						break;
					}
					case EMFModuleMeshNormalVertex: {
						m_normalMode = ewol::Mesh::normalModeVertex;
						vec3 normal(0,0,0);
						// find the vertex Normal list.
						while (NULL != LoadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
							m_listVertexNormal.PushBack(normal);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EWOL_DEBUG("            " << m_listVertexNormal.Size() << " Normals");
						break;
					}
					case EMFModuleMeshNormalFace: {
						m_normalMode = ewol::Mesh::normalModeFace;
						vec3 normal(0,0,0);
						// find the face Normal list.
						while (NULL != LoadNextData(inputDataLine, 2048, fileName, true, true) ) {
							if (inputDataLine[0] == '\0') {
								break;
							}
							sscanf(inputDataLine, "%f %f %f", &normal.m_floats[0], &normal.m_floats[1], &normal.m_floats[2] );
							m_listFacesNormal.PushBack(normal);
							int32_t len = strlen(inputDataLine)-1;
							if(    inputDataLine[len] == '\n'
							    || inputDataLine[len] == '\r') {
								break;
							}
						}
						EWOL_DEBUG("            " << m_listFacesNormal.Size() << " Normals");
						break;
					}
					case EMFModuleMeshFace:
					case EMFModuleMeshFaceMaterial:
						if (level == 3) {
							//Find mesh name ...
							if (NULL == LoadNextData(inputDataLine, 2048, fileName, true)) {
								// reach end of file ...
								break;
							}
							RemoveEndLine(inputDataLine);
							// new maretial selection
							currentMode = EMFModuleMeshFaceMaterial;
							FaceIndexing empty;
							m_listFaces.Add(inputDataLine, empty);
							meshFaceMaterialID = m_listFaces.GetId(inputDataLine);
							EWOL_DEBUG("            " << inputDataLine);
						} else if (currentMode == EMFModuleMeshFaceMaterial) {
							while (NULL != LoadNextData(inputDataLine, 2048, fileName, true, true) ) {
								if (inputDataLine[0] == '\0') {
									// end of line
									break;
								}
								if (meshFaceMaterialID < 0) {
									continue;
								}
								int32_t matches;
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
								matches = sscanf(inputDataLine, "%d/%d/%d %d/%d/%d %d/%d/%d",
								                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
								                 &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
								m_listFaces.GetValue(meshFaceMaterialID).m_faces.PushBack(Face(vertexIndex[0], uvIndex[0], normalIndex[0],
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
							EWOL_DEBUG("                " << m_listFaces.GetValue(meshFaceMaterialID).m_faces.Size() << " faces");
						} else {
							// insert element without material ...
							EWOL_ERROR(" try to add face without material selection ...");
							JumpEndLine(fileName);
						}
						break;
					case EMFModuleMeshPhysics:
					case EMFModuleMeshPhysicsNamed:
						if (NULL == LoadNextData(inputDataLine, 2048, fileName, true, false, false)) {
							// reach end of file ...
							break;
						}
						RemoveEndLine(inputDataLine);
						if (level == 3) {
							physics = ewol::PhysicsShape::Create(inputDataLine);
							if (physics==NULL) {
								EWOL_ERROR("Allocation error when creating physical shape ...");
								continue;
							}
							m_physics.PushBack(physics);
							EWOL_DEBUG("            " << inputDataLine);
							currentMode = EMFModuleMeshPhysicsNamed;
						} else if (currentMode == EMFModuleMeshPhysicsNamed) {
							if (physics == NULL) {
								EWOL_ERROR("Can not parse :'" << inputDataLine << "' in physical shape ...");
								continue;
							}
							if (false == physics->Parse(inputDataLine)) {
								EWOL_ERROR("ERROR when parsing :'" << inputDataLine << "' in physical shape ...");
							}
						}
						break;
				}
				continue;
			} else if (currentMode>=EMFModuleMaterial && currentMode<=EMFModuleMaterial_END) {
				// all material element is stored on 1 line (size < 2048)
				if (NULL == LoadNextData(inputDataLine, 2048, fileName, true)) {
					// reach end of file ...
					break;
				}
				RemoveEndLine(inputDataLine);
				if (level == 1) {
					// add previous material :
					if(    materialName != ""
					    && material!=NULL) {
						m_materials.Add(materialName, material);
						materialName = "";
						material = NULL;
					}
					material = new ewol::Material();
					materialName = inputDataLine;
					currentMode = EMFModuleMaterialNamed;
					EWOL_DEBUG("    "<< materialName);
					continue;
				}
				// level >1
				if (currentMode != EMFModuleMaterialNamed) {
					EWOL_WARNING("        Unknow element ..."<< level);
					JumpEndLine(fileName);
					continue;
				}
				if (NULL == material) {
					EWOL_ERROR("material allocation error");
					JumpEndLine(fileName);
					continue;
				}
				if(0==strncmp(inputDataLine,"Ns ",3)) {
					float tmpVal=0;
					sscanf(&inputDataLine[3], "%f", &tmpVal);
					material->SetShininess(tmpVal);
					EWOL_WARNING("        Shininess " << tmpVal);
				} else if(0==strncmp(inputDataLine,"Ka ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->SetAmbientFactor(tmp);
					EWOL_WARNING("        AmbientFactor " << tmp);
				} else if(0==strncmp(inputDataLine,"Kd ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->SetDiffuseFactor(tmp);
					EWOL_WARNING("        DiffuseFactor " << tmp);
				} else if(0==strncmp(inputDataLine,"Ks ",3)) {
					float tmpVal1=0;
					float tmpVal2=0;
					float tmpVal3=0;
					sscanf(&inputDataLine[3], "%f %f %f", &tmpVal1, &tmpVal2, &tmpVal3);
					vec4 tmp(tmpVal1, tmpVal2, tmpVal3, 1);
					material->SetSpecularFactor(tmp);
					EWOL_WARNING("        SpecularFactor " << tmp);
				} else if(0==strncmp(inputDataLine,"Ni ",3)) {
					float tmpVal=0;
					sscanf(&inputDataLine[3], "%f", &tmpVal);
					// TODO : ...
					EWOL_WARNING("        Ni " << tmpVal);
				} else if(0==strncmp(inputDataLine,"d ",2)) {
					float tmpVal=0;
					sscanf(&inputDataLine[2], "%f", &tmpVal);
					// TODO : ...
					EWOL_WARNING("        d " << tmpVal);
				} else if(0==strncmp(inputDataLine,"illum ",6)) {
					int tmpVal=0;
					sscanf(&inputDataLine[6], "%d", &tmpVal);
					// TODO : ...
					EWOL_WARNING("        illum " << tmpVal);
				} else if(0==strncmp(inputDataLine,"map_Kd ",7)) {
					material->SetTexture0(fileName.GetRelativeFolder() + &inputDataLine[7]);
					EWOL_WARNING("        Texture " << &inputDataLine[7]);
				} else {
					EWOL_ERROR("unknow material property ... : '" << inputDataLine << "'");
				}
			} else {
				// unknow ...
				EWOL_WARNING("Unknow type of line ==> jump end of line ... ");
				JumpEndLine(fileName);
			}
		}
	}
	// add last material ...
	if(    materialName != ""
	    && material!=NULL) {
		m_materials.Add(materialName, material);
		materialName = "";
		material = NULL;
	}
	EWOL_DEBUG("Stop parsing Mesh file");
	
	fileName.FileClose();
	GenerateVBO();
	return true;
}



void ewol::Mesh::AddMaterial(const etk::UString& _name, ewol::Material* _data)
{
	if (NULL==_data) {
		EWOL_ERROR(" can not add material with null pointer");
		return;
	}
	if (_name=="") {
		EWOL_ERROR(" can not add material with no name");
		return;
	}
	// really add the material :
	m_materials.Add(_name, _data);
}
