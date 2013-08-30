#include <pwx/types/CWaveColor.h>
#include <SFML/Graphics.hpp>

#include <cstdio>

const uint32_t maxX = 800;
const uint32_t maxY = 600;

/* Visible light is only produced between 380nm and 780nm. This
 * is then a range of 400nm. When maxX is 800, this means 0.5 nm
 * per pixel.
 */
double nm_step = 400.0 / static_cast<double>(maxX);

int main()
{
    sf::RenderWindow App(sf::VideoMode(maxX, maxY), "PrydeWorX WaveDraw");
	sf::Image screenshot(maxX, maxY);
	pwx::CWaveColor wc;
	uint32_t x = 0;
	uint8_t r,g,b;
    sf::Event Event;
    double wavelength = 380.0;
	bool drawing = true;

	// Set initial Wavelength
	wc.setWavelength(0, wavelength);
	wc.getRGB(r, g, b);

	// event loop to be able to break at any time
    while (App.IsOpened()) {

		if (drawing) {
			// Get color and draw Line
			App.Draw(sf::Shape::Line((float)x, 0.f, (float)x, (float)maxY, 1.f, sf::Color(r, g, b)));
			fprintf(stdout, "% 4d: 0x%02x, 0x%02x, 0x%02x - %4.1f\n", x, r, g, b, wavelength);
			// Move forward:
			if (++x == maxX) {
				drawing = false;
				screenshot = App.Capture();
			} else {
				wavelength += nm_step;
				wc.setWavelength(0, wavelength);
				wc.getRGB(r, g, b);
			}
		} else
			sf::Sleep(0.25f);

        // Process events
        while (App.GetEvent(Event)) {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
			else if (drawing && (Event.Type == sf::Event::LostFocus))
				screenshot = App.Capture();
			else if (Event.Type == sf::Event::GainedFocus) {
				App.Clear();
				App.Draw(sf::Sprite(screenshot));
			}
        }

		App.Display();
    }

    return EXIT_SUCCESS;
}
