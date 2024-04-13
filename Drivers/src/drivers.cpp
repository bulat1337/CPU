#include <stdio.h>
#include <math.h>

#include <SFML/Graphics.hpp>
#include "drivers.h"

void file_draw(VM *vm, char *current_byte_code, FILE *output_file)
{
	unsigned int head = *(unsigned int *)(current_byte_code + sizeof(double));
	unsigned int end  = *(unsigned int *)(current_byte_code + sizeof(double) + sizeof(int));

	size_t screen_size = (size_t)sqrt(end - head + 1); // sqrt тяжело

	for(unsigned int address = head; address < end; address++)
	{
		fprintf(output_file, "%c  ", (char)vm->rand_access_mem.user_RAM[address]);

		if((address + 1) % screen_size == 0)
		{
			fprintf(output_file, "\n");
		}

	}
}

void window_draw(VM *vm, char * current_byte_code, FILE *junk)
{
	unsigned int head = *(unsigned int *)(current_byte_code + sizeof(double));
	unsigned int end  = *(unsigned int *)(current_byte_code + sizeof(double) + sizeof(int));


	size_t screen_size = (size_t)sqrt(end - head + 1);

	sf::RenderWindow window(sf::VideoMode(screen_size, screen_size), "SFML Window");

	while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
			{
                window.close();
			}
        }

        window.clear(sf::Color::Black);

		for(size_t pixel_ID = head; pixel_ID < end; pixel_ID++)
		{
			if(vm->rand_access_mem.user_RAM[pixel_ID] != 46)
			{
				sf::RectangleShape rectangle(sf::Vector2f(1, 1));
        		rectangle.setFillColor(sf::Color::Red);
        		rectangle.setPosition(pixel_ID % screen_size , pixel_ID / screen_size);

				window.draw(rectangle);
			}
		}

        window.display();
    }
}
