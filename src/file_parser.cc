#include<vector>
#include<string.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<filesystem>
#include"include/image_handle.hpp"
#include"include/file_parser.hpp"

#define _CRT_SECURE_NO_WARNINGS



//worst case of O(n) space complexity as this function essentially creates a list of all files in the current directory,
//if you have not iterated over all files in the directory, the function has O(n - number of files iterated over) space complexity

//do i even need to hold the files in a data structure at all? i could just iterate over the files in the directory upon pressing the button to change the image


[[nodiscard]] bool FileParser::get_next_filename(){

    //TODO: how do i traverse files backwards now?

    for(; this->dir_iter != std::filesystem::directory_iterator(); ++this->dir_iter){

        if(std::filesystem::is_regular_file(dir_iter->path())){
            std::cout<< "Iterator value: " << this->iterated_over_files <<std::endl;
            std::cout<< "Number of valid files: " << this->number_of_valid_files << std::endl;
            
            if((this->iterated_over_files) == this->number_of_valid_files){
                this->iterated_over_files = 0;
                this->dir_iter = std::filesystem::directory_iterator(".");
                return true;
            }


            if(dir_iter->path().extension() == ".png" || dir_iter->path().extension() == ".jpg" || dir_iter->path().extension() == ".jpeg") {
                this->current_file = dir_iter->path().string();
                std::cout << "current: " << this->current_file << std::endl;
                while(current_file == dir_iter->path().string()){
                    this->iterated_over_files++;
                    ++this->dir_iter;
                }
                return true;
            }
        }
    }
    return false;
}

/*
[[nodiscard]] bool FileParser::get_next_filename(){

    //TODO: traverse files backwards now?

    for(; this->dir_iter != std::filesystem::directory_iterator(); ++this->dir_iter){
        if(std::filesystem::is_regular_file(dir_iter->path())){
            if(dir_iter->path().extension() == ".png" || dir_iter->path().extension() == ".jpg" || dir_iter->path().extension() == ".jpeg") {
                this->current_file = dir_iter->path().string();
                std::cout << "current: " << this->current_file << std::endl;
                //TODO: add this current filename to the list of filenames in the fileparser
                valid_images.emplace_back(this->current_file);
                ++this->dir_iter;
                return true;
            }
        }
    }
return false;
}
*/

/*
[[nodiscard]] bool FileParser::get_prev_filename(){

    //TODO: decrement ???????
    //TODO: how do i decrement an iterator? should i just use a for loop that catalogues files as running? problem is that space complexity is then O(n). space complexity should be O(1)
        for(; this->dir_iter != std::filesystem::directory_iterator("."); --this->dir_iter){

            if(std::filesystem::is_regular_file(dir_iter->path())){

                if((this->iterated_over_files) == this->number_of_valid_files){
                    this->iterated_over_files = 0;
                    this->dir_iter = std::filesystem::directory_iterator(".");
                    return true;
                }

                if(dir_iter->path().extension() == ".png" || dir_iter->path().extension() == ".jpg" || dir_iter->path().extension() == ".jpeg") {
                    this->current_file = dir_iter->path().string();
                    std::cout << "current: " << this->current_file << std::endl;
                    this->iterated_over_files++;
                    --this->dir_iter;
                    return true;
                }
            }
        }
}
 */

void FileParser::get_number_of_valid_files(){

        for(const auto& entry : std::filesystem::directory_iterator(".")){
            if(std::filesystem::is_regular_file(entry.path())){
                if(entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg"){
                    this->number_of_valid_files++;
                }
            }
        }
}


//old file parser
bool FileParser::identify_filetype(const std::string& file_name){

    std::string hex_contents;
    std::ifstream input(file_name);

    //read first 4 bytes of input into a string
    std::string buffer(4, '\0');
    input.read(&buffer[0], 4);
    std::string contents(buffer);
    input.close();
    
    std::stringstream ss;
    for(auto i = 0; i < 4; i++){
        ss << std::hex << (int)contents.c_str()[i];
    }
    
    hex_contents = ss.str();

    ss.clear();

    if(hex_contents == "ffffff89504e47"){
        this->set_file_type(FileType::PNG);
        return true;
    } else if(hex_contents == "ffffffffffffffd8ffffffffffffffe0"){
        this->set_file_type(FileType::JPG); 
        return true;
    } else if(hex_contents == "ffffffffffffffd8ffffffffffffffe2"){
        this->set_file_type(FileType::JPG); 
        return true;
    } else {
        return false;
        this->set_file_type(FileType::INVALID);
    }
    
    hex_contents.clear();

}
