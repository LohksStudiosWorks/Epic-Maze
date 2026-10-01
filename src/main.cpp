#include <allegro5/allegro.h>
#include <iostream>

int main(int argc, char **argv) {
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro 5.\n";
        return -1;
    }

    // Create a simple window
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) {
        std::cerr << "Failed to create display.\n";
        return -1;
    }

    // Clear to a blue color, wait 2 seconds, and exit
    al_clear_to_color(al_map_rgb(50, 100, 200));
    al_flip_display();
    al_rest(2.0);

    al_destroy_display(display);
    return 0;
}
