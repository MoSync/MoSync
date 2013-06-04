/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/


//------------------------------------------------------------------------------------------
// Filename: BMFont.cpp
//
// Supports BMFont (BitMap Font Generator)
//
#include "GFont.h"
#include "Utils.h"
#include "ReadCSV.h"
#include <GLES2/gl2.h>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <mavsprintf.h>
#include "Shaders.h"

/**
* \brief Create the texture used for rendering.
* @param MAHandle resource
* @return index of shader element
*/

GLuint CreateTexture(MAHandle resource)
{
	GLuint textureHandle;

	lprintfln("CreateTexture\n");
	//Create an OpenGL 2D texture from the R_BOX resource.
	glGenTextures(1, &textureHandle);
	checkGLError("CreateTexture::glGenTextures(1, &textureHandle)");
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	checkGLError("glBindTexture");
	int res = maOpenGLTexImage2D(resource);
	if (res < 0)
	{
		lprintfln("Error: CreateTexture => load failed = %i",res);
		return (GLuint)MA_GL_TEX_IMAGE_2D_INVALID_IMAGE;
	}
	lprintfln("Info: CreateTexture => loaded = %i",textureHandle);

	// Set texture parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	checkGLError("glTexParameteri");



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR /*  GL_NEAREST*/);
	checkGLError("glTexParameteri");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR /*  GL_NEAREST*/);
	checkGLError("glTexParameteri");
	return textureHandle;
}

/**
 * \brief BMFont
 * Constructor
 */

BMFont::BMFont()
{
	m_string2enum["info"]		= PBM_INFO;
	m_string2enum["common"]		= PBM_COMMON;
	m_string2enum["page"]		= PBM_PAGE;
	m_string2enum["chars"]		= PBM_CHARS;
	m_string2enum["char"]		= PBM_CHAR;
	m_string2enum["kernings"]	= PBM_KERNINGS;
	m_string2enum["kerning"]	= PBM_KERNING;
	m_string2enum["idle"]		= PBM_IDLE;
}

/**
 * \brief BMFont Copy constructor
 * @param other  referenced element of a BMFont
 */

BMFont::BMFont(const BMFont& other) :
	IFont(),
	m_chars(other.m_chars),
	m_kernings(other.m_kernings),
	m_texStores(other.m_texStores),
	m_pages(other.m_pages)
{
	if (this != &other)
		return;

	m_info		= other.m_info;
	m_common	= other.m_common;
}

/**
 *	\brief ~BMFont
 *	Destructor
 */

BMFont::~BMFont()
{
	Clear();
}

/**
 * \brief Init(), initiates from resource data .fnt file and texture.
 * @param fontResource, resource of a .fnt contains textual information about
 * this font.
 * @param textureResources,
 * array of resource of one or multiple font textures that represents a Font.
 * @return bool true/false true = success, false = failed.
 */

bool BMFont::Init(MAHandle fontResource, std::vector<MAHandle> &textureResources)
{
	// Load in the text file containing the font data.
	bool result = LoadFontData(fontResource);	// with its font specific data. (see BM font for more format info) use first example for now

	lprintfln("Info: BMFont::Init => load Font Textures %i (%i)",m_common.m_pages,(int)textureResources.size());

	if(result == false)
	{
		lprintfln("Error: BMFont::Init => load failed = %i",result);
		return false;
	}

	if (m_common.m_pages > (int)textureResources.size())
	{
		lprintfln("Error: BMFont::Init => pages=%i > amount of texture resources:%i",m_common.m_pages, (int)textureResources.size());
		return false;
	}

	// Load the texture that has the font characters on it.
	for(int i=0; i<m_common.m_pages; i++)
	{
		GLuint texHandle = CreateTexture(textureResources[i]);

		if (((int)texHandle) < 0)
		{
			lprintfln("Error: BMFont::Init => Invalid 2D Image err=%i",texHandle);
			return false;
		}
		else
		{
			lprintfln("Info: BMFont::Init Texture = %d => TexHandle = %x loaded", i, texHandle);
		}
		TextureStore t;
		t.m_texture = texHandle;
		m_texStores.push_back(t);
	}
	return true;
}

/**
 * \brief Clear
 * Release Texture and Font DB creation
 */

void BMFont::Clear()
{
	ReleaseTexture();
	ReleaseFontData();
	return;
}

/**
 * \brief getBMType
 * @param header lookup enum from string hence the name
 * @return ParseBMFont enum
 */

BMFont::ParseBMFont BMFont::getBMType(std::string &header)
{
	std::hash_map<std::string, ParseBMFont>::iterator it = m_string2enum.find(header);
	return (it!=m_string2enum.end())? it->second: PBM_IDLE;
}

/**
 * \note helper function
 * \brief GetValue gets a value from a string e.g. "label = 10" => value is int 10
 * @param keyValue e.g. "label = 10"
 * @return returns the value part of = XX e.g. int 10
 */
int GetValue(std::string &keyValue)
{
	std::vector<std::string> key_value = Utils::split(keyValue,'=',true);
	return atoi((key_value[1]).c_str());
}

/**
 * \note helper function
 * \brief GetKeyValueFrom gets both Key and Value from a string
 * e.g. "label = 10" => key=label value=10
 * @param keyValue input string e.g. "label = 10"
 * @param key output e.g. key = "label"
 * @param value output e.g. value = 10
 */
void GetKeyValueFrom(std::string &keyValue,std::string &key, std::string &value)
{
	std::vector<std::string> key_value = Utils::split(keyValue,'=',true);
	key		= key_value[0];
	value	= key_value[1];
}

/**
 * \brief BMInfo::parse function
 * @param line, contains string contains whole line of text from .fnt file
 * chopped up in string array from the space delimited data.
 */
void BMInfo::parse(std::vector<std::string> &line)
{
	std::string key,value;
	lprintfln("BMInfo parse size=%d",(int)line.size());
	for(size_t i=1; i<line.size(); i++)
	{
		GetKeyValueFrom(line[i],key,value);
//		lprintfln("BMInfo line=%s key=%s = value=%s",line[i].c_str(), key.c_str(), value.c_str());

		if (key == "face")
			m_face = Utils::unQuote(value);
		else if(key == "size")
			m_size = atoi(value.c_str());
		else if (key == "bold")
			m_bold = atoi(value.c_str())? true: false;
		else if (key == "italic")
			m_italic = atoi(value.c_str())? true: false;
		else if (key == "charset")
			m_charset = value;
		else if (key == "unicode")
			m_unicode = atoi(value.c_str());
		else if (key == "stretchH")
			m_stretchH = atoi(value.c_str());
		else if (key == "smooth")
			m_smooth = atoi(value.c_str());
		else if (key == "aa")
			m_aa = atoi(value.c_str());
		else if (key == "padding")
		{
			std::vector<std::string> padding = Utils::split(value,',',true);
			for(size_t j=0; j<padding.size() && j<4;j++)
				m_padding[j] = atoi(padding[j].c_str());
		}
		else if (key == "spacing")
		{
			std::vector<std::string> spacing = Utils::split(value,',',true);
			for(size_t j=0; j<spacing.size() && j<2;j++)
				m_spacing[j] = atoi(spacing[j].c_str());
		}
		else if (key == "outline")
			m_outline = atoi(value.c_str());
		else
		//	debugOut << "BMInfo::parse => Unknown type:" << key.c_str() << std::endl;
			lprintfln("BMInfo::parse at line %d => Unknown type: %s/n",(int)i,key.c_str());

	}
}

/**
 * \brief BMCommon::parse function
 * @param line, contains string contains whole line of text from .fnt file
 * chopped up in string array from the space delimited data.
 */

void BMCommon::parse(std::vector<std::string> &line)
{
	std::string key,value;
	for(size_t i=1; i<line.size(); i++)
	{
		GetKeyValueFrom(line[i],key,value);
		if (key == "lineHeight")
			m_lineHeight = atoi(value.c_str());
		else if (key == "base")
			m_base = atoi(value.c_str());
		else if (key == "scaleW")
			m_scaleW = atoi(value.c_str());
		else if (key == "scaleH")
			m_scaleH = atoi(value.c_str());
		else if (key == "pages")
			m_pages = atoi(value.c_str());
		else if (key == "packed")
			m_packed = atoi(value.c_str());
		else if (key == "alphaChnl")
			m_alphaChnl = atoi(value.c_str());
		else if (key == "redChnl")
			m_redChnl = atoi(value.c_str());
		else if (key == "greenChnl")
			m_greenChnl = atoi(value.c_str());
		else if (key == "blueChnl")
			m_blueChnl = atoi(value.c_str());
		else
//			debugOut << "BMCommon::parse => Unknown type:" << key.c_str() << std::endl;
			lprintfln("BMCommon::parse at line %d => Unknown type:%s/n",(int)i,key.c_str());
	}
}

/**
 * \brief BMPage::parse function
 * @param line, contains string contains whole line of text from .fnt file
 * chopped up in string array from the space delimited data.
 */

void BMPage::parse(std::vector<std::string> &line)
{
	std::string key,value;
	for(size_t i=1; i<line.size(); i++)
	{
		GetKeyValueFrom(line[i],key,value);
		if (key == "id")
			m_id = atoi(value.c_str());
		else if (key == "file")
			m_file = Utils::unQuote(value);
		else
			lprintfln("BMPage::parse at line %d => Unknown type:%s/n",(int)i,key.c_str());
	}
}

/**
 * \brief BMChar::parse function
 * @param line, contains string contains whole line of text from .fnt file
 * chopped up in string array from the space delimited data.
 */

void BMChar::parse(std::vector<std::string> &line)
{
	std::string key,value;
	for(size_t i=1; i<line.size(); i++)
	{
		GetKeyValueFrom(line[i],key,value);
//		lprintfln("BMCHAR::parse:%s : \'%s\'=\'%s\'",line[i].c_str(),key.c_str(),value.c_str());
		if (key == "id")
			m_id = atoi(value.c_str());
		else if (key == "x")
			m_x = atoi(value.c_str());
		else if (key == "y")
			m_y = atoi(value.c_str());
		else if (key == "width")
			m_width = atoi(value.c_str());
		else if (key == "height")
			m_height = atoi(value.c_str());
		else if (key == "xoffset")
			m_xoffset = atoi(value.c_str());
		else if (key == "yoffset")
			m_yoffset = atoi(value.c_str());
		else if (key == "xadvance")
			m_xadvance = atoi(value.c_str());
		else if (key == "page")
			m_page = atoi(value.c_str());
		else if (key == "chnl")
			m_chnl = atoi(value.c_str());
		else
			lprintfln("BMChar::parse at line %d => Unknown type:%s/n",(int)i,key.c_str());
	}
}

/**
 * \brief BMKerning::parse function
 * @param line, contains string contains whole line of text from .fnt file
 * chopped up in string array from the space delimited data.
 */
void BMKerning::parse(std::vector<std::string> &line)
{
	std::string key,value;
	for(size_t i=1; i<line.size(); i++)
	{
		GetKeyValueFrom(line[i],key,value);
		if (key == "first")
			m_first = atoi(value.c_str());
		else if (key == "second")
			m_second = atoi(value.c_str());
		else if (key == "amount")
			m_amount = atoi(value.c_str());
		else
			lprintfln("BMKerning::parse at line %d => Unknown type:%s/n",(int)i,key.c_str());
	}
}

/**
 * \brief LoadFontData.. load a .fnt file and parse
 * @param resource of an .fnt file
 * @return bool true/false, true = success, false = failed
 */
bool BMFont::LoadFontData(MAHandle resource)
{
	char delim = ' ';
	bool trim = true;
	ReadCSV csv;

	csv.load(resource,delim,trim);
	lprintfln("Info: BMFont::LoadFontData => loaded db size=%i",(int)csv.getDB().size());

	std::vector<std::vector<std::string> >& lineData = csv.getDB();
	std::string key,value;

	for(size_t i=0; i<lineData.size(); i++)			// array of lines
	{
		// Note beginning of each line there is a chapter tag.
		// 1. get header info..			"info"
		// 2. get common info header..	"common"
		// 3. get page ID..				"page"
		// 4. get count chars (items)	"chars"
		// 5. get char type array		"char"
		// 6. get count kernings (items)"kernings"
		// 7. get kerning				"kerning"
		std::vector<std::string> &line = lineData[i];
//		lprintfln("BMFont::LoadFontData => line %d => tokens %d",(int)i,(int)line.size());

		if (line.size() < 2)
		{
			lprintfln("INFO BMFont::LoadFontData => Skip line %d => tokens %d\n",(int)i,(int)line.size());
			continue;
		}


		ParseBMFont state = getBMType(line[0]);
//		lprintfln("Parsing BMFONT state=%d\n",(int)state);

		switch(state)
		{
			case PBM_INFO:
				m_info.parse(line);
				break;
			case PBM_COMMON:
				m_common.parse(line);
				break;
			case PBM_PAGE:
			{
				BMPage		bmpage;
				bmpage.parse(line);
				m_pages.push_back(bmpage);
				break;
			}
			case PBM_CHARS:
				m_nchars = GetValue(line[1]); lprintfln("BMFont::LoadFontData => PBM_CHARS n_kernings=%d\n",m_nchars);
				break;
			case PBM_CHAR:
			{
				BMChar	bmchar;
				bmchar.parse(line);
/*				lprintfln("BMFont::LoadFontData => PBM_CHAR m_id=%d\n",bmchar.m_id);
				lprintfln("BMChar.height=%d",bmchar.m_height);
				lprintfln("BMChar.width=%d",bmchar.m_width);
				lprintfln("BMChar.page=%d",bmchar.m_page);
				lprintfln("BMChar.xadvance=%d",bmchar.m_xadvance);
				lprintfln("BMChar.xoffset=%d",bmchar.m_xoffset);
				lprintfln("BMChar.x=%d",bmchar.m_x);
				lprintfln("BMChar.y=%d",bmchar.m_y);
				lprintfln("BMChar.yoffset=%d",bmchar.m_yoffset);
*/

				if ((m_chars.insert(Int_BMChar_Pair(bmchar.m_id, bmchar))).second == false)  //      [bmchar.m_id] = bmchar;
					lprintfln("BMFont::LoadFontData PBM_CHARS double entry detected");
				BMChar bmchar2 = m_chars[bmchar.m_id];
/*
				if (memcmp(&bmchar2,&bmchar,sizeof(BMChar)) == 0)
					lprintfln("Testing id = %d, OK",bmchar.m_id);
				else
					lprintfln("Testing id = %d, FAILED",bmchar.m_id);
	*/			break;
			}
			case PBM_KERNINGS:
				m_nkernings = GetValue(line[1]);
				lprintfln("BMFont::LoadFontData => PBM_KERNINGS n_kernings=%d\n",m_nkernings);
				break;
			case PBM_KERNING:
			{
				BMKerning	bmkerning;

//				lprintfln("BMFont::LoadFontData => PBM_KERNING %d",(int)line.size());
//				for(size_t i=0; i<line.size();++i)
//					lprintfln("BMFont::LoadFontData =>%d %s",(int)i,line[i].c_str());

				bmkerning.parse(line);

				std::hash_map<int,std::vector<BMKerning> >::iterator it;
/*
				lprintfln("BMKerning.first=%d",bmkerning.m_first);
				lprintfln("BMKerning.second=%d",bmkerning.m_second);
				lprintfln("BMKerning.amount=%d",bmkerning.m_amount);
*/

				it = m_kernings.find(bmkerning.m_first);

				if (it != m_kernings.end())
				{	// entry exists
					std::vector<BMKerning> &table = it->second;
					table.push_back(bmkerning);
//					lprintfln("%d entry, verifying=%d",table.size(),m_kernings[bmkerning.m_first].size());
				}
				else
				{	// first time entry not existing
					std::vector<BMKerning> table;
					table.push_back(bmkerning);
					m_kernings.insert(Int_Vector_BMKerning_Pair(bmkerning.m_first,table));
//					lprintfln("first entry");
				}

				std::vector<BMKerning> &table = m_kernings[bmkerning.m_first];
				BMKerning bmkerning2 = table[table.size()-1];
	/*			if (memcmp(&bmkerning,&bmkerning2,sizeof(BMKerning)) == 0 )
					lprintfln("BMKerning parsed OK");
				break;
					lprintfln("BMKerning parsed FAILED");
*/
				break;
			}
			default: // Note check last empty line what happens there?
			{
				lprintfln("BMFont::LoadFontData => Unknown type:%s\n",line[0].c_str());
				return false;
			}
		}
	}
	lprintfln("BMFont::LoadFontData => parsed %d chars, %d kernings\n",m_nchars,m_nkernings);

	return true;
}

/**
 * \brief ReleaseFontData...
 */

void BMFont::ReleaseFontData()
{
	// Release the font data array.
	m_chars.clear();
	m_kernings.clear();
	m_pages.clear();
	m_nchars = 0;
	m_nkernings = 0;
}

/**
 * \brief ReleaseTexture...
 */

void BMFont::ReleaseTexture()
{
	if (m_texStores.size())
	{
		std::vector<GLuint> textures;
		for(size_t i=0; i<m_texStores.size(); i++)
		{
			textures.push_back(m_texStores[i].m_texture);
		}

		// make sure that OpenGL gets this resource cleared.
		glDeleteTextures(textures.size(), &textures[0]);
		m_texStores.clear();
	}
}

/**
 * \brief GetTexture get texture after its index from array
 * @param i, index in array to lookup the texture
 * @return GLuint shader index.
 */
GLuint BMFont::GetTexture(int i)
{
	if (m_texStores.size() > 0)
		return m_texStores[i].m_texture;
	else
	{
		lprintfln("Error:BMFont::GetTexture(%d) = %d No textures Loaded! nTextures=%d",i,0,(int)m_texStores.size());
		return 0;
	}
}

/**
 * \brief getTextProperties calculates the vertex array from any given string containing this BMFont.
 * @param sentence char* string, to build a vertex table from
 * @param drawX, position in X
 * @param drawY, position in Y
 * @param scaleX, scale in X
 * @param scaleY, scale in Y
 * @param property, output vec2 => width , height of text.
 * @return string width
 */

float BMFont::getTextProperties(const char* sentence, float drawX, float drawY, float scaleX, float scaleY,TextProperty *property)
{
	int numLetters, i, letter, second;
	float start = drawX;
	float minY = 1000000.0f;
	float maxY = -1000000.0f;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		std::vector<BMKerning> 	*kernings 	= 0;
		BMChar 					*bc 		= 0;

		letter = ((unsigned int)sentence[i]);
		second = ((unsigned int)sentence[i+1]);

		std::hash_map<int, std::vector<BMKerning> >::iterator itk = m_kernings.find(letter);
		if(itk != m_kernings.end())
			kernings = &itk->second;


		std::hash_map<int,BMChar>::iterator itc = m_chars.find(letter);
		if(itc != m_chars.end())
			bc = &itc->second;

		if(bc == 0)	// unknown default
		{
			drawX = drawX + 3.0f;	// Could be other values.
//			lprintfln("deadspace =%c => %d\n", letter,letter);
		}
		else
		{
			float w,h,ox,oy,posX,posY,ax,ks;
			// positions and relative ofsets etc.
			ox = ((float)bc->m_xoffset) 	* scaleX;
			oy = ((float)bc->m_yoffset) 	* scaleY;
			w  = ((float)bc->m_width) 		* scaleX;
			h  = ((float)bc->m_height) 		* scaleY;
			ax = ((float)bc->m_xadvance)	* scaleX;
			ks = 0.0f;

			posX = drawX + ox;	// move offset position for char
			posY = drawY - oy;

			// Update the x location for drawing by the size of the letter and one pixel.
			if (kernings && second)	// second could point to a null term
			{
				for(size_t j=0;j<kernings->size(); j++)
				{
					BMKerning &kn = kernings->at(j);
					if (second == kn.m_second)
					{
						ks = (float)kn.m_amount * scaleX;
						break;
					}
				}
			}

			drawX = posX - ox + ax + ks;	// calculate out position of second char.

			if (posY > maxY)
				maxY = posY;
			if (posY-h < minY)
				minY = posY-h;
		}
	}

	property->sWidth = drawX-start;				// return the total width of the text
	property->sHeight = maxY - minY;				// return the height of the text.
	property->sLineHeight = m_common.m_lineHeight*scaleY;
	return property->sWidth;
}

/**
 * \brief BuildVertexArray buids the vertex array from any given string containing this BMFont.
 * @param vertexPtr vertex, pointer where the user provides to be filled, user also needs to have sufficient allocated space for this
 * @param sentence char* string, to build a vertex table from
 * @param drawX, position in X
 * @param drawY, position in Y
 * @param scaleX, scale in X
 * @param scaleY, scale in Y
 * @return string width
 */

float BMFont::BuildVertexArray(glm::vec4 * vertexPtr, const char* sentence, float drawX, float drawY, float scaleX, float scaleY)
{
	int numLetters, index, i, letter, second;
	float start = drawX;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;
	int nTri=0;

	// Draw each letter onto a quad.
	for(i=0; i<numLetters; i++)
	{
		std::vector<BMKerning> 	*kernings 	= 0;
		BMChar 					*bc 		= 0;

		letter = ((unsigned int)sentence[i]);
		second = ((unsigned int)sentence[i+1]);

//		lprintfln("letter %c (%d) in %s .. \n", letter,letter, sentence);

		std::hash_map<int, std::vector<BMKerning> >::iterator itk = m_kernings.find(letter);
		if(itk != m_kernings.end())
			kernings = &itk->second;
//		else
//			lprintfln("no kerning found =%c => %d\n", letter, letter);


		std::hash_map<int,BMChar>::iterator itc = m_chars.find(letter);
		if(itc != m_chars.end())
			bc = &itc->second;
//		else
//			lprintfln("no char found =%c => %d\n", letter,letter);

		// If the letter is a space then just move over three pixels.
		if(bc == 0)	// unknown default
		{
			drawX = drawX + 3.0f;	// Could be other values.
//			lprintfln("deadspace =%c => %d\n", letter,letter);
		}
		else
		{
			nTri++;
//			lprintfln("Building a quad with 2 triangles cnt=%d (%d) vertexPtr[%d]",nTri,2*nTri,index);
			// First triangle in quad.
			float tx,ty,tw,th,sw,sh,w,h,ox,oy,posX,posY,ax,ks;
			// positions and relative ofsets etc.

			ox = ((float)bc->m_xoffset) 	* scaleX;
			oy = ((float)bc->m_yoffset) 	* scaleY;
			w  = ((float)bc->m_width) 		* scaleX;
			h  = ((float)bc->m_height) 		* scaleY;
			ax = ((float)bc->m_xadvance)	* scaleX;
			ks = 0.0f;

			// texture coordinates. 0..1
			sw = (float)m_common.m_scaleW;
			sh = (float)m_common.m_scaleH;
			tx = ((float)bc->m_x)/sw;
			ty = ((float)bc->m_y)/sh;
			tw = ((float)bc->m_width)/sw;
			th = ((float)bc->m_height)/sh;

		//	lprintfln("tx,ty(%f,%f) w,h(%f,%f)",tx,ty,tw,th);

			posX = drawX + ox;	// move offset position for char
			posY = drawY - oy;

			vertexPtr[index] = glm::vec4(posX, posY, tx, ty);  // Top left.
			index++;

			vertexPtr[index] = glm::vec4(posX, (posY-h), tx, (ty+th));  // Bottom left.
			index++;

			vertexPtr[index] = glm::vec4((posX+w), (posY-h), tx+tw, (ty+th));  // Bottom right.
			index++;

			// Second triangle in quad.
			vertexPtr[index] = glm::vec4(posX, posY, tx, ty);  // Top left.
			index++;

			vertexPtr[index] = glm::vec4((posX+w), (posY-h), tx+tw, (ty+th));  // Bottom right.
			index++;

			vertexPtr[index] = glm::vec4(posX+w, posY, tx+tw, ty);  // Top right.
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.

			if (kernings && second)	// second could point to a null term
			{
				for(size_t j=0;j<kernings->size(); j++)
				{
					BMKerning &kn = kernings->at(j);
					if (second == kn.m_second)
					{
						ks = (float)kn.m_amount * scaleX;
	//					lprintfln("added kerning=%f for = %c letter=\'%c\'\n",ks, j, second);
						break;
					}
				}
			}
	//		else
	//			lprintfln("No kerning detected for =%c\n",letter);

			//      start posX + width + advanceX + kerning (negative values)
			drawX = posX - ox + ax + ks;
		}
	}

	return drawX-start;
}
