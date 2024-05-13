#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <memory>
#include <iostream>
#include "image_handle.hpp"

int handle_events(image::SDL_GraphicsInstance& GraphicsInstance, image::ImageHandler& image_instance);
