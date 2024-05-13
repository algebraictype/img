#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#include <vector>
#include <stdlib.h>
#include <iostream>

#include"include/file_parser.hpp"
#include"include/input_handle.hpp"
#include"include/image_handle.hpp"
#include"include/round_to_int.hpp"
#include"include/point.hpp"


#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280


int handle_events(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture* texture, image::image_handler* image_instance){

        FileParser files; //file parser object
		Point point; //mouse movement tracking object
        bool moving = 0; //is mouse moving
	    bool quit = 0; //have we quit
		size_t file_counter = 0; //depends on if program is executed with arugment or not
                            
        double zoom_factor = 0;
        int delta_w = 0;
        int delta_h = 0;
		
        //store filenames in working directory
        std::vector<std::string> working_directory = files.get_filenames(".");

        //compute size of working directory once so as to minimize number of function calls.
        size_t working_dir_size = working_directory.size();
        //init objects
        
        SDL_Rect lens; //lens in which image is drawn (frame)
        lens.w = image_instance->res_x;
		lens.h = image_instance->res_y;
        
        lens.x = WINDOW_WIDTH/2 - lens.w/4;
		lens.y = WINDOW_HEIGHT/2 - lens.h/4;
        size_t increment = 100;

        lens.w /= 2;
        lens.h /= 2;


	while (!quit) {

        
        image::zoom_image(renderer, texture, lens);
        //boolean to check if the mouse is moving;
        if(moving == 1){

            lens.x = point.x - (point.dx - point.m_state_x);
            lens.y = point.y - (point.dy - point.m_state_y);
            image::zoom_image(renderer, texture, lens);

        }

        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch(event.type){

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){

                        case SDLK_RIGHT:
                            lens.x -= 50;
                            image::zoom_image(renderer, texture, lens);
                            break;

                        case SDLK_LEFT:
                            lens.x += 50;
                            image::zoom_image(renderer, texture, lens);
                            break;

                        case SDLK_UP:
                            lens.y += 50;
                            image::zoom_image(renderer, texture, lens);
                            break;

                        case SDLK_DOWN:
                            lens.y -= 50;
                            image::zoom_image(renderer, texture, lens);
                            break;

                        case SDLK_1:
                        
                        if(lens.w > 0 && lens.h > 0){
                            zoom_factor = sqrt(increment);
                            delta_w = round_to_int(zoom_factor * lens.w)/2;
                            delta_h = round_to_int(zoom_factor * lens.h)/2;
                            lens.w += delta_w;
                            lens.h += delta_h;
                            lens.x -= delta_w / 2;
                            lens.y -= delta_h / 2;
                            image::zoom_image(renderer, texture, lens);
                            SDL_RenderClear(renderer);
                        }
                                break;
                            
                        
                        case SDLK_2:
                            
                           printf("lens subtract: %d\n", (lens.h - round_to_int(sqrt((increment) * lens.h/lens.w))/2));
                                    
                                lens.w -= round_to_int(sqrt(increment * lens.w/lens.h))/2;
                                lens.h -= round_to_int(sqrt(increment * lens.h/lens.w))/2;
                                printf("lensw: %d, lensh: %d\n", lens.w, lens.h);
                                
                                if(lens.w > 0 && lens.h > 0){
                                    lens.x += round_to_int(sqrt(increment * lens.w/lens.h))/4;
                                    lens.y += round_to_int(sqrt(increment * lens.h/lens.w))/4;
                                }
                                
                                image::zoom_image(renderer, texture, lens);
                                SDL_RenderClear(renderer);

                            break;

                        case SDLK_z:

                            file_counter += 1; //advance dir position


                            if(file_counter >= working_dir_size){
                                file_counter = 0; //directory circularity
                            }


                                SDL_DestroyTexture(texture); //destroy previously drawn image
                                std::cout<<"opening: "<<working_directory[file_counter]<<std::endl;


                                    if(files.identify_filetype(working_directory[file_counter])
                                        == 1 ||
                                        files.identify_filetype(working_directory[file_counter])
                                        == 2){

                                        texture = image::image_scroll(renderer,
                                        working_directory[file_counter], image_instance);
                                        image_instance->get_resolution(working_directory[file_counter]);
                                        
                                        lens.w = image_instance->res_x;
                                        lens.h = image_instance->res_y;
                                        lens.x = WINDOW_WIDTH/2 - lens.w/4;
                                        lens.y = WINDOW_HEIGHT/2 - lens.h/4;
                                        lens.w /= 2;
                                        lens.h /= 2;

                                        printf("%d, %d\n", lens.w, lens.h);
                                        image::zoom_image(renderer, texture, lens);

                                    }

                                event.key.repeat = 0;
                                SDL_RenderClear(renderer);

                            break;

                        case SDLK_x:

                           file_counter -= 1; //decrement the file counter upon pressing x such that
                                              //we can traverse backwards through the file system.

                           if(file_counter <= 0){
                               file_counter = working_dir_size - 1;
                           }
                                    SDL_DestroyTexture(texture);
                                    std::cout<<working_directory[file_counter]<<std::endl;

                                    //CHECK IF WE R OPENING IMAGE TO AVOID SEGFAULT
                                    if(files.identify_filetype(working_directory[file_counter])
                                        == 1 ||
                                        files.identify_filetype(working_directory[file_counter])
                                        == 2){

                                        texture = image::image_scroll(renderer,
                                        working_directory[file_counter], image_instance);
                                        lens.w = image_instance->res_x;
                                        lens.h = image_instance->res_y;
                                        image::zoom_image(renderer, texture, lens);

                                    }

                                    event.key.repeat=0;
                                    SDL_RenderClear(renderer);

                            break;

                        case SDLK_ESCAPE:
                            quit = 1;
                            break;

                        default:
                            break;
                        }
                    break;
            default:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                break;

            case SDL_MOUSEBUTTONDOWN:
                moving = 1;
                //reassign calculated mouse-down pos and the change in (x,y);
                point.dx = point.x;
                point.dy = point.y;
                point.m_state_x = lens.x;
                point.m_state_y = lens.y;
                break;

            case SDL_MOUSEBUTTONUP:
                moving = 0;
                //mouse button is up
                point.dx = point.x;
                point.dy = point.y;
                break;

            case SDL_MOUSEWHEEL:
                if(event.wheel.y > 0){
                         
                        lens.w += round_to_int(sqrt(increment * lens.w/lens.h))/2;
                        lens.h += round_to_int(sqrt(increment * lens.h/lens.w))/2;
                        lens.x -= round_to_int(sqrt(increment * lens.w/lens.h))/4;
                        lens.y -= round_to_int(sqrt(increment * lens.h/lens.w))/4;

                        image::zoom_image(renderer, texture, lens);
                        SDL_RenderClear(renderer);
                        break;
                
                } else if(event.wheel.y < 0){
                         
                         
                        if((round_to_int(sqrt(increment * lens.w/lens.h))/2) > 0 && (round_to_int(sqrt(increment * lens.h/lens.w))/2) > 0){
                            
                            lens.w -= round_to_int(sqrt(increment * lens.w/lens.h))/2;
                            lens.h -= round_to_int(sqrt(increment * lens.h/lens.w))/2;
                            
                            printf("sqrt-lensw: %f, sqrt-lensh: %f\n", sqrt(increment * lens.w/lens.h)/2 , sqrt(increment * lens.h/lens.w)/2);
                            printf("lensw: %d, lensh: %d\n", lens.w, lens.h);
                            
                            lens.x += round_to_int(sqrt(increment * lens.w/lens.h))/4;
                            lens.y += round_to_int(sqrt(increment * lens.h/lens.w))/4;
                            image::zoom_image(renderer, texture, lens);
                            SDL_RenderClear(renderer);
                            break;
                        }
                        printf("max zoom reached\n");
                        break;
                }
            }
		}

        SDL_GetMouseState(&point.x, &point.y);

    }


    //de-allocate allocated textures and renderer/windows.
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
