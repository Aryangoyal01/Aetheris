#include "application/application.h"

int main()
{
    Application app;
    app.Initialize();
    app.Run();
    app.Shutdown();
    return 0;
}
