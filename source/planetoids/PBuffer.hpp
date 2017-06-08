#pragma once

#ifndef _PBUFFER_HPP
#define _PBUFFER_HPP

#include "BufferObject.hpp"
#include "Error.hpp"
#include "Platform.hpp"
#include "Texture2D.hpp"

namespace planetoids
{
	class	PBuffer
	{
	protected:
		HDC			hdc;
		HGLRC		hGlRc;
		HPBUFFERARB	hBuffer;
		int			width;
		int			height;
		int			mode;
		bool		shareObjects;
		HDC			saveHdc;					// save values from from makeCurrent
		HGLRC		saveHglrc;

	public:
		PBuffer  ( int theWidth, int theHeight, int theMode = modeAlpha | modeDepth | modeStencil,
			bool theShareObjects = true );
		~PBuffer ();

		int	getWidth () const
		{
			return width;
		}

		int	getHeight () const
		{
			return height;
		}

		bool	getShareObjects () const
		{
			return shareObjects;
		}

		int	getMode () const
		{
			return mode;
		}

		void	clear  ();
		bool	create ();

		// set attribute (like WGL_TEXTURE_CUBE_MAP*_ARB)
		bool	setAttr ( int attr, int value );

		bool	makeCurrent ();				// direct rendering to this pbuffer

		bool	restoreCurrent ();			// direct rendering to prev. target

		bool	isLost () const;			// whether p-buffer is lost due to mode switch

		// bind pbuffer to previously bound texture
		bool	bindToTexture     ( int buf = WGL_FRONT_LEFT_ARB );

		// unbind from texture (release)
		bool	unbindFromTexture ( int buf = WGL_FRONT_LEFT_ARB );

		// set specific cubemap side, call glFlush after side is done
		bool	setCubemapSide    ( int side );

		void	printLastError ();

		enum	Mode
		{
			modeAlpha          = 1,
			modeDepth          = 2,
			modeStencil        = 4,
			modeAccum          = 8,
			modeDouble         = 16,
			modeTexture1D      = 32,
			modeTexture2D      = 64,
			modeTextureCubeMap = 128,
			modeTextureMipmap  = 256
		};
	};
}

#endif
