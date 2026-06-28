#pragma once

#include <string>

class IPanel
{
public:
    virtual ~IPanel() = default;
    
    virtual void Render() = 0;
    virtual const std::string& GetName() const = 0;
};
