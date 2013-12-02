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

#ifndef TEXTURE_H
#define	TEXTURE_H

#ifdef WIN32
#include <Windows.h>
#endif

#include <string>

#include <GL/glew.h>
#include "../Include/ImageMagick/Magick++.h"


/**
Uses ImageMagick, a free software library that supports many image types 
and is portable across several operating systems to reading image files.

*/
class Texture
{
public:

	/**
	When creating a Texture object you will need to specify a target (we use GL_TEXTURE_2D) 
	and file name. After that you call the Load() function.
	*/
    Texture(GLenum TextureTarget, const std::string& FileName);

	/**
	Uses ImageMagick to load the texture from file and prepare it in memory to be loaded into 
	OpenGL.
	*/
    bool Load();

	/**
	Before each draw call is made we need to bind the texture object we want to one of the 
	texture units so that it will be sampled from in the fragment shader. This function takes 
	the texture unit enum (GL_TEXTURE0, GL_TEXTURE1, etc) as a parameter. It makes it active 
	using glActiveTexture() and then binds the texture object to it. This object will remain 
	bound to this texture unit until the next call to Texture::Bind() is made with the same 
	texture unit.
	*/
    void Bind(GLenum TextureUnit);

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    Magick::Image* m_pImage;
    Magick::Blob m_blob;
};


#endif	/* TEXTURE_H */

