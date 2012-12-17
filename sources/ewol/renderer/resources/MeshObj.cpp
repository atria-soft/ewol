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
	
	
	etk::Vector<int32_t> indicesVertices;
	etk::Vector<int32_t> indicesUv;
	etk::Vector<int32_t> indicesNormal;
	etk::Vector< vec3 > vertices;
	etk::Vector< vec2 > uvTextures;
	etk::Vector< vec3 > normals;
	
	
	while (NULL != fileName.FileGets(inputDataLine, 2048) )
	{
		if (inputDataLine[0]=='v') {
			if (inputDataLine[1]=='n') {
				// Vertice normal   : vn 0.000000 0.000000 -1.000000
				vec3 vertex;
				sscanf(&inputDataLine[3], "%f %f %f", &vertex.x, &vertex.y, &vertex.z );
				normals.PushBack(vertex);
			} else if (inputDataLine[1]=='t') {
				// Texture position : vt 0.748573 0.750412
				vec2 vertex;
				sscanf(&inputDataLine[3], "%f %f", &vertex.x, &vertex.y);
				uvTextures.PushBack(vertex);
			} else {
				// Vertice position : v 1.000000 -1.000000 -1.000000
				vec3 vertex;
				sscanf(&inputDataLine[2], "%f %f %f", &vertex.x, &vertex.y, &vertex.z );
				vertices.PushBack(vertex);
			}
		} else if (inputDataLine[0]=='f') {
			// face : f 5/1/1 1/2/1 4/3/1*
			uint32_t vertexIndex[3], uvIndex[3], normalIndex[3];
			int32_t matches = sscanf(&inputDataLine[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n",
			                         &vertexIndex[0], &uvIndex[0], &normalIndex[0],
			                         &vertexIndex[1], &uvIndex[1], &normalIndex[1],
			                         &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (9 != matches){
				EWOL_ERROR("Parsing error in the .obj files : " << fileName);
				continue;
			}
			indicesVertices.PushBack(vertexIndex[0]);
			indicesVertices.PushBack(vertexIndex[1]);
			indicesVertices.PushBack(vertexIndex[2]);
			indicesUv.PushBack(uvIndex[0]);
			indicesUv.PushBack(uvIndex[1]);
			indicesUv.PushBack(uvIndex[2]);
			indicesNormal.PushBack(normalIndex[0]);
			indicesNormal.PushBack(normalIndex[1]);
			indicesNormal.PushBack(normalIndex[2]);
			
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
			ivec2 tmpSize(256, 256);
			if (NULL != m_texture1) {
				EWOL_INFO("Release previous loaded texture ... ");
				ewol::resource::Release(m_texture1);
			}
			etk::UString tmpFilename = fileName.GetRelativeFolder() + tmpVal;
			if (false == ewol::resource::Keep(tmpFilename, m_texture1, tmpSize)) {
				EWOL_ERROR("Can not load specific texture : " << tmpVal);
			}
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
	// For each vertex of each triangle
	for( uint32_t iii=0; iii<indicesVertices.Size(); iii++ ){
		// Get the indices of its attributes
		uint32_t vertexIndex = indicesVertices[iii];
		uint32_t uvIndex     = indicesUv[iii];
		uint32_t normalIndex = indicesNormal[iii];
		
		// Put the attributes in buffers
		m_object.m_vertices.PushBack(vertices[vertexIndex-1]);
		m_object.m_uvTextures.PushBack(uvTextures[uvIndex-1]);
		m_object.m_normals.PushBack(normals[normalIndex-1]);
		draw::Color  tmpppp(0xFFFFFFFF);
		draw::Colorf tmppppp(tmpppp);
		m_coordColor.PushBack(tmppppp);
		
	}
}
