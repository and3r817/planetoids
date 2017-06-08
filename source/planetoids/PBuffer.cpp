#include "PBuffer.hpp"

namespace planetoids
{
#define	MAX_ATTRIBS		20
#define	MAX_PFORMATS	20

	template <typename T>
	class	AttrList
	{
		T	attrs [2*MAX_ATTRIBS];
		int	numAttrs;
	public:
		AttrList ()
		{
			numAttrs = 0;

			clear ();
		}

		void	clear ()
		{
			for ( int i = 0; i < 2*MAX_ATTRIBS; i++ )
				attrs [i] = 0;
		}

		void	add ( int attr, T value )
		{
			attrs [2*numAttrs    ] = attr;
			attrs [2*numAttrs + 1] = value;
			numAttrs++;
		}

		int	getNumAttrs () const
		{
			return numAttrs;
		}

		const T * getAttrs () const
		{
			return attrs;
		}
	};

	PBuffer :: PBuffer  ( int theWidth, int theHeight, int theMode, bool theShareObjects )
	{
		width        = theWidth;
		height       = theHeight;
		mode         = theMode;
		shareObjects = theShareObjects;
		hBuffer      = NULL;
		hdc          = NULL;
		hGlRc        = NULL;
		saveHdc      = NULL;
		saveHglrc    = NULL;
	}

	PBuffer :: ~PBuffer ()
	{
		clear ();
	}

	void	PBuffer :: clear ()
	{
		if ( hBuffer != NULL )
		{
			wglDeleteContext       ( hGlRc );
			wglReleasePbufferDCARB ( hBuffer, hdc );
			wglDestroyPbufferARB   ( hBuffer );
		}
	}

	bool	PBuffer :: create ()
	{
		HDC		hCurDc = wglGetCurrentDC      ();
		HGLRC	hCurRc = wglGetCurrentContext ();
		int		format;

		// check for extension support & initialization
		if ( wglChoosePixelFormatARB == NULL || wglCreatePbufferARB  == NULL ||
			wglGetPbufferDCARB      == NULL || wglQueryPbufferARB   == NULL  ||
			wglReleasePbufferDCARB  == NULL || wglDestroyPbufferARB == NULL )
			return false;

		if ( (mode & modeTexture1D) || (mode & modeTexture2D) || (mode & modeTextureCubeMap) )
			if ( wglBindTexImageARB == NULL || wglReleaseTexImageARB == NULL || wglSetPbufferAttribARB == NULL )
				return false;

		// query for pixel format
		AttrList <int>		intAttrs;
		AttrList <float>	floatAttrs;

		intAttrs.add ( WGL_SUPPORT_OPENGL_ARB,  GL_TRUE );
		intAttrs.add ( WGL_DRAW_TO_PBUFFER_ARB, GL_TRUE );
		intAttrs.add ( WGL_PIXEL_TYPE_ARB,      WGL_TYPE_RGBA_ARB );
		intAttrs.add ( WGL_RED_BITS_ARB,        8 );
		intAttrs.add ( WGL_GREEN_BITS_ARB,      8 );
		intAttrs.add ( WGL_BLUE_BITS_ARB,       8 );
		intAttrs.add ( WGL_DOUBLE_BUFFER_ARB, (mode & modeDouble ? GL_TRUE : GL_FALSE ) );

		if ( mode & modeAlpha )
			intAttrs.add ( WGL_ALPHA_BITS_ARB, 8 );

		if ( mode & modeDepth )
			intAttrs.add ( WGL_DEPTH_BITS_ARB, 24 );

		if ( mode & modeStencil )
			intAttrs.add ( WGL_STENCIL_BITS_ARB, 8 );

		if ( mode & modeAccum )
			intAttrs.add ( WGL_ACCUM_BITS_ARB, 32 );

		if ( (mode & modeTexture1D) || (mode & modeTexture2D) || (mode & modeTextureCubeMap) )
			if ( mode & modeAlpha )
				intAttrs.add ( WGL_BIND_TO_TEXTURE_RGBA_ARB, GL_TRUE );
			else
				intAttrs.add ( WGL_BIND_TO_TEXTURE_RGB_ARB, GL_TRUE );

		int			pixelFormats [MAX_PFORMATS];
		unsigned	numFormats = 0;

		if ( !wglChoosePixelFormatARB ( hCurDc, intAttrs.getAttrs (), floatAttrs.getAttrs (),
			MAX_PFORMATS, pixelFormats, &numFormats ) )
			return false;

		if ( numFormats < 1 )
			return false;

		format = pixelFormats [0];

		AttrList <int>	props;

		if ( (mode & modeTexture1D) || (mode & modeTexture2D) )
			if ( mode & modeAlpha )
				props.add ( WGL_TEXTURE_FORMAT_ARB,  WGL_TEXTURE_RGBA_ARB );
			else
				props.add ( WGL_TEXTURE_FORMAT_ARB,  WGL_TEXTURE_RGB_ARB );

		if ( mode & modeTexture1D )
			props.add ( WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_1D_ARB );
		else
			if ( mode & modeTexture2D )
				props.add ( WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB );
			else
				if ( mode & modeTextureCubeMap )
					props.add ( WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_CUBE_MAP_ARB );

		if ( mode & modeTextureMipmap )
			props.add ( WGL_MIPMAP_TEXTURE_ARB, GL_TRUE );

		hBuffer = wglCreatePbufferARB ( hCurDc, format, width, height, props.getAttrs () );

		if ( hBuffer == NULL )
			return false;

		hdc = wglGetPbufferDCARB ( hBuffer );

		if ( hdc == NULL )
			return false;

		hGlRc = wglCreateContext ( hdc );

		if ( shareObjects )
			wglShareLists ( hCurRc, hGlRc );

		// get real size
		wglQueryPbufferARB ( hBuffer, WGL_PBUFFER_WIDTH_ARB,  &width );
		wglQueryPbufferARB ( hBuffer, WGL_PBUFFER_HEIGHT_ARB, &height );

		return true;
	}

	bool	PBuffer :: makeCurrent ()
	{
		saveHdc   = wglGetCurrentDC      ();	// save previous target
		saveHglrc = wglGetCurrentContext ();

		return wglMakeCurrent ( hdc, hGlRc ) != 0;
	}

	bool	PBuffer :: restoreCurrent ()
	{
		if ( saveHdc == NULL || saveHglrc == NULL )
			return false;

		bool	result = wglMakeCurrent ( saveHdc, saveHglrc );

		saveHdc   = NULL;
		saveHglrc = NULL;

		return result;
	}

	bool	PBuffer :: isLost () const
	{
		int	lost = 0;

		wglQueryPbufferARB ( hBuffer, WGL_PBUFFER_LOST_ARB, &lost );

		return lost != 0;
	}

	bool	PBuffer :: bindToTexture ( int buf )
	{
		return wglBindTexImageARB ( hBuffer, buf ) != GL_FALSE;
	}

	bool	PBuffer :: unbindFromTexture ( int buf )
	{
		return wglReleaseTexImageARB ( hBuffer, buf ) != GL_FALSE;
	}

	bool	PBuffer :: setAttr ( int attr, int value )
	{
		AttrList <int>	attrs;

		attrs.add ( attr, value );

		return wglSetPbufferAttribARB ( hBuffer, attrs.getAttrs () ) != GL_FALSE;
	}

	bool	PBuffer :: setCubemapSide ( int side )
	{
		if ( (mode & modeTextureCubeMap) == 0 )
			return false;

		glFlush ();

		return setAttr ( WGL_CUBE_MAP_FACE_ARB, side );
	}


	void PBuffer :: printLastError()
	{
		DWORD err = GetLastError();

		switch ( err )
		{
		case ERROR_INVALID_PIXEL_FORMAT:
			fprintf ( stderr, "Error:  ERROR_INVALID_PIXEL_FORMAT\n" );
			break;

		case ERROR_NO_SYSTEM_RESOURCES:
			fprintf ( stderr, "Error:  ERROR_NO_SYSTEM_RESOURCES\n ");
			break;

		case ERROR_INVALID_DATA:
			fprintf ( stderr, "Error:  ERROR_INVALID_DATA\n" );
			break;

		case ERROR_INVALID_WINDOW_HANDLE:
			fprintf ( stderr, "Error:  ERROR_INVALID_WINDOW_HANDLE\n" );
			break;

		case ERROR_RESOURCE_TYPE_NOT_FOUND:
			fprintf ( stderr, "Error:  ERROR_RESOURCE_TYPE_NOT_FOUND\n" );
			break;

		case ERROR_SUCCESS:
			// no error
			break;

		default:
			LPVOID msgBuf;

			FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				err,
				MAKELANGID ( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
				(LPTSTR) &msgBuf,
				0,
				NULL );

			fprintf  ( stderr, "Error %d: %s\n", err, msgBuf );
			LocalFree( msgBuf );
			break;
		}

		SetLastError ( 0 );
	}
}