/**
 * @file lz78.hpp
 * @author Julia Feitosa (midxfc@cin.ufpe.br)
 * @brief LZ78 is responsible for implementing the LZ78_Compress and LZ78_Decompress methods
 * @version 0.1
 * @date 2018-11-26
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef LZ78_H
#define LZ78_H

#include <string> 
#include <vector> 
#include <string.h> 
#include <stdexcept> 
#include <iostream> 
#include <fstream> 
#include <unordered_map>

#define COMPRESSED_QUALIFIER "_compressed"
#define UNCOMPRESSED_QUALIFIER "_uncompressed"
#define EXT ".idx"

class LZ78 {

    private:
        std::vector < std::pair < int, char >> code;
    public:
    /**
    * @brief Construct a new LZ78 object
    * 
    */
    
    LZ78() {};

    ~LZ78() {};

    void PrintDict(){
        for(int i=0; i<code.size(); i++){
            std::cout << "<" << i+1 << " " << code[i].first << "," << code[i].second << "> \n";
        }
    }


    void WriteToFile(const std::string & filename) {

        std::ofstream out(filename.c_str());
        int dict_size = code.size();

        for (int i = 0; i < dict_size; ++i) {
            out.write(reinterpret_cast <
                const char * > ( & code[i].first), sizeof(int));
            out.write( & code[i].second, sizeof(char));
        }

    }

    std::string LZ78_Decompress() {
        std::string txt, nextWord;
        std::unordered_map < int, std::string > dict;
        int d = 1;

        dict[0] = "";

        for (int idx = 0; idx < code.size(); ++idx) {
            int dict_index = code[idx].first;
            char decoded_char = code[idx].second;

            nextWord = dict[dict_index] + decoded_char;
            dict[d++] = nextWord;
            txt += nextWord;
        }

        return txt;
    }

    void Decompress(const std::string & input_filename, const std::string & output_filename) {

        std::ifstream in (input_filename);
        std::ofstream out(output_filename);

        std::string txt;
        int dict_size, idx;
        char chr;

        std::vector < std::pair < int, char >> code;

        while (! in .eof()) {

            in .read(reinterpret_cast < char * > ( & idx), sizeof(int)); in .read( & chr, sizeof(char));
            code.push_back(std::make_pair(idx, chr));
        }
        txt = LZ78_Decompress();
        out << txt;
    }

    void LZ78_Compress(const std::string & txt) {

        std::unordered_map < std::string, int > dict;
        std::string preffix, emp = "";
        int idx = 1, codeWordForPreffix;

        dict[""] = 0;

        for (int i = 0; i < txt.size(); ++i) {

            if (dict.find(preffix + txt[i]) != dict.end()) {
                preffix += txt[i];
            } else {
                if(preffix.empty()){
                    codeWordForPreffix = 0;
                }
                else{
                    codeWordForPreffix = dict[preffix];
                }
                 
                 code.push_back(std::make_pair(codeWordForPreffix, txt[i]));
                 dict[preffix + txt[i]] = idx++;
                 preffix.clear();

            }

        }
        if(!preffix.empty())
        {
            code.push_back(std::make_pair(dict[preffix], emp[0]));
        }


    }

    void Compress(const std::string & input_filename,
        const std::string & output_filename) {

        std::ifstream in (input_filename.c_str());
        std::string line, txt;
        std::vector < std::pair < int, char >> code;

        while (getline( in , line)) {
            txt += line;
            txt += "\n";
        } in .close();

        LZ78_Compress(txt);
        WriteToFile(output_filename);

    }

    std::string ParseFileName(std::string filename, std::string qualifier) {

        filename = filename.substr(0, filename.size() - 4);
        filename = filename + qualifier + EXT;
        return filename;

    }



};

#endif