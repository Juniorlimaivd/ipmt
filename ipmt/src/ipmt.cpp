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

enum Compress
{
      INVALID,
      lz78
};

enum Mode {
      INVALID,
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
void BuildIndex (RunInfo info){

LZ78 lz78;

for(string textname : info.textFiles) {
  std::string compressedFileName = lz78.ParseFileName(textname, "");
  std::string uncompressedFileName = lz78.ParseFileName(textname, UNCOMPRESSED_QUALIFIER);

  lz78.Compress(textname, compressedFileName); 
  //lz78.Decompress(compressedFileName, uncompressedFileName);         

}

}


void executeAlgorithm(RunInfo info){

      Searcher * s;
      std::vector<char> alph;

      for (int i = 0; i < 256; i++) {
            alph.push_back(char(i));
      }

      switch (info.chosenAlgorithm)
      {
            case SUFFIX_ARRAY:{
                  //s = new SuffixArray(info.patterns[0], alph);
                  break;
            }
            
      }
      int i=1;
      
      do {
            for(string textname : info.textFiles) {
                  ifstream fr(textname);

                  std::stringstream fullline;
                  
                  string line;

                  int ret ;
                  if(ret == -1) continue;

                  int lineCount = 0;
                  bool countedLine = false, found = false;

                  do {
                        if(!info.isCountMode) fullline << line;

                        found = s->search(line, ret == 0);

                        if (found && !countedLine) {
                              lineCount++;
                              countedLine = true;
                        }

                        if(ret == 0 && countedLine && !info.isCountMode)
                              printf("%s\n", fullline.str().c_str());
                        
                        if(ret == 0){
                              fullline.str(std::string());
                              countedLine = false;
                        }

                        ret = 0;

                  } while(ret != -1);

                  if (info.isCountMode) {

                        printf("Total occurrences: %d\n", s->count());
		

                  } else {
                        printf("Number of lines: %d\n", lineCount);
                  }

            }

            if(i < info.patterns.size()) s->resetPattern(info.patterns[i]);
            i++;
      } while( i <= info.patterns.size());


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
            return;
      }

      if (strcmp(argv[1], "index") == 0) return INDEX;
      if (strcmp(argv[1], "search") == 0) return SEARCH;
      if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printHelp();
            exit(0);
            return;
      }

      return;
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
