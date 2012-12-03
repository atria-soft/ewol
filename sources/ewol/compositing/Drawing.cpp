/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/debug.h>
#include <ewol/compositing/Drawing.h>


#if 0

static void generatePolyGone(etk::Vector<vec2 > & input, etk::Vector<vec2 > & output )
{
	if (input.Size()<3) {
		return;
	}
	// TODO : Regenerate a linear poligone generation
	for (int32_t iii=1; iii<input.Size()-1; iii++) {
		output.PushBack(input[0]);
		output.PushBack(input[iii]);
		output.PushBack(input[iii+1]);
	}
	//EWOL_DEBUG("generate Plygone : " << input.Size() << " ==> " << output.Size() );
}

static void SutherlandHodgman(etk::Vector<vec2 > & input, etk::Vector<vec2 > & output, float sx, float sy, float ex, float ey)
{
	// with Sutherland-Hodgman-Algorithm
	if (input.Size() <0) {
		return;
	}
	//int32_t sizeInit=input.Size();
	// last element :
	vec2 destPoint;
	vec2 lastElement = input[input.Size()-1];
	bool inside = true;
	if (lastElement.x < sx) {
		inside = false;
	}
	//EWOL_DEBUG("generate an crop : ");
	for(int32_t iii=0; iii<input.Size(); iii++) {
		if(input[iii].x < sx) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.PushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.PushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*sx + bbb;
				destPoint.x = sx;
				output.PushBack(destPoint);
				output.PushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	//EWOL_DEBUG("generate an crop on element : " << sizeInit << " ==> " << output.Size() << "intermediate (1)");
	input = output;
	output.Clear();
	lastElement = input[input.Size()-1];
	inside = true;
	if (lastElement.y < sy) {
		inside = false;
	}
	for(int32_t iii=0; iii<input.Size(); iii++) {
		if(input[iii].y < sy) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//x=aaay+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.PushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.PushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = sy;
				destPoint.x = sy*aaa + bbb;
				output.PushBack(destPoint);
				output.PushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	input = output;
	output.Clear();
	lastElement = input[input.Size()-1];
	inside = true;
	if (lastElement.x > ex) {
		inside = false;
	}
	//EWOL_DEBUG("generate an crop : ");
	for(int32_t iii=0; iii<input.Size(); iii++) {
		if(input[iii].x > ex) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.PushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.PushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.y-input[iii].y) / (lastElement.x-input[iii].x);
				float bbb = lastElement.y - (aaa*lastElement.x);
				destPoint.y = aaa*ex + bbb;
				destPoint.x = ex;
				output.PushBack(destPoint);
				output.PushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	input = output;
	output.Clear();
	lastElement = input[input.Size()-1];
	inside = true;
	if (lastElement.y > ey) {
		inside = false;
	}
	for(int32_t iii=0; iii<input.Size(); iii++) {
		if(input[iii].y > ey) {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> OUT ");
				//new point intersection ...
				//x=aaay+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.PushBack(destPoint);
			} else {
				//EWOL_DEBUG("element OUT ==> OUT ");
			}
			inside = false;
		} else {
			if(true == inside) {
				//EWOL_DEBUG("element IN ==> IN ");
				output.PushBack(input[iii]);
			} else {
				//EWOL_DEBUG("element OUT ==> IN ");
				//new point intersection ...
				//y=aaax+bbb
				float aaa = (lastElement.x-input[iii].x) / (lastElement.y-input[iii].y);
				float bbb = lastElement.x - (aaa*lastElement.y);
				destPoint.y = ey;
				destPoint.x = ey*aaa + bbb;
				output.PushBack(destPoint);
				output.PushBack(input[iii]);
			}
			inside = true;
		}
		// update the last point position :
		lastElement.x = input[iii].x;
		lastElement.y = input[iii].y;
	}
	
	
	//EWOL_DEBUG("generate an crop on element : " << sizeInit << " ==> " << output.Size() );
}
#endif

ewol::Drawing::Drawing(void) :
	m_position(0.0, 0.0, 0.0),
	m_clippingPosStart(0.0, 0.0, 0.0),
	m_clippingPosStop(0.0, 0.0, 0.0),
	m_clippingEnable(false),
	m_color(draw::color::black),
	m_colorBg(draw::color::none),
	m_GLprogram(NULL),
	m_GLPosition(-1),
	m_GLMatrix(-1),
	m_GLColor(-1),
	m_thickness(0.0),
	m_triElement(0)
{
	LoadProgram();
	for (int32_t iii=0; iii<3; iii++) {
		m_triangle[iii] = m_position;
		m_tricolor[iii] = m_color;
	}
}


ewol::Drawing::~Drawing(void)
{
	UnLoadProgram();
}


void ewol::Drawing::GenerateTriangle(void)
{
	m_triElement = 0;
	
	m_coord.PushBack(m_triangle[0]);
	m_coordColor.PushBack(m_tricolor[0]);
	m_coord.PushBack(m_triangle[1]);
	m_coordColor.PushBack(m_tricolor[1]);
	m_coord.PushBack(m_triangle[2]);
	m_coordColor.PushBack(m_tricolor[2]);
}

void ewol::Drawing::InternalSetColor(draw::Color& color)
{
	if (m_triElement < 1) {
		m_tricolor[0] = color;
	}
	if (m_triElement < 2) {
		m_tricolor[1] = color;
	}
	if (m_triElement < 3) {
		m_tricolor[2] = color;
	}
}


void ewol::Drawing::SetPoint(vec3 point)
{
	m_triangle[m_triElement] = point;
	m_triElement++;
	if (m_triElement>=3) {
		GenerateTriangle();
	}
}


void ewol::Drawing::ResetCount(void)
{
	m_triElement = 0;
}

void ewol::Drawing::UnLoadProgram(void)
{
	if (NULL!=m_GLprogram) {
		ewol::resource::Release(m_GLprogram);
		m_GLprogram = NULL;
	}
}

void ewol::Drawing::LoadProgram(void)
{
	// remove previous loading ... in case
	UnLoadProgram();
	// oad the new ...
	etk::UString tmpString("DATA:color3.prog");
	// get the shader resource :
	if (true == ewol::resource::Keep(tmpString, m_GLprogram) ) {
		m_GLPosition = m_GLprogram->GetAttribute("EW_coord3d");
		m_GLColor    = m_GLprogram->GetAttribute("EW_color");
		m_GLMatrix   = m_GLprogram->GetUniform("EW_MatrixTransformation");
	}
}

void ewol::Drawing::Draw(void)
{
	if (m_coord.Size()<=0) {
		// TODO : a remÃštre ...
		//EWOL_WARNING("Nothink to draw...");
		return;
	}
	if (m_GLprogram==NULL) {
		EWOL_ERROR("No shader ...");
		return;
	}
	// set Matrix : translation/positionMatrix
	mat4 tmpMatrix = ewol::openGL::GetMatrix()*m_matrixApply;
	m_GLprogram->Use();
	m_GLprogram->UniformMatrix4fv(m_GLMatrix, 1, tmpMatrix.m_mat);
	// position :
	m_GLprogram->SendAttribute(m_GLPosition, 3/*x,y,z*/, &m_coord[0]);
	// color :
	m_GLprogram->SendAttribute(m_GLColor, 4/*r,g,b,a*/, &m_coordColor[0]);
	// Request the draw od the elements : 
	glDrawArrays(GL_TRIANGLES, 0, m_coord.Size());
	m_GLprogram->UnUse();
}


void ewol::Drawing::Clear(void)
{
	// call upper class
	ewol::Compositing::Clear();
	// Reset Buffer :
	m_coord.Clear();
	m_coordColor.Clear();
	// Reset temporal variables :
	m_position = vec3(0.0, 0.0, 0.0);
	
	m_clippingPosStart = vec3(0.0, 0.0, 0.0);
	m_clippingPosStop = vec3(0.0, 0.0, 0.0);
	m_clippingEnable = false;
	
	m_color = draw::color::black;
	m_colorBg = draw::color::none;
	
	for (int32_t iii=0; iii<3; iii++) {
		m_triangle[iii] = m_position;
		m_tricolor[iii] = m_color;
	}
}


vec3 ewol::Drawing::GetPos(void)
{
	return m_position;
}


void ewol::Drawing::SetPos(vec3 pos)
{
	m_position = pos;
}


void ewol::Drawing::SetRelPos(vec3 pos)
{
	m_position += pos;
}


void ewol::Drawing::SetColor(draw::Color color)
{
	m_color = color;
}


void ewol::Drawing::SetColorBg(draw::Color color)
{
	m_colorBg = color;
}


void ewol::Drawing::SetClippingWidth(vec3 pos, vec3 width)
{
	SetClipping(pos, pos+width);
}

void ewol::Drawing::SetClipping(vec3 pos, vec3 posEnd)
{
	// note the internal system all time request to have a bounding all time in the same order
	if (pos.x <= posEnd.x) {
		m_clippingPosStart.x = pos.x;
		m_clippingPosStop.x = posEnd.x;
	} else {
		m_clippingPosStart.x = posEnd.x;
		m_clippingPosStop.x = pos.x;
	}
	if (pos.y <= posEnd.y) {
		m_clippingPosStart.y = pos.y;
		m_clippingPosStop.y = posEnd.y;
	} else {
		m_clippingPosStart.y = posEnd.y;
		m_clippingPosStop.y = pos.y;
	}
	if (pos.z <= posEnd.z) {
		m_clippingPosStart.z = pos.z;
		m_clippingPosStop.z = posEnd.z;
	} else {
		m_clippingPosStart.z = posEnd.z;
		m_clippingPosStop.z = pos.z;
	}
	m_clippingEnable = true;
}


void ewol::Drawing::SetClippingMode(bool newMode)
{
	m_clippingEnable = newMode;
}


void ewol::Drawing::SetThickness(float thickness)
{
	m_thickness = thickness;
	// thickness must be positive
	if (m_thickness < 0) {
		m_thickness *= -1;
	}
}


void ewol::Drawing::AddVertex(void)
{
	InternalSetColor(m_color);
	SetPoint(m_position);
}


void ewol::Drawing::LineTo(vec3 dest)
{
	ResetCount();
	InternalSetColor(m_color);
	if (m_position.x == dest.x && m_position.y == dest.y) {
		EWOL_WARNING("Try to draw an line width 0");
		return;
	}
	//teta = tan-1(oposer/adjacent)
	float teta = 0;
	if (m_position.x <= dest.x) {
		teta = atan((dest.y-m_position.y)/(dest.x-m_position.x));
	} else {
		teta = M_PI + atan((dest.y-m_position.y)/(dest.x-m_position.x));
	}
	if (teta < 0) {
		teta += 2*M_PI;
	} else if (teta > 2*M_PI) {
		teta -= 2*M_PI;
	}
	//EWOL_DEBUG("teta = " << (teta*180/(M_PI)) << " deg." );
	float offsety = sin(teta-M_PI/2) * (m_thickness/2);
	float offsetx = cos(teta-M_PI/2) * (m_thickness/2);

	SetPoint(vec3(m_position.x - offsetx, m_position.y - offsety, (float)0.0) );
	SetPoint(vec3(m_position.x + offsetx, m_position.y + offsety, (float)0.0) );
	SetPoint(vec3(dest.x + offsetx, dest.y + offsety, (float)0.0) );
	
	SetPoint(vec3(dest.x + offsetx, dest.y + offsety, (float)0.0) );
	SetPoint(vec3(dest.x - offsetx, dest.y - offsety, (float)0.0) );
	SetPoint(vec3(m_position.x - offsetx, m_position.y - offsety, (float)0.0) );
	// update the system position :
	m_position = dest;
}

void ewol::Drawing::LineRel(vec3 vect)
{
	LineTo(m_position+vect);
}


void ewol::Drawing::Rectangle(vec3 dest)
{
	ResetCount();
	InternalSetColor(m_color);
	/* Bitmap position
	 *      xA     xB
	 *   yC *------*
	 *      |      |
	 *      |      |
	 *   yD *------*
	 */
	float dxA = m_position.x;
	float dxB = dest.x;
	float dyC = m_position.y;
	float dyD = dest.y;
	if (true == m_clippingEnable) {
		if (dxA < m_clippingPosStart.x) {
			dxA = m_clippingPosStart.x;
		}
		if (dxB > m_clippingPosStop.x) {
			dxB = m_clippingPosStop.x;
		}
		if (dyC < m_clippingPosStart.y) {
			dyC = m_clippingPosStart.y;
		}
		if (dyD > m_clippingPosStop.y) {
			dyD = m_clippingPosStop.y;
		}
	}
	if(    dyC >= dyD
	    || dxA >= dxB) {
		return;
	}
	SetPoint(vec3(dxA, dyD, (float)0.0) );
	SetPoint(vec3(dxA, dyC, (float)0.0) );
	SetPoint(vec3(dxB, dyC, (float)0.0) );

	SetPoint(vec3(dxB, dyC, (float)0.0) );
	SetPoint(vec3(dxB, dyD, (float)0.0) );
	SetPoint(vec3(dxA, dyD, (float)0.0) );
}


void ewol::Drawing::RectangleWidth(vec3 size)
{
	Rectangle(m_position+size);
}



void ewol::Drawing::Cube(vec3 dest)
{
	
}


void ewol::Drawing::Circle(float radius, float angleStart, float angleStop)
{
	ResetCount();
	
	if (radius<0) {
		radius *= -1;
	}
	angleStop = angleStop-angleStart;
	
	
	int32_t nbOcurence = radius;
	if (nbOcurence < 10)
	{
		nbOcurence = 10;
	}
	
	// display background :
	if (m_colorBg.a!=0) {
		InternalSetColor(m_colorBg);
		for (int32_t iii=0; iii<nbOcurence; iii++) {
			SetPoint(vec3(m_position.x, m_position.y) );
			
			float angleOne = angleStart + (angleStop* iii / nbOcurence) ;
			float offsety = sin(angleOne) * radius;
			float offsetx = cos(angleOne) * radius;
			
			SetPoint(vec3(m_position.x + offsetx, m_position.y + offsety) );
			
			float angleTwo = angleStart + (angleStop* (iii+1) / nbOcurence) ;
			offsety = sin(angleTwo) * radius;
			offsetx = cos(angleTwo) * radius;
			
			SetPoint(vec3(m_position.x + offsetx, m_position.y + offsety) );
		}
	}
	
	// show if we have a border :
	if(    m_thickness==0
	    || m_color.a==0) {
		return;
	}
	InternalSetColor(m_color);
	for (int32_t iii=0; iii<nbOcurence; iii++) {
		
		float angleOne =  angleStart + (angleStop* iii     / nbOcurence) ;
		float offsetExty = sin(angleOne) * (radius+m_thickness/2);
		float offsetExtx = cos(angleOne) * (radius+m_thickness/2);
		float offsetInty = sin(angleOne) * (radius-m_thickness/2);
		float offsetIntx = cos(angleOne) * (radius-m_thickness/2);
		
		float angleTwo =  angleStart + (angleStop*  (iii+1) / nbOcurence );
		float offsetExt2y = sin(angleTwo) * (radius+m_thickness/2);
		float offsetExt2x = cos(angleTwo) * (radius+m_thickness/2);
		float offsetInt2y = sin(angleTwo) * (radius-m_thickness/2);
		float offsetInt2x = cos(angleTwo) * (radius-m_thickness/2);
		
		SetPoint(vec3(m_position.x + offsetIntx,  m_position.y + offsetInty));
		SetPoint(vec3(m_position.x + offsetExtx,  m_position.y + offsetExty));
		SetPoint(vec3(m_position.x + offsetExt2x, m_position.y + offsetExt2y));
		
		SetPoint(vec3(m_position.x + offsetExt2x, m_position.y + offsetExt2y));
		SetPoint(vec3(m_position.x + offsetInt2x, m_position.y + offsetInt2y));
		SetPoint(vec3(m_position.x + offsetIntx,  m_position.y + offsetInty));
	}
}

