#include "../headers/Application.h"

int main()
{
    srand(static_cast<unsigned>(time(NULL)));

    Application::instance = new Application();

    Application::instance->Run();

    delete Application::instance;

    return EXIT_SUCCESS;
}