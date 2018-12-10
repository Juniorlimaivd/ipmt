#include <iostream>
#include <getopt.h>
#include <string>
#include <vector>
#include <sstream>
#include "searcher.hpp"
#include "lz78.hpp"
#include "suffix_array.hpp"



using namespace std;

enum Algorithm
{
      NONE,
      SUFFIX_TREE,
      SUFFIX_ARRAY,
};



enum Mode {
      invalid,
      INDEX,
      SEARCH
};


typedef struct RunInfo {
      vector<string> patterns;
      string textFile;
      bool isCountMode;
      Mode runMode;
} RunInfo;

vector<string> parserPatternFile(string filename){
      vector<string> p;
      
      return p;
}

void printHelp() {
      printf("Usage: $ pmt mode [options]\n");
      printf("-i, --index textfile\n"
            "-s, --search pattern indexfile\n"
            "-c, --count\n"
            "-h, --help\n"
            "-p, --pattern [pattern_file]\n");
}

Algorithm chooseAlgorithm(RunInfo info) {
		return SUFFIX_ARRAY;
}
void indexTextFile (RunInfo info){

LZ78 lz78;


}

void decompressAndSearch (RunInfo info){

LZ78 lz78;


}


static void printInvalid() {
      printf("Invalid format. --help for more info.\n");
      exit(0);
      return;
}

 static Mode getMode(int argc, char *argv[]) {
      if (argc < 2) {
            printf("Invalid format. --help for more info.\n");
            exit(0);
            return invalid;
      }

      if (strcmp(argv[1], "index") == 0) return INDEX;
      if (strcmp(argv[1], "search") == 0) return SEARCH;
      if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printHelp();
            exit(0);
            return invalid;
      }

      return invalid;
}



int main(int argc, char *argv[])
{
      RunInfo info; 

      info.runMode = getMode(argc, argv);

      switch(info.runMode) {
            case INDEX: {
                  if (argc == 3) {
                        info.textFile = string(argv[2]);
                  } else {
                        printf("Invalid format. --help for more info.\n");
                        exit(0);
                  }

                  indexTextFile(info);
                  break;
            }
            case SEARCH: {
                  int opt, opt_index; /* opt = value returned by the getopt_long function | opt_index = index of the chosen option, stored by the getopt_long function */
                  /* {long option name, argument, flag value, short option name} */
                  static struct option long_options[]{
                        {"count",     no_argument,       0, 'c'},
                        {"help",      no_argument,       0, 'h'},
                        {"pattern",   required_argument, 0, 'p'},
                        {0, 0, 0, 0}
                  };

                  while((opt = getopt_long(argc, argv, "p:ch", long_options, &opt_index)) != -1){ 
                        switch(opt) {
                              case 'p':{
                                    info.patterns = parserPatternFile(optarg);
                                    break;
                              } 
                              case 'c': {
                                    info.isCountMode = true;
                                    break;
                              }
                              case 'h':{
                                    printHelp();
                                    exit(0);
                              }
                              case '?':{
                                    exit(0);
                                    break;
                              }
                              default: {
                                    abort();
                                    break;
                              }
                        }
                  }

                  int neededArgs = (info.patterns.empty()) ? 2 : 1;

                  if (argc - optind != neededArgs) {
                        printInvalid();
                        exit(1);
                  }
                  
                  if (info.patterns.size() == 0) 

                  if(info.patterns.empty()) info.patterns.push_back(argv[optind++]);

                  info.textFile = string(argv[optind++]);

                  decompressAndSearch(info);
            }
      }




      return 0;
}
