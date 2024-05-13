#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include <cstdlib>
#include <iostream>
#include <memory>

#include "include/image_handle.hpp"
#include "include/round_to_int.hpp"
#include "include/input_handle.hpp"
#include "include/file_parser.hpp"



//TODO: add a launch flag to define the boot-time window size.
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

SDL_Texture* image::file_to_texture(SDL_Renderer* ren, const std::string& image_name, image::ImageHandler& image_instance){

    //create a surface loading the passed file_name
    SDL_Surface* next_image = IMG_Load(image_name.c_str());
    if(next_image == NULL){
        std::cerr << "img: SDL_Surface* next_image is NULL " << IMG_GetError();
        return NULL;
    }
    SDL_Texture* next_texture = SDL_CreateTextureFromSurface(ren,next_image);
    if(next_texture == NULL){
        std::cerr << "img: SDL_Texture* next_texture is NULL " << SDL_GetError();
        SDL_FreeSurface(next_image);
        return NULL;
    }
    image_instance.set_res_x(next_image->w);
    image_instance.set_res_y(next_image->h);

    SDL_FreeSurface(next_image);
    return next_texture;
}

void image::draw_at_translation(Point& point, std::unique_ptr<SDL_Rect>& lens_ptr){
    lens_ptr->x = point.x - (point.dx - point.m_state_x);
    lens_ptr->y = point.y - (point.dy - point.m_state_y);
}

//the reason this is kept in the ImageHandler is so one may be able to later access the current image's resolution
//without recomputing or carrying a 'struct' around with values
int image::ImageHandler::get_resolution(const std::string& filename){

    SDL_Surface* loaded_img = IMG_Load(filename.c_str());

    if(loaded_img == NULL){
        std::cerr << "img: SDL_Surface* loaded_img is NULL " << IMG_GetError();
        return 1;
    }

    this->res_x = loaded_img->w;
    this->res_y = loaded_img->h;
    
    SDL_FreeSurface(loaded_img);
    loaded_img = nullptr;

    return 0;

}

bool image::GraphicsInstanceNULL(image::SDL_GraphicsInstance& GraphicsInstance){

    if(GraphicsInstance.texture == NULL || GraphicsInstance.renderer == NULL || GraphicsInstance.window == NULL){
        return true;
    }

    return false;

}

void image::init_lens(std::unique_ptr<SDL_Rect>& lens_ptr, image::ImageHandler& image_instance, SDL_GraphicsInstance& GraphicsInstance){

    lens_ptr->w = image_instance.get_res_x();
    lens_ptr->h = image_instance.get_res_y();
    lens_ptr->x = GraphicsInstance.window_width/2 - lens_ptr->w/4;
    lens_ptr->y = GraphicsInstance.window_height/2 - lens_ptr->h/4;

    lens_ptr->w /= 2;
    lens_ptr->h /= 2;

}

void image::adjust_lens(std::unique_ptr<SDL_Rect>& lens_ptr, bool zoom_in, Point& point){

    double zoom_factor = sqrt(0.1);
    int delta_h = round_to_int(zoom_factor * lens_ptr->h)/2;
    int delta_w = round_to_int(zoom_factor * lens_ptr->w)/2;

    int center_x = lens_ptr->x + lens_ptr->w / 2;
    int center_y = lens_ptr->y + lens_ptr->h / 2;

    float proportion_x = (point.x - center_x) / (float)lens_ptr->w;
    float proportion_y = (point.y - center_y) / (float)lens_ptr->h;

        if(zoom_in){
            if(lens_ptr->w > 0 && lens_ptr->h > 0){
                lens_ptr->w += delta_w;
                lens_ptr->h += delta_h;
                lens_ptr->x = center_x - lens_ptr->w / 2 - proportion_x * delta_w;
                lens_ptr->y = center_y - lens_ptr->h / 2 - proportion_y * delta_h;
            }
        } else {
            if ((lens_ptr->w - delta_w > 0) && (lens_ptr->h - delta_h > 0)) {
                lens_ptr->w -= delta_w;
                lens_ptr->h -= delta_h;
                lens_ptr->x = center_x - lens_ptr->w / 2 + proportion_x * delta_w;
                lens_ptr->y = center_y - lens_ptr->h / 2 + proportion_y * delta_h;
            }
        }
}

void image::draw(image::SDL_GraphicsInstance& GraphicsInstance, std::unique_ptr<SDL_Rect>& lens_ptr){

    if(!GraphicsInstanceNULL(GraphicsInstance)){
        
        SDL_SetRenderDrawBlendMode(GraphicsInstance.renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderClear(GraphicsInstance.renderer);
        SDL_RenderCopy(GraphicsInstance.renderer, GraphicsInstance.texture, nullptr, lens_ptr.get());
        SDL_RenderPresent(GraphicsInstance.renderer);
    }

}

inline void init_lens(std::unique_ptr<SDL_Rect>& lens_ptr, image::ImageHandler& image_instance){

    lens_ptr->w = image_instance.get_res_x()/2;
    lens_ptr->h = image_instance.get_res_y()/2;

    std::cout<< "img: lens_ptr->w: " << lens_ptr->w << '\n';
    std::cout<< "img: lens_ptr->h: " << lens_ptr->h << '\n';

    lens_ptr->x = WINDOW_WIDTH/2 - lens_ptr->w/4;
    lens_ptr->y = WINDOW_HEIGHT/2 - lens_ptr->h/4;
}

void image::SDL_destruct_routine(image::SDL_GraphicsInstance& GraphicsInstance){

    if(!GraphicsInstanceNULL(GraphicsInstance)){
        std::cerr << "img: GraphicsInstance's field is NULL\nimg: Cannot continue destruct routine.\n";
    } else {
        SDL_DestroyTexture(GraphicsInstance.texture);
        SDL_DestroyRenderer(GraphicsInstance.renderer);
        //TODO: LEAK: this is leaking memory with an invalid read of size 8?
        SDL_DestroyWindow(GraphicsInstance.window);
        GraphicsInstance.texture = nullptr;
        GraphicsInstance.renderer = nullptr;
        GraphicsInstance.window = nullptr;
        SDL_Quit();
        IMG_Quit();
    }
}

int image::init_graphics(const std::string &file_name, const uint32_t& window_width, const uint32_t& window_height){

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)!= 0) {
        std::cerr<<"img: SDL2_INIT_ERR: "<< SDL_GetError()<<std::endl;
        exit(EXIT_FAILURE);
    }

    //Init SDL2's image renderer.
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0){
        std::cerr<<"img: Couldn't initialize the image drawing engine: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Surface* loaded_img = IMG_Load(file_name.c_str());
    if (loaded_img == NULL) {
        std::cerr << "img: Invalid filetype: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Window* window = SDL_CreateWindow("img", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    //error checking
    if (window == NULL) {
        std::cerr<<"SDL2_INIT_WIN_ERR: "<< SDL_GetError();
        exit(EXIT_FAILURE);
    }

    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //error checking
    if (ren == NULL) {
        std::cerr<<"SDL2_INIT_RENDERER_ERR: "<<SDL_GetError();
        exit(EXIT_FAILURE);
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, loaded_img);
    //error checking
    if (tex == NULL) {
        std::cout << "SDL2_INIT_TEXTURE_ERR: "<<SDL_GetError();
        exit(EXIT_FAILURE);
    }

    SDL_GraphicsInstance GraphicsInstance;
    GraphicsInstance.texture = tex;
    GraphicsInstance.window = window;
    GraphicsInstance.renderer = ren;
    GraphicsInstance.window_height = window_height;
    GraphicsInstance.window_width = window_width;

    image::ImageHandler image_instance;
    image_instance.get_resolution(file_name);
    std::cout << image_instance.get_res_x() << '\n';
    std::cout << image_instance.get_res_y() << '\n';

    //free sdl surface because the sdl texture has already been created with it.
    handle_events(GraphicsInstance, image_instance);
    SDL_FreeSurface(loaded_img);

    return 0;
}
