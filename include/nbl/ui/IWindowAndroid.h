#ifndef __NBL_I_WINDOW_ANDROID_H_INCLUDED__
#define __NBL_I_WINDOW_ANDROID_H_INCLUDED__

#include "nbl/ui/IWindow.h"

#ifdef _NBL_PLATFORM_ANDROID_

#include <android/native_window.h>

namespace nbl {
namespace ui
{

class IWindowAndroid : public IWindow
{
protected:
    virtual ~IWindowAndroid() = default;

public:
    using IWindow::IWindow;

    using native_handle_t = struct ANativeWindow*;

    virtual native_handle_t getNativeHandle() const = 0;
};

}
}

#endif // _NBL_PLATFORM_ANDROID_

#endif
