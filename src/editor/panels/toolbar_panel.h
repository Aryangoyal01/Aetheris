#pragma once
#include "i_panel.h"
#include <string>

class EditorState;

class ToolbarPanel : public IPanel
{
public:
    ToolbarPanel(EditorState* state);
    void Render() override;
    const std::string& GetName() const override { return m_name; }
private:
    std::string m_name = "Toolbar";
    EditorState* m_state;
};