#include "nbl/video/IAPIConnection.h"

#include "CFileSystem.h"
#include "nbl/builtin/common.h"

namespace nbl {
namespace video
{

// Functions defined in connections' .cpp files
// (i dont want to have all backends in single translation unit)
// as a result, if one wants to turn off compilation of whole backend, one can just remove corresponding API connection's .cpp from build
core::smart_refctd_ptr<IAPIConnection> createOpenGLConnection(SDebugCallback* dbgCb);
core::smart_refctd_ptr<IAPIConnection> createOpenGLESConnection(SDebugCallback* dbgCb);


core::smart_refctd_ptr<IAPIConnection> IAPIConnection::create(E_API_TYPE apiType, uint32_t appVer, const char* appName, SDebugCallback* dbgCb)
{
    switch (apiType)
    {
    case EAT_OPENGL:
        return createOpenGLConnection(dbgCb);
    case EAT_OPENGL_ES:
        return createOpenGLESConnection(dbgCb);
    //case EAT_VULKAN:
        //
    default:
        return nullptr;
    }
}

IAPIConnection::IAPIConnection()
{
    //! This variable tells us where the directory holding "nbl/builtin/" is if the resources are not embedded
    /** For shipping products to end-users we recommend embedding the built-in resources to avoid a plethora of
    "works on my machine" problems, as this method is not 100% cross platform, i.e. if the engine's headers'
    install directory is different between computers then it will surely not work.*/
    std::string builtinResourceDirectoryPath =
#ifdef _NBL_BUILTIN_PATH_AVAILABLE
        builtin::getBuiltinResourcesDirectoryPath();
#else
        "";
#endif
    m_fs = core::make_smart_refctd_ptr<io::CFileSystem>(std::move(builtinResourceDirectoryPath));

    m_GLSLCompiler = core::make_smart_refctd_ptr<asset::IGLSLCompiler>(m_fs.get());
}

}
}