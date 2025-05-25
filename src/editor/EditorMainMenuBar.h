/**
 * @file    EditorMainMenuBar.cpp
 * @brief   EditorMainMenuBar class implementation file
 * @details This file contains the implementation of the EditorMainMenuBar class which is responsible
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#ifndef EDITORMAINMENUBAR_H
#define EDITORMAINMENUBAR_H

#include "imgui/ImGuiFileDialog.h"

class Editor;

enum class FileDialogAction { None, NewProject, OpenProject, SaveProjectAs };

class EditorMainMenuBar {
public:
    explicit EditorMainMenuBar(Editor *editor);

    void render();

    void handleProjectMenuDialog();

private:
    Editor *_editor;
    IGFD::FileDialogConfig _fileDialogConfig;
    FileDialogAction _pendingAction = FileDialogAction::None;
    std::string _pendingPath;
    std::string _pendingProjectName;
};


#endif //EDITORMAINMENUBAR_H
