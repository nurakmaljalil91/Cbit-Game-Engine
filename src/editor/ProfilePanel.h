/**
 * @file    ProfilePanel.h
 * @brief   ProfilePanel class header file
 * @details This file contains the definition of the ProfilePanel class which is responsible for rendering
 * @author  Nur Akmal bin Jalil
 * @date    2025-05-19
 */

#ifndef PROFILEPANEL_H
#define PROFILEPANEL_H

class Editor;

class ProfilePanel {
public:
    explicit ProfilePanel(Editor *editor);

    void render() const;

private:
    Editor *_editor;
};


#endif //PROFILEPANEL_H
