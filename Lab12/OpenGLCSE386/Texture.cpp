/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "texture.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
    m_pImage        = NULL;
}

bool Texture::Load()
{
    try {
		// Instantiate a class member with the type Magic::Image using the texture file name.
        m_pImage = new Magick::Image(m_fileName);

		// Write the image into a Magick::Blob object using the RGBA (red, green, blue and 
		// alpha channels) format.
        m_pImage->write(&m_blob, "RGBA");
    }
    catch (Magick::Error& Error) {
        std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
        return false;
    }

	// Generates a single texture object and places the handle in the GLuint array pointer 
    glGenTextures(1, &m_textureObj);

	// Bind the texture object.
    glBindTexture(m_textureTarget, m_textureObj);

	// Load the main part of the texture object, that is, the texture data itself.
	// Second parameter is the LOD, or Level-Of-Detail. 0 is specified for the highest resolution mipmap.
	// Third parameter is the internal format in which OpenGL will store the texture.
	// Four and fifth parameters are the width and height of the texture in texels.
	// Fifth parameter is the border. Here there will be no border.
	// Last three parameters specify the source of the incoming texture data. The parameters are format, type and memory address. 
	//		Format tells  the number of channels and needs to match the BLOB in memory.
	//		Type describes the core data type that we have per channel. 
	//		Memory address the location of the actual data and is supplied here by the Blob::data() function.
    glTexImage2D(m_textureTarget, 0, GL_RGB, m_pImage->columns(), m_pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());

	// The filter type determines how to handle the case of magnifying or minifying the texture to match the 
	// proportion of the triangle. When the rasterized triangle is larger than the texture (e.g. it is very 
	// close to the camera) we may have several pixels covered by the same texel (magnification). When it is 
	// smaller (e.g. very far from the camera) several texels are covered by the same pixel (minification). 
	// Here, teh linear interpolation filter type is selected for both cases. 
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
