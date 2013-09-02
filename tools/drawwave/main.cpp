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
	sf::Image screenshot;
	sf::Texture texture;
	pwx::CWaveColor wc;
	int32_t x = 0;
	uint8_t r,g,b;
    sf::Event Event;
    double wavelength = 380.0;
	bool drawing = true;

	// create the image and the texture
	screenshot.create(maxX, maxY);
	texture.create(maxX, maxY);

	// Set initial Wavelength
	wc.setWavelength(0, wavelength);
	wc.getRGB(r, g, b);

	// event loop to be able to break at any time
    while (App.isOpen()) {

		if (drawing) {
			// Get color and draw Line
			for (int32_t y = 0; y < maxY; ++y) {
				wc.setWavelength(0, wavelength);
				wc.doppler(0.0, 0.0, 666.0 * (halfY - y) * (y - halfY));
				wc.getRGB(r, g, b);
				screenshot.setPixel(x, y, sf::Color(r, g, b));
				if (0 == y)
					fprintf(stdout, "% 4d: 0x%02x, 0x%02x, 0x%02x - %4.1f\n", x, r, g, b, wavelength);
			}

			texture.loadFromImage(screenshot);
			App.draw(sf::Sprite(texture));

			// Move forward:
			if (++x == maxX)
				drawing = false;
			else
				wavelength += nm_step;
		} else
			sf::sleep(sf::milliseconds(250));

        // Process events
        while (App.pollEvent(Event)) {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();
			else if (drawing && (Event.type == sf::Event::LostFocus))
				screenshot = App.capture();
			else if (Event.type == sf::Event::GainedFocus) {
				App.clear();
				texture.loadFromImage(screenshot);
				App.draw(sf::Sprite(texture));
			}
        }

		App.display();
    }

    return EXIT_SUCCESS;
}
