#pragma once

//system headers
#include <SDL2/SDL_render.h>
#include <iostream>
#include <memory>
//graphics headers
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
//custom headers
#include"file_parser.hpp"
#include"point.hpp"


namespace image{

typedef struct SDL_GraphicsInstance{
    
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Window* window;
    uint32_t window_height    = 0;
    uint32_t window_width     = 0;

} SDL_GraphicsInstance;

class ImageHandler{
    
    private:
        uint32_t res_x = 0;
        uint32_t res_y = 0;
        SDL_Rect lens;

    public:
        ImageHandler() = default;
        int get_resolution(const std::string& filename);

        uint32_t get_res_x(){ return this->res_x; };
        uint32_t get_res_y(){ return this->res_y; };
        void set_res_x(const uint32_t parameter){ this->res_x = parameter; };
        void set_res_y(const uint32_t parameter){ this->res_y = parameter; };
};

    SDL_Texture* file_to_texture(SDL_Renderer* ren, const std::string& image_name, image::ImageHandler& image_instance);
    void draw(image::SDL_GraphicsInstance& GraphicsInstance, std::unique_ptr<SDL_Rect>& lens_ptr);
    void compute_lens(std::unique_ptr<SDL_Rect>& lens_ptr, const int& delta_w, const int& delta_h, bool zoom_in);

    void adjust_lens(std::unique_ptr<SDL_Rect>& lens_ptr, bool zoom_in, Point& point);

    void draw_at_translation(Point& point, std::unique_ptr<SDL_Rect>& lens_ptr);
    //void init_lens(std::unique_ptr<SDL_Rect>& lens_ptr, image::ImageHandler& image_instance);
    int  init_graphics(const std::string& file_name, const uint32_t& window_width, const uint32_t& window_height);
   
    void SDL_destruct_routine(image::SDL_GraphicsInstance& GraphicsInstance);
    bool GraphicsInstanceNULL(image::SDL_GraphicsInstance& GraphicsInstance);

    void init_lens(std::unique_ptr<SDL_Rect> &lens_ptr, ImageHandler &image_instance, SDL_GraphicsInstance &GraphicsInstance);
}
