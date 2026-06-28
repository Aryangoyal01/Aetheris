#pragma once
#include "i_panel.h"
#include <string>

class BottomPanel : public IPanel
{
public:
    void Render() override;
    const std::string& GetName() const override { return m_name; }
private:
    std::string m_name = "Console";
};