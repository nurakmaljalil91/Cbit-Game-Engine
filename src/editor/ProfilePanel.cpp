/**
 * @file    ProfilePanel.cpp
 * @brief   ProfilePanel class implementation file
 * @details This file contains the implementation of the ProfilePanel class which is responsible
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#include "ProfilePanel.h"
#include "Editor.h"

ProfilePanel::ProfilePanel(Editor *editor): _editor(editor) {
}

void ProfilePanel::render() const {
    ImGui::Begin("Profile");
    ImGui::Text("FPS: %.1f", _editor->getFPS());
    ImGui::Text("Build Version: %s", _editor->getBuildVersion().c_str());
    ImGui::End();
}
