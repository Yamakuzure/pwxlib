#include <pwx/types/CWaveColor.h>
#include <SFML/Graphics.hpp>

#include <cstdio>

const int32_t maxX  = 800;
const int32_t maxY  = 600;
const int32_t halfX = maxX / 2;
const int32_t halfY = maxY / 2;

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
	int32_t x = 0;
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
			for (int32_t y = 0; y < maxY; ++y) {
				wc.setWavelength(0, wavelength);
				wc.doppler(100.0 * ( x - halfX), 100.0 * ( y - halfY), 100.0 * ((halfX - x) + (halfY - y)),
						  -500.0 *   x,         -500.0 *   y,         -500.0 * ((x - halfX) * (y - halfY)) );
				wc.getRGB(r, g, b);
				screenshot.SetPixel(x, y, sf::Color(r, g, b));
				if (0 == y)
					fprintf(stdout, "% 4d: 0x%02x, 0x%02x, 0x%02x - %4.1f\n", x, r, g, b, wavelength);
			}

			App.Draw(sf::Sprite(screenshot));
			// Move forward:
			if (++x == maxX)
				drawing = false;
			else
				wavelength += nm_step;
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
