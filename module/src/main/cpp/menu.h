//
// Created by lbert on 4/15/2023.
//

#ifndef ZYGISK_MENU_TEMPLATE_MENU_H
#define ZYGISK_MENU_TEMPLATE_MENU_H

using namespace ImGui;
#include "font.h"
SharedData data;
void writeSharedDataToFile(const char* filename, const SharedData* data) {
    FILE* file = fopen(filename, "wb"); // 打开文件用于二进制写入
    if (file == NULL) {
        LOGI("Error opening file");
        return;
    }

    // 写入数据到文件
    fwrite(data, sizeof(SharedData), 1, file);

    // 关闭文件
    fclose(file);
}

void DrawMenu()
{
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    {
        ImGuiStyle& Style = ImGui::GetStyle();
        Style.ChildRounding=8.0f;
        Style.FrameRounding = 11.0f;
        Style.GrabRounding = 11.0f;
        Style.ScrollbarSize=20.0f;
        Style.WindowTitleAlign=ImVec2(0.5f,0.5f);
        Begin(OBFUSCATE("Mod"));
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyScroll ;
        if (BeginTabBar("Menu", tab_bar_flags)) {
            if (BeginTabItem(OBFUSCATE("功能"))) {
                // here menu stuff, remove test btw
                // ImGui::Checkbox(OBFUSCATE("This is a checkbox"), &test);
                SliderInt("Atk",&data.AtkValue,1,100);
                SliderInt("Def",&data.DefValue,1,100);
                SliderInt("AtkSpeed",&data.AttackSpeedValue,1,100);
                Checkbox(OBFUSCATE("无限sp"), &data.isSp);
                Checkbox(OBFUSCATE("无消耗 "), &data.isCost);
                Checkbox(OBFUSCATE("全挡住"), &data.is_blockCnt);
                Checkbox(OBFUSCATE("不死"), &data.isUndeadable);
                EndTabItem();
            }
            EndTabBar();
        }
        Patches();
        End();
    }
}

void SetupImgui() {
    IMGUI_CHECKVERSION();
    CreateContext();
    ImGuiIO &io = GetIO();
    io.DisplaySize = ImVec2((float) glWidth, (float) glHeight);
    ImGui_ImplOpenGL3_Init("#version 100");
    StyleColorsDark();
    GetStyle().ScaleAllSizes(7.0f);
    io.Fonts->AddFontFromMemoryTTF((void*)myfont_data, myfont_size, 30.0f,NULL,io.Fonts->GetGlyphRangesChineseFull());
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
    ImGuiIO &io = GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    NewFrame();
    DrawMenu();
    writeSharedDataToFile(path.c_str(),&data);
    EndFrame();
    Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}


#endif //ZYGISK_MENU_TEMPLATE_MENU_H
