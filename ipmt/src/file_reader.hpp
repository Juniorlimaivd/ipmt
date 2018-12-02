/**
 * @file file_reader.hpp
 * @author Junior Lima (junior.lima.ivd.2@gmail.com)
 * @brief File reader is responsible for create an interface for 
 *        read text files line by line efficiently
 * @version 0.1
 * @date 2018-10-16
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef FILE_READER_H
#define FILE_READER_H

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <stdexcept>


class FileReader {
    /**
     * @brief Size of text chunk
     * 
     */
    static const int BUFFER_SIZE = 327680;

public:
    /**
     * @brief Construct a new File Reader object
     * 
     */
    FileReader(){};

    /**
     * @brief Construct a new File Reader object and load the file
     * 
     * @param filename Name of the file to be loaded
     */
    FileReader(const std::string &filename){
        this->load(filename);
    };

    /**
     * @brief Destroy the File Reader object
     * 
     */
    ~FileReader(){
        close(this->_fileDescriptor);
    }; 

    /**
     * @brief Check if the file exist, open the file, create a file descriptor and 
     * read the first chunk of the file.  
     * 
     * @param filename Name of the file to be loaded
     * @return true Sucessful load
     * @return false Unsucessful loading
     */
    bool load(const std::string& filename) {
        this->_filename = filename;
        
        if (!fileExists()){
            printf("Error: file '%s' not exist.\n", this->_filename.c_str());
            exit(-1);
        }

        this->_bytesRead = 0;
        this->openFile();
        
        _bytesRead = read(_fileDescriptor, _buffer, BUFFER_SIZE);

        this->_linePointer = this->_buffer - 1;

        return true;
    };

    /**
     * @brief Get the next line of the file
     * OBS: If the line is not ending in his chunk, it will return a slice of the line 
     * 
     * @param dest String where the line should be returned
     * @return int The result of the operation
     *          0 : Sucessful line reading
     *          1 : Readed only a slice of the line
     *         -1 : End of File
     */
    int getLine(std::string& dest) {

        if (_filename == "" || _bytesRead == 0 || this->_linePointer == NULL){
            printf("Error: fileReader not initialized. Call FileReader::load() before get line.");
            exit(-1);
        }
        
        _linePointer++;

        int size = (int) ((_buffer + _bytesRead) - _linePointer);

        // If all buffer readed, try to read new chunk
        if (size <= 0) { 
            _bytesRead = read(_fileDescriptor, _buffer, BUFFER_SIZE);
            _linePointer = this->_buffer;

            size = (int) ((_buffer + _bytesRead) - _linePointer);

            // If can't read buffer, reach EOF
            if (!_bytesRead) { 
                return -1;
            }  
        }
        
        char* pointer = (char *) memchr(_linePointer, '\n', size);

        int linesize;

        // If pointer is NULL, the line is splitted in two chunks
        if (pointer == NULL)
            linesize = (int)((_buffer + _bytesRead) - _linePointer); 
        else
            linesize = (int)(pointer - _linePointer);

        char line[linesize+1];
        
        strncpy(line,_linePointer, linesize);
        line[linesize] = '\0';

        dest =  std::string(line);
        
        if (pointer != NULL) 
            _linePointer = pointer;
        else 
            _linePointer = _buffer + _bytesRead;

        return (pointer == NULL) ? 1 : 0;
    };

private:
    std::string _filename;
    int _fileDescriptor;
    char _buffer[BUFFER_SIZE+1];
    int _bytesRead;
    char* _linePointer;

    /**
     * @brief Return if file exists in current directory
     */
    bool fileExists() {
        struct stat buffer;
        return (stat(this->_filename.c_str(), &buffer) == 0);
    };

    /**
     * @brief Opens the file and init the file descriptor
     * 
     */
    void openFile() {
        _fileDescriptor = open(this->_filename.c_str(), O_RDONLY);
        
        if (_fileDescriptor == -1){
            printf("Error tryin to open %s.\n", this->_filename.c_str());
            exit(-1);
        }

        //posix_fadvise(_fileDescriptor, 0, 0, 1); // FDADVICE_SEQUENTIAL
    }
};

#endif