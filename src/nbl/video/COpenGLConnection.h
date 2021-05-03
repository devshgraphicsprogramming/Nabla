#ifndef __NBL_C_OPENGL_CONNECTION_H_INCLUDED__
#define __NBL_C_OPENGL_CONNECTION_H_INCLUDED__

#include "nbl/video/COpenGL_Connection.h"
#include "nbl/video/COpenGLPhysicalDevice.h"

namespace nbl {
namespace video
{

using COpenGLConnection = COpenGL_Connection<COpenGLPhysicalDevice, EAT_OPENGL>;

}
}

#endif
