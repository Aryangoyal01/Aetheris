#pragma once
#include "i_panel.h"
#include <string>

class EditorState;

class RightSidebar : public IPanel
{
public:
    RightSidebar(EditorState* state);
    void Render() override;
    const std::string& GetName() const override { return m_name; }
private:
    std::string m_name = "Inspector";
    EditorState* m_state;
};