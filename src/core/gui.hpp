#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "imgui.h"
#include "backend/imgui_impl_glfw.h"
#include "backend/imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
    #include <GLES2/gl2.h>
#endif

namespace ARAS
{
    class GUI
    {
        public:
            explicit GUI();
            ~GUI();

            virtual bool RunGUI();

        private:
    };
}
#endif