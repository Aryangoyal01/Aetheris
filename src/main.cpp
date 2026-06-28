#include "application/application.h"
#include "platform/dpi_aware.h"

int main()
{
    SetAetherisDpiAware();

    Application app;
    app.Initialize();
    app.Run();
    app.Shutdown();
    return 0;
}
