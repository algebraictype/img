//sdl headers
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//std headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

//custom headers
#include "include/file_parser.hpp"
#include "include/image_handle.hpp"


//TODO: PASS THESE AS ARGUMENTS TO THE INIT_GRAPHICS FUNCTION
static uint32_t W_HEIGHT = 720;
static uint32_t W_WIDTH = 1280;

std::pair<int, int> parse_arguments(std::optional<std::vector<std::string>>& args, uint32_t& W_HEIGHT, uint32_t& W_WIDTH){
    for(auto& arg : args.value()){
        if(arg == "--window-size"){
            W_HEIGHT = std::stoi(args.value()[1]);
            W_WIDTH = std::stoi(args.value()[2]);
            return std::make_pair(W_HEIGHT, W_WIDTH);
        }
    }
    return std::make_pair(W_HEIGHT, W_WIDTH);
}

std::optional<std::vector<std::string>> check_args(const size_t& argc, char** argv){
    std::optional<std::vector<std::string>> args;
    for(size_t i = 0; i < argc + 1; i++){
        args->emplace_back(argv[i]);
    }

    if(args.has_value()){
        for(auto& arg : args.value()){
            switch(arg[0]){
                case '-':
                    if(arg == "--help"){
                        std::cout << "img: usage: img [file]\n";
                        exit(EXIT_SUCCESS);
                    }
                    if(arg == "--version"){
                        std::cout << "img: version 0.0.1\n";
                        exit(EXIT_SUCCESS);
                    }
                    if(arg == "--fullscreen"){
                        std::cout << "img: fullscreen flag detected\n";
                        break;
                    }
                    if(arg == "--windowed"){
                        std::cout << "img: windowed flag detected\n";
                        break;
                    }
                    if(arg == "--window-size"){
                        parse_arguments(args, W_HEIGHT, W_WIDTH);
                        std::cout << "img: window-size\n";
                        break;
                    } else {
                        W_HEIGHT = 1280;
                        W_WIDTH = 720;
                    }
                    break;
                default:
                    break;
            }
        }
        return args.value();
    }
    return std::nullopt;
}

//TODO: add multithreading to split up the rendering work (making
//TODO: fix window resizing and add recenter hotkey
//TODO: how-to-use-instructions on '--help'
//TODO: PROPER DIRECTORY NAV; PLEASEEEEEEEEEE;
//TODO: add a launch flag to define the boot-time window size.
int main(int argc, char** argv) {
	
    FileParser file;
    std::string file_name;

	if(argc > 1){

        std::optional<std::vector<std::string>> args = check_args(argc, argv);

	    file_name = argv[1];
        
        if(file.identify_filetype(file_name)){
            
            std::cout<< "opening: " << file_name << '\n';

            if(file.get_file_type() != FileType::INVALID){
                image::init_graphics(file_name, W_WIDTH, W_HEIGHT);
                return 0;
            }
        };

        perror("invalid filetype");
        return 1;

    } else {
        file.get_next_filename();

        //pass the already instantiated file instance such that it is already incremented

        std::cout << "img: No argument provided. Walking current directory. \n";
            std::cout << "Current file: " << file.get_current_file()<<std::endl;
            //file.identify_filetype(file.get_current_file());
            file.identify_filetype(file.get_current_file());

            if(file.get_file_type() != FileType::INVALID){
                
                std::cout << "img: Opening image.\n";
                image::init_graphics(file.get_current_file(), W_WIDTH, W_HEIGHT);
                exit(EXIT_SUCCESS);
            
            } else {
                
                std::cout << "img: No valid filetypes in directory. Exiting...\n";
            
            }

    }

    exit(EXIT_SUCCESS);

}
