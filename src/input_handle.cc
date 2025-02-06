#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>

#include <vector>
#include <stdlib.h>
#include <iostream>

#include"include/file_parser.hpp"
#include"include/input_handle.hpp"
#include"include/image_handle.hpp"
#include"include/round_to_int.hpp"
#include"include/point.hpp"
#include "include/util.hpp"

static int WINDOW_HEIGHT = 720;
static int WINDOW_WIDTH  = 1280;

inline void advance_file(image::SDL_GraphicsInstance& GraphicsInstance, std::string& file_name, image::ImageHandler& image_instance, std::unique_ptr<SDL_Rect>& lens_ptr){

    std::cout<<"img: Opening: " << file_name << std::endl;

    try {
        GraphicsInstance.texture = image::file_to_texture(GraphicsInstance.renderer, file_name, image_instance);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to allocate memory for file: " << file_name << '\n';
        std::cerr << "Exception: " << e.what() << '\n';
    }
perferrable
    image_instance.get_resolution(file_name);
    image::init_lens(lens_ptr, image_instance, GraphicsInstance);
   image::draw(GraphicsInstance, lens_ptr);
    printf("%d, %d\n", lens_ptr->w, lens_ptr->h);

}

int handle_events(image::SDL_GraphicsInstance& GraphicsInstance, image::ImageHandler& image_instance){


    //TODO: parse images upon key-press rather than reading entire directory at once. saves on memory usage.
    FileParser files; //file parser object
	Point point; //mouse movement tracking object
    bool moving = false; //is mouse moving
	bool quit = false; //have we quit

    std::string file_name; //current file name
    files.get_number_of_valid_files(); //get number of valid files in directory

    //TODO: when this multithreaded, add the capability to, while open, parse new files in directories.
    //TODO: add multithreading to split up the rendering work (making sure i am not fucking up)

    //SDL_Rect lens; //lens in which image is drawn (frame)
    std::unique_ptr<SDL_Rect> lens_ptr = std::make_unique<SDL_Rect>();
    image::init_lens(lens_ptr, image_instance, GraphicsInstance);
    
	
    while (!quit) {

        image::draw(GraphicsInstance, lens_ptr);
        
        //boolean to check if the mouse is moving;
        if(moving == true){
            image::draw_at_translation(point, lens_ptr);
            image::draw(GraphicsInstance, lens_ptr);
        }

        SDL_Event event;
        if (SDL_WaitEvent(&event)) {

            switch(event.type){

                //case that a key is pressed.
                case SDL_KEYDOWN:

                    //switch on the keysymbol that is pressed.
                    switch(event.key.keysym.sym){

                        case SDLK_RIGHT:
                            lens_ptr->x -= 50;
                            image::draw(GraphicsInstance, lens_ptr);
                            break;

                        case SDLK_LEFT:
                            lens_ptr->x += 50;
                            image::draw(GraphicsInstance, lens_ptr);
                            break;

                        case SDLK_UP:
                            lens_ptr->y += 50;
                            image::draw(GraphicsInstance, lens_ptr);
                            break;

                        case SDLK_DOWN:
                            lens_ptr->y -= 50;
                            image::draw(GraphicsInstance, lens_ptr);
                            break;

                        case SDLK_1:
                            image::adjust_lens(lens_ptr, true, point);
                            image::draw(GraphicsInstance, lens_ptr);
                            break;
                        
                        case SDLK_2:
                            image::adjust_lens(lens_ptr, false,point);
                            image::draw(GraphicsInstance, lens_ptr);
                            break;

                        case SDLK_r:
                            image::init_lens(lens_ptr, image_instance, GraphicsInstance);
                            image::draw(GraphicsInstance, lens_ptr);
                            break;

                        case SDLK_z:

                            if(GraphicsInstance.texture){
                                SDL_DestroyTexture(GraphicsInstance.texture); //destroy previously drawn image
                                GraphicsInstance.texture = nullptr;
                            }

                            if(files.get_next_filename()){
                                files.identify_filetype(files.get_current_file());
                            };

                            #ifdef DEBUG
                                std::cout << "FILETYPE: " << static_cast<int>(files.get_file_type()) << std::endl;
                                std::cout << "CURRENT_FILE: " << files.get_current_file() << std::endl;
                                std::cout << "FILE_COUNTER: " << files.get_file_counter() << std::endl;
                            #endif

                            file_name = files.get_current_file();

                            if(files.get_file_type() == FileType::JPG || files.get_file_type() == FileType::PNG){
                                advance_file(GraphicsInstance, file_name, image_instance, lens_ptr);
                                continue;
                            }

                            event.key.repeat = 0;
                            SDL_RenderClear(GraphicsInstance.renderer);
                            break;

                        case SDLK_x:

                            if(GraphicsInstance.texture){
                                SDL_DestroyTexture(GraphicsInstance.texture); //destroy previously drawn image
                                GraphicsInstance.texture = nullptr;
                            }

                            if(files.get_next_filename()){
                                files.identify_filetype(files.get_current_file());
                            };

                            #ifdef DEBUG
                            std::cout << "FILETYPE: " << static_cast<int>(files.get_file_type()) << std::endl;
                            std::cout << "CURRENT_FILE: " << files.get_current_file() << std::endl;
                            std::cout << "FILE_COUNTER: " << files.get_file_counter() << std::endl;
                            #endif

                            file_name = files.get_current_file();

                            if(files.get_file_type() == FileType::JPG || files.get_file_type() == FileType::PNG){
                                advance_file(GraphicsInstance, file_name, image_instance, lens_ptr);
                                continue;
                            }

                            event.key.repeat = 0;
                            SDL_RenderClear(GraphicsInstance.renderer);
                            break;
/*
                        //TODO: implement back-tracking through the directory...
                        //TODO: fix circular image browsing in current directory.


   if(files.get_file_counter() > 0) files.add_to_file_counter(-1); //advance dir position

                            if(files.get_file_counter() == 0){
                                files.set_file_counter(files.get_working_dir().size() - 1); //directory circularity
                            }

                            std::cout<<files.get_working_dir().size() << std::endl;

                            if(GraphicsInstance.texture){
                                std::cout << "GraphicsInstance.texture != NULL" << std::endl;
                                SDL_DestroyTexture(GraphicsInstance.texture); //destroy previously drawn image
                                GraphicsInstance.texture = nullptr;
                            }

                            files.identify_filetype(files.get_current_file());

                            #ifdef DEBUG
                            std::cout << "FILETYPE: " << static_cast<int>(files.get_file_type()) << std::endl;
                            std::cout << "CURRENT_FILE: " << files.get_current_file() << std::endl;
                            std::cout << "FILE_COUNTER: " << files.get_file_counter() << std::endl;
                            #endif

                            file_name = files.get_current_file();


                            if(files.get_file_type() == FileType::JPG || files.get_file_type() == FileType::PNG){
                                advance_file(GraphicsInstance, file_name, image_instance, lens_ptr);
                            } else {
                                std::cout << "img: Invalid file type\n";
                                continue;
                            }

                            event.key.repeat = 0;
                            SDL_RenderClear(GraphicsInstance.renderer);
                            */
                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        default:
                            break;
                        }
                    
                    break;

            //case that left mouse button is down.
            case SDL_MOUSEBUTTONDOWN:

                moving = 1;
                //reassign calculated mouse-down pos and the change in (x,y);
                point.dx = point.x;
                point.dy = point.y;
                point.m_state_x = lens_ptr->x;
                point.m_state_y = lens_ptr->y;
                break;

            //case that left mouse button is up.
            case SDL_MOUSEBUTTONUP:

                moving = 0;
                //mouse button is up
                point.dx = point.x;
                point.dy = point.y;
                break;

            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED){
                    std::cout<<"Window resized\n";
                    GraphicsInstance.window_width = event.window.data1;
                    GraphicsInstance.window_height = event.window.data2;
                }
                    break;

            case SDL_MOUSEWHEEL:

                if(event.wheel.y > 0){
                    image::adjust_lens(lens_ptr, true, point);
                    image::draw(GraphicsInstance, lens_ptr);
                } else if(event.wheel.y < 0){
                    image::adjust_lens(lens_ptr, false, point);
                    image::draw(GraphicsInstance, lens_ptr);
                }
                break;

            default:
                SDL_SetRenderDrawColor(GraphicsInstance.renderer, 40, 43, 40, SDL_ALPHA_OPAQUE);
                break;
            
            //end of outer switch
            }
        //end of conditional statement
        }
    SDL_GetMouseState(&point.x, &point.y);
    //end of while
    }
    
    image::SDL_destruct_routine(GraphicsInstance);
	return 0;

}
