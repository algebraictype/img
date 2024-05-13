#pragma once
#include<vector>
#include<string>
#include<filesystem>
#include <deque>

enum class FileType{
    PNG,
    JPG,
    INVALID
};

class FileParser{

private:

        //keep track of which file in the directory we are on
        std::string current_file;

        //variable to hold the type of an analyzed
        FileType file_type;
        uint32_t number_of_valid_files = 0;
        uint32_t iterated_over_files = 1;
        std::deque<std::string> valid_images;
        //TODO: RETAIN VALUES WHEN DIRECTORY IS CHANGED
        //TODO: LOAD IMAGE AND IMAGE INFRONT OF IT
        std::filesystem::directory_iterator dir_iter = std::filesystem::directory_iterator(".");

public:

    //default constructor;
    FileParser(){
        this->current_file.clear();
        this->file_type = FileType::INVALID;
    };

    void list_directories(const char* dirname);
    void init_working_dir();
    bool identify_filetype(const std::string& file_name);
    void add_iterated_over(){ this->iterated_over_files++; }


        //accesors and modifiers;
    //TODO: have currentfile tell us more information about the file we are trying to open.

    void set_file_type(FileType type) { this->file_type = type; };
    bool get_next_filename();
    std::string get_current_file(){
        //explicit this'ing
        return this->current_file;
    }
    bool get_prev_filename() {
        if (!valid_images.empty()) {
            valid_images.pop_back();
            return true;
        }
        return false;
    }
    FileType get_file_type() { return this->file_type; };
    void get_number_of_valid_files();

    //bool get_prev_filename();
};
