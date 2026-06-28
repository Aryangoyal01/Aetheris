#pragma once
#include "i_panel.h"
#include <string>

class EditorState;

class BottomPanel : public IPanel
{
public:
    BottomPanel(EditorState* state);
    void Render() override;
    const std::string& GetName() const override { return m_name; }
private:
    std::string m_name = "Console";
    EditorState* m_state;
};