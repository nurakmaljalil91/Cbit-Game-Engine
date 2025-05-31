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

EditorMainMenuBar::EditorMainMenuBar(Editor *editor): _editor(editor) {
    _fileDialogConfig.path = ".";
}

void EditorMainMenuBar::setup() {
    _themeName = EditorThemes::loadThemeFromFile();
    if (EditorThemes::themeMap.contains(_themeName)) {
        EditorThemes::themeMap.at(_themeName)();
    } else {
        EditorThemes::useDefaultTheme(); // fallback
    }
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
            if (ImGui::MenuItem("Save Project")) {
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
            handleThemeMenuDialog();
            if (ImGui::MenuItem("Exit")) {
                // TODO: Handle exit action
                // _editor->getApplication()->exit();
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
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
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

void EditorMainMenuBar::handleThemeMenuDialog() {
    if (ImGui::BeginMenu("Settings")) {
        if (ImGui::BeginMenu("Themes")) {
            if (ImGui::MenuItem("Default", nullptr, _themeName == "Default")) {
                EditorThemes::useDefaultTheme();
                EditorThemes::saveThemeToFile("Default");
                _themeName = "Default";
            }
            if (ImGui::MenuItem("Classic", nullptr, _themeName == "Classic")) {
                EditorThemes::useClassicTheme();
                EditorThemes::saveThemeToFile("Classic");
                _themeName = "Classic";
            }
            if (ImGui::MenuItem("Light", nullptr, _themeName == "Light")) {
                EditorThemes::useLightTheme();
                EditorThemes::saveThemeToFile("Light");
                _themeName = "Light";
            }
            if (ImGui::MenuItem("Photoshop", nullptr, _themeName == "Photoshop")) {
                EditorThemes::usePhotoshopTheme();
                EditorThemes::saveThemeToFile("Photoshop");
                _themeName = "Photoshop";
            }
            if (ImGui::MenuItem("Codz01", nullptr, _themeName == "Codz01")) {
                EditorThemes::useCodzTheme();
                EditorThemes::saveThemeToFile("Codz01");
                _themeName = "Codz01";
            }
            if (ImGui::MenuItem("Microsoft", nullptr, _themeName == "Microsoft")) {
                EditorThemes::useMicrosoftTheme();
                EditorThemes::saveThemeToFile("Microsoft");
                _themeName = "Microsoft";
            }
            if (ImGui::MenuItem("Darcula", nullptr, _themeName == "Darcula")) {
                EditorThemes::useDarculaTheme();
                EditorThemes::saveThemeToFile("Darcula");
                _themeName = "Darcula";
            }
            if (ImGui::MenuItem("Unreal Engine", nullptr, _themeName == "Unreal Engine")) {
                EditorThemes::useUnrealTheme();
                EditorThemes::saveThemeToFile("Unreal Engine");
                _themeName = "Unreal Engine";
            }
            if (ImGui::MenuItem("Cherry", nullptr, _themeName == "Cherry")) {
                EditorThemes::useCherryTheme();
                EditorThemes::saveThemeToFile("Cherry");
                _themeName = "Cherry";
            }
            if (ImGui::MenuItem("Mini Dart", nullptr, _themeName == "Mini Dart")) {
                EditorThemes::useMiniDartTheme();
                EditorThemes::saveThemeToFile("Mini Dart");
                _themeName = "Mini Dart";
            }
            if (ImGui::MenuItem("Corporate Grey", nullptr, _themeName == "Corporate Grey")) {
                EditorThemes::useCorporateGreyTheme();
                EditorThemes::saveThemeToFile("Corporate Grey");
                _themeName = "Corporate Grey";
            }
            if (ImGui::MenuItem("Simple", nullptr, _themeName == "Simple")) {
                EditorThemes::useSimpleTheme();
                EditorThemes::saveThemeToFile("Simple");
                _themeName = "Simple";
            }
            if (ImGui::MenuItem("Maroon", nullptr, _themeName == "Maroon")) {
                EditorThemes::useMaroonTheme();
                EditorThemes::saveThemeToFile("Maroon");
                _themeName = "Maroon";
            }
            if (ImGui::MenuItem("Bess Dark", nullptr, _themeName == "Bess Dark")) {
                EditorThemes::useBessDarkTheme();
                EditorThemes::saveThemeToFile("Bess Dark");
                _themeName = "Bess Dark";
            }
            if (ImGui::MenuItem("Catpucin Mocha", nullptr, _themeName == "Catpucin Mocha")) {
                EditorThemes::useCatpuccinMochaTheme();
                EditorThemes::saveThemeToFile("Catpucin Mocha");
                _themeName = "Catpucin Mocha";
            }
            if (ImGui::MenuItem("Modern Dark", nullptr, _themeName == "Modern Dark")) {
                EditorThemes::useModernDarkTheme();
                EditorThemes::saveThemeToFile("Modern Dark");
                _themeName = "Modern Dark";
            }
            if (ImGui::MenuItem("Dark Theme", nullptr, _themeName == "Dark Theme")) {
                EditorThemes::useDarkThemeTheme();
                EditorThemes::saveThemeToFile("Dark Theme");
                _themeName = "Dark Theme";
            }
            if (ImGui::MenuItem("Fluent UI", nullptr, _themeName == "Fluent UI")) {
                EditorThemes::useFluentUITheme();
                EditorThemes::saveThemeToFile("Fluent UI");
                _themeName = "Fluent UI";
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
}
