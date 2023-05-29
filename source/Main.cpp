
// Este código es de dominio público.
// angel.rodriguez@esne.edu
// 2021.12 - 2021.04

#include <SFML/Window.hpp>
#include <windows.h>
#include "View.hpp"
using namespace sf;
using namespace example;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

    // Create the window and the view that will be shown within the window:

    constexpr auto window_width  = 800u;
    constexpr auto window_height = 600u;

    Window window(VideoMode(window_width, window_height), "Mesh Loader", Style::Titlebar | Style::Close);
    View   view  (window_width, window_height);

    window.setVerticalSyncEnabled (true);

    const float target_fps = 60.f;                      // Cuántos fotogramas por segundo se busca conseguir
    const float target_time = 1.f / target_fps;         // Duración en segundos de un fotograma a la tasa deseada

    float delta_time = target_time;                     // Previsión de la duración del fotograma actual

    // Run the main loop:

    bool exit = false;
	
    Clock timer;

    do
    {
        timer.restart();

        Event event;

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed) exit = true;
        }
    
        // Handle real-time input
        view.get_camera().handle_input(window, delta_time);
        
        view.update ();

        view.render ();

        window.display ();

		
        float elapsed = timer.getElapsedTime().asSeconds();

        if (elapsed < target_time)
        {
            sleep(seconds(target_time - elapsed));
        }

        // Se restablece la estimación de la duración del siguiente fotograma:

        delta_time = timer.getElapsedTime().asSeconds();
		
    }
    while (not exit);

    return 0;
}
