//
// Created by Mono on 2024/12/30.
//

#ifndef ZYGISK_IL2CPPDUMPER_MENU_H
#define ZYGISK_IL2CPPDUMPER_MENU_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_android.h"
#include <time.h>
#include "font.h"
#include "il2cpp_hook.h"
int glHeight, glWidth;bool setupimg = false;bool show_menu = true;bool login = true;
const char* ProjectName = "Mono";float baseFontSize = 14.0f;
float iconFontSize = baseFontSize * 2.0f / 3.0f;
enum Headers {
    PLAYER,
    ESP,
    MISC,
    ABOUT,
    HIDE,
    HEADERS_COUNT
};
ImFont* regular;
ImFont* medium;
ImFont* bold;
ImFont* title;
ImFont* icons;

void SetUpColors(ImGuiStyle& style, ImVec4* colors)
{
    ImColor hovered = { 31, 110, 171 };
    ImColor Transparented = { 0, 0, 0, 255 };
    colors[ImGuiCol_WindowBg] = ImColor(20, 23, 25);
    colors[ImGuiCol_ChildBg] = ImColor(24, 28, 30);
    colors[ImGuiCol_Text] = ImColor(255, 255, 255);

    //Header
    colors[ImGuiCol_Header] = ImColor(30, 138, 200);
    colors[ImGuiCol_HeaderHovered] = hovered;
    colors[ImGuiCol_HeaderActive] = ImColor(30, 116, 215);

    //buttons
    colors[ImGuiCol_Button] = ImColor(25, 145, 215);
    colors[ImGuiCol_ButtonHovered] = hovered;
    colors[ImGuiCol_ButtonActive] = ImColor(100, 161, 222);

    //checkboxes
    colors[ImGuiCol_CheckMark] = ImColor(0, 0, 0);
    colors[ImGuiCol_FrameBg] = ImColor(25, 158, 215, 200);
    colors[ImGuiCol_FrameBgActive] = ImColor(25, 164, 215);
    colors[ImGuiCol_FrameBgHovered] = ImColor(20, 212, 250);

    colors[ImGuiCol_Border] = Transparented;

    style.WindowRounding = 10.0f;
    style.FrameRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 2.3f;
    style.TabRounding = 2.3f;

    //style.WindowMinSize = { 600,300 };
    style.ChildRounding = 5.0f;
}
void SetupImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    io.DisplaySize = ImVec2((float) glWidth, (float) glHeight);
    ImGui_ImplOpenGL3_Init("#version 100");
    io.Fonts->AddFontDefault();
    SetUpColors(style, colors);
    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;
    static const ImWchar ranges[] =
            {
                    0x0020, 0x00FF, // Basic Latin + Latin Supplement
                    0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
                    0x2DE0, 0x2DFF, // Cyrillic Extended-A
                    0xA640, 0xA69F, // Cyrillic Extended-B
                    0xE000, 0xE226, // icons
                    0
            };
    ImFontConfig icontfont_config;
    icontfont_config.MergeMode = true;
    icontfont_config.GlyphMinAdvanceX = iconFontSize;
    icontfont_config.PixelSnapH = true;

    regular = io.Fonts->AddFontFromMemoryTTF(RobotoRegular, sizeof(RobotoRegular), 15.0f, &font_config, ranges);
    medium  = io.Fonts->AddFontFromMemoryTTF(RobotoMedium, sizeof(RobotoMedium), 15.0f, &font_config, ranges);
    bold    = io.Fonts->AddFontFromMemoryTTF(RobotoBold, sizeof(RobotoBold), 15.0f, &font_config, ranges);
    title   = io.Fonts->AddFontFromMemoryTTF(RobotoBold, sizeof(RobotoBold), 25.0f, &font_config, ranges);
    io.FontDefault = regular;

}

void DrawMenu()
{
    static Headers tab = PLAYER;
    const char* tab_names[] = { "PLAYER", "ESP", "MISC", "ABOUT","HIDE" };
    // Get Window Size
    ImVec2 wndSize = ImGui::GetIO().DisplaySize;

    // min and max size for the main window
    ImVec2 minSize(600, 300);
    ImVec2 maxSize(wndSize);
    // Fix the next window size
    if(login)
    {
        if(show_menu)
        {
            ImGui::SetNextWindowSizeConstraints(minSize, maxSize);
            if (ImGui::Begin("Mono", nullptr, /*ImGuiWindowFlags_NoDecoration |*/ ImGuiWindowFlags_NoTitleBar))
            {
                ImVec2 pos = ImGui::GetWindowPos();
                ImVec2 size = ImGui::GetWindowSize();
                ImVec2 wndSize = ImGui::GetWindowSize();
                ImGuiStyle style = ImGui::GetStyle();
                ImDrawList* draw = ImGui::GetWindowDrawList();

                float minWidth = 120;

                ImVec2 mainLeftSize = ImVec2(ImGui::GetContentRegionAvail().x * 0.16f, ImGui::GetContentRegionAvail().y);
                mainLeftSize.x = ImClamp(mainLeftSize.x, minWidth, wndSize.x);

                ImGui::BeginChild("MainLeft", mainLeftSize, 0);
                {

                    ImVec2 leftTopChildSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.16f);
                    ImGui::BeginChild("LeftSide1", leftTopChildSize, false);

                    ImGui::PushFont(title);

                    ImVec2 textSize = ImGui::CalcTextSize(ProjectName);

                    float posX = (leftTopChildSize.x - textSize.x) * 0.5f;
                    float posY = (leftTopChildSize.y - textSize.y) * 0.5f;

                    ImGui::SetCursorPos(ImVec2(posX, posY));

                    ImGui::TextColored(ImColor(84, 160, 227), "%s",ProjectName);

                    ImGui::PopFont();
                    ImGui::EndChild();


                    if (ImGui::BeginChild("LeftSide2", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), 0))
                    {
                        for (unsigned int i = 0; i < HEADERS_COUNT; ++i)
                        {
                            bool selected = (tab == static_cast<Headers>(i));
                            auto windowvars = ImGuiStyleVar_SelectableTextAlign;

                            ImGui::PushStyleVar(windowvars, ImVec2(0.5f, 0.5f));

                            if (ImGui::Selectable(tab_names[i], &selected, 0, ImVec2(ImGui::GetContentRegionAvail().x, 24)))
                            {
                                tab = static_cast<Headers>(i);
                            }
                            ImGui::PopStyleVar();

                            if (selected)
                                ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();
            }
            ImGui::SameLine(0);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));

            ImGui::BeginChild("RightSide", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), 0);

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Indent(10.0f);

            switch (tab)
            {
                case PLAYER:

                    //NP
                    ImGui::Checkbox("NPMax", &writedata.isNP);

                    //CD
                    ImGui::Checkbox("CD", &writedata.isCD);

                    //Kill
                    ImGui::Checkbox("Kill", &writedata.kill);


                    //HP
                    ImGui::Checkbox("HP", &writedata.isHp);
                    if(writedata.isHp)
                    {
                        ImGui::BeginChild(2, ImVec2(0, 80), 1);
                        ImGui::Text("HP Multiplier");
                        ImGui::SliderInt("## HP Multiplier", &writedata.Hp, 1, 100);
                        ImGui::EndChild();
                    }


                    break;
                case ESP:
                    ImGui::Text("in dev.");
                    break;
                case MISC:
                    ImGui::Text("in dev.");
                    break;
                case ABOUT:
                    ImGui::Text("The Void Burrowing Pig qwq, so void-like!");
                    break;
                case HIDE:
                    tab = PLAYER;
                    show_menu = false;
                    break;
            }

            ImGui::Unindent();
            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::EndChild();
            ImGui::PopStyleVar();
        }
        else
        {
            ImGui::SetNextWindowSize(ImVec2(55, 40));
            if(ImGui::Begin("Mono", nullptr, /*ImGuiWindowFlags_NoDecoration |*/ ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize))
            {
                if(ImGui::Button("-----"))
                {
                    show_menu = true;
                }
            }
        }
    }
    else
    {
        ImGui::SetNextWindowSizeConstraints(minSize, maxSize);
        if (ImGui::Begin("Mono", nullptr, /*ImGuiWindowFlags_NoDecoration |*/ ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::Text("卡密：");ImGui::SameLine(20);
        }
    }
    ImGui::End(); // Render end
}



EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    if (!setupimg)
    {
        SetupImgui();
        setupimg = true;
    }
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    DrawMenu();
    writeSharedDataToFile(path.c_str(),&writedata);
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

#endif //ZYGISK_IL2CPPDUMPER_MENU_H
