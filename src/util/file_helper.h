#pragma once

#include "filesystem.h"
#include <fstream>
#include <string>
#include <streambuf>

namespace FileHelper {
/*  Does the given file exist?  */
  inline bool file_exists(std::string name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
      fclose(file);
      return true;
    } 
    else return false;
  }

  inline std::string get_file_contents(std::string filename) {
    std::ifstream t(filename);
    std::string str{(std::istreambuf_iterator<char>(t)),
                       std::istreambuf_iterator<char>()};
    return str;
  }
}