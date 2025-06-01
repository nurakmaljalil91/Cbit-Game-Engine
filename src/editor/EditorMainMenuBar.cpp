/**
 * @file    EditorMainMenuBar.cpp
 * @brief   EditorMainMenuBar class implementation file
 * @details This file contains the implementation of the EditorMainMenuBar class which is responsible
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#include "EditorMainMenuBar.h"
#include "Application.h"
#include "Editor.h"
#include "EditorThemes.h"
#include "../utilities/ForkAwesomIcon.h"

EditorMainMenuBar::EditorMainMenuBar(Editor *editor): _editor(editor) {
    _fileDialogConfig.path = ".";
}


void EditorMainMenuBar::render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Project")) {
                _pendingAction = FileDialogAction::NewProject;
                ImGuiFileDialog::Instance()->OpenDialog(
                    "ChooseProjectFolderDialog", "Choose Project Folder", nullptr, _fileDialogConfig);
            }
            if (ImGui::MenuItem("Open Project")) {
                _pendingAction = FileDialogAction::OpenProject;
                ImGuiFileDialog::Instance()->OpenDialog(
                    "OpenProjectDialog", "Open Project File", ".json", _fileDialogConfig);
            }
            if (ImGui::MenuItem(ICON_FOA_SAVE " Save Project", "Ctr+S")) {
                _editor->getApplication()->getProjectManager().saveProject();
                _editor->getApplication()->getSceneManager().saveScenesToProject(
                    _editor->getApplication()->getProjectManager().getProjectPath());
            }
            if (ImGui::MenuItem("Save Project As...")) {
                _pendingAction = FileDialogAction::SaveProjectAs;
                ImGuiFileDialog::Instance()->OpenDialog(
                    "SaveProjectAsDialog", "Save Project As", ".json", _fileDialogConfig);
            }
            // Separator
            ImGui::Separator();
            handleSettingsMenuDialog();
            if (ImGui::MenuItem("Exit")) {
                // TODO: Handle exit action
                _editor->getApplication()->exit();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            // TODO: Add edit menu items
            ImGui::MenuItem("Undo");
            ImGui::MenuItem("Redo");
            ImGui::MenuItem("Cut");
            ImGui::MenuItem("Copy");
            ImGui::MenuItem("Paste");
            ImGui::MenuItem("Delete");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            // TODO: Add view menu items
            ImGui::MenuItem("Toggle Fullscreen");
            ImGui::MenuItem("Toggle Console");
            ImGui::MenuItem("Toggle Asset Manager");
            ImGui::MenuItem("Toggle Game Viewport");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            // TODO: Add help menu items
            ImGui::MenuItem("Documentation");
            ImGui::MenuItem("About");
            ImGui::MenuItem("Check for Updates");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void EditorMainMenuBar::handleProjectMenuDialog() {
    // Handle New Project - folder select
    if (ImGuiFileDialog::Instance()->Display("ChooseProjectFolderDialog")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string folderPath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // Ask for the project name
            ImGui::OpenPopup("EnterProjectNamePopup");
            _pendingPath = folderPath;
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // Popup to get the project name for New Project
    if (ImGui::BeginPopupModal("EnterProjectNamePopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char projNameBuf[128] = "";
        ImGui::InputText("Project Name", projNameBuf, sizeof(projNameBuf));
        if (ImGui::Button("Create")) {
            _editor->getApplication()->getProjectManager().newProject(_pendingPath, projNameBuf);
            projNameBuf[0] = '\0';
            _pendingPath.clear();
            ImGui::CloseCurrentPopup();
            _pendingAction = FileDialogAction::None;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            projNameBuf[0] = '\0';
            _pendingPath.clear();
            ImGui::CloseCurrentPopup();
            _pendingAction = FileDialogAction::None;
        }
        ImGui::EndPopup();
    }

    // Handle Open Project - file select
    if (ImGuiFileDialog::Instance()->Display("OpenProjectDialog")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            const std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            _editor->getApplication()->getProjectManager().loadProject(filePath);
        }
        ImGuiFileDialog::Instance()->Close();
        _pendingAction = FileDialogAction::None;
    }

    // Handle Save Project As - file select
    if (ImGuiFileDialog::Instance()->Display("SaveProjectAsDialog")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            _editor->getApplication()->getProjectManager().saveProjectAs(filePath);
            _editor->getApplication()->getSceneManager().saveScenesToProject(filePath);
        }
        ImGuiFileDialog::Instance()->Close();
        _pendingAction = FileDialogAction::None;
    }
}

void EditorMainMenuBar::handleSettingsMenuDialog() const {
    if (ImGui::BeginMenu("Settings")) {
        if (ImGui::BeginMenu("Themes")) {
            const auto &themeName = _editor->getThemeName();
            for (const auto &theme: EditorThemes::themeList) {
                if (ImGui::MenuItem(theme.name, nullptr, themeName == theme.name)) {
                    _editor->setTheme(theme.name);
                    theme.use();
                    EditorThemes::saveThemeToFile(themeName);
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Fonts")) {
            // List available fonts
            for (const auto &[fontName, font]: _editor->getFonts()) {
                if (ImGui::MenuItem(fontName.c_str(), nullptr, _editor->getFontName() == fontName)) {
                    _editor->setFontName(fontName);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
}
