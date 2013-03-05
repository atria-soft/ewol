/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <etk/Vector.h>
#include <etk/os/FSNode.h>

#include <ewol/debug.h>
#include <ewol/renderer/ResourceManager.h>
#include <ewol/renderer/resources/MeshObj.h>


ewol::MeshObj::MeshObj(etk::UString _fileName) :
	ewol::Mesh(_fileName)
{
	etk::FSNode fileName(_fileName);
	// Get the fileSize ...
	int32_t size = fileName.FileSize();
	if (size == 0 ) {
		EWOL_ERROR("No data in the file named=\"" << fileName << "\"");
		return;
	}
	if(false == fileName.FileOpenRead() ) {
		EWOL_ERROR("Can not find the file name=\"" << fileName << "\"");
		return;
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
}


