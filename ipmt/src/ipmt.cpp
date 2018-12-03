#include <iostream>
#include <getopt.h>
#include <string>
#include <vector>
#include <sstream>
#include "file_reader.hpp"
#include "searcher.hpp"



using namespace std;

enum Algorithm
{
      NONE,
      SUFFIX_TREE,
      SUFFIX_ARRAY,
};

enum Compress
{
      LZ77,
      LZ78,
};


typedef struct RunInfo {
      vector<string> patterns;
      vector<string> textFiles;
      int distance;
      bool isExact;
      bool isCountMode;
      Algorithm chosenAlgorithm;
			Compress chosenCompress;

} RunInfo;

vector<string> parserPatternFile(string filename){
      vector<string> p;
      FileReader f(filename);

      string fullline, line;

      int ret = f.getLine(line);
      if (ret == -1) return p;

      do {
            fullline += line;
            if (ret == 0){
                  if (!fullline.empty()) p.push_back(fullline);
                  fullline = "";
            }
            ret = f.getLine(line);
      } while(ret != -1);
}

Algorithm chooseAlgorithm(RunInfo info) {
		return SUFFIX_ARRAY;
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
            case SUFFIX_TREE:{
                  //s = new SuffixTree(info.patterns, alph);
                  break;
            }

            
      }
      int i=1;
      
      do {
            for(string textname : info.textFiles) {
                  FileReader fr(textname);

                  std::stringstream fullline;
                  
                  string line;

                  int ret = fr.getLine(line);
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

                        ret = fr.getLine(line);

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

int main(int argc, char *argv[])
{
      vector<string> pmt_algorithms = {"suffix-array", "suffix-tree"};
			vector<string> pmt_compress = {"lz77", "lz78"};
      int opt, opt_index; /* opt = value returned by the getopt_long function | opt_index = index of the chosen option, stored by the getopt_long function */
      string algorithm, compress_type;
      RunInfo info;
      info.chosenAlgorithm = NONE;
      info.distance = -1;
      info.isCountMode = false;
      info.isExact = true;
      info.patterns.clear();
      info.textFiles.clear();

      /* {long option name, argument, flag value, short option name} */
      static struct option long_options[]{
            {"algorithm", required_argument, 0, 'a'},
            {"count",     no_argument,       0, 'c'},
            {"lz",      required_argument, 0, 'l'},
            {"help",      no_argument,       0, 'h'},
            {"pattern",   required_argument, 0, 'p'}, 
            {0, 0, 0, 0}
      };

      while((opt = getopt_long(argc, argv, "a:l:cp:h", long_options, &opt_index)) != -1){
            switch(opt){

                  case 'a':
                  {
                        algorithm = optarg;
                        
                        for(int i=0; i < pmt_algorithms.size(); i++) {
                              if (pmt_algorithms[i] == algorithm) {
                                    info.chosenAlgorithm = Algorithm(i+1);
                              }      
                        }

                        if (info.chosenAlgorithm == NONE) {
                              printf("Invalid algorithm name.\n");
                              exit(1);
                        }
                  
                  break;
                  }

                  case 'c':                  
                        info.isCountMode = true;
                  break;

                  
                  case 'l':
                        compress_type = optarg;
                        
                        for(int i=0; i < pmt_compress.size(); i++) {
                              if (pmt_compress[i] == compress_type) {
                                    info.chosenCompress = Compress(i+1);
                              }      
                        }

                        if (info.chosenCompress == NONE) {
                              printf("Invalid compression algorithm name.\n");
                              exit(1);
                        }          
                  break;

                  case 'h':
                  
                        printf("Usage: $ pmt [options] pattern textfile [textfile...]\n");
                        printf("-a, --algorithm [algorithm_name]\n\t[suffix-tree]\n\t[suffix-array]\n"
                              "-c, --count\n"
                              "-l, --compression algorithm [compression_algorithm_name]\n\t[lz77]\n\t[lz78]\n"
                              "-h, --help\n"
                              "-p, --pattern [pattern_file]\n");
                        exit(0);
                        break;
                  break;

                  case 'p':                  
                        info.patterns = parserPatternFile(optarg);                  
                  break;

                  case '?':
                        printf("Invalid argument. Please try again.\n");
                        exit(1);
                  break;
                  default:
                        abort();
            }

      }

      int neededArgs = (info.patterns.empty()) ? 2 : 1;

      if (argc - optind < neededArgs) {
            printf("Not enought arguments.\n");
            exit(1);
      }

      if(info.patterns.empty()) info.patterns.push_back(argv[optind++]);

      for(; optind < argc; optind++) info.textFiles.push_back(argv[optind]);




      executeAlgorithm(info);

      return 0;
}
