#include <iostream>
#include <getopt.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "searcher.hpp"
#include "lz78.hpp"
#include "suffix_array.hpp"
#include <string.h>

using namespace std;


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

string vectorToString(std::vector<lli> v) {
      string s;
      stringstream ss;
      lli n = v.size();
      for (lli i = 0; i < n; ++i) {
            ss << (std::to_string(v[i]) + "|");
      }
      ss << "%";
      return ss.str();
}

vector<lli> stringToVector(std::string& s) {
      lli n = s.size();
      string temp;
      vector<lli> v;

      for (lli i = 0; i < n; ++i) {
            if (s[i] == '|') {
                  v.push_back(stoll(temp));
                  temp.clear();
            } else if (s[i] == '%') {
                  s = s.substr(i+1);
                  return v;
            } else temp += s[i];           
      }
      return v;
}

vector<string> parserPatternFile(string filename){
      vector<string> p;
      ifstream file(filename);

      if (file.fail()) {
            printf("Failed to pattern open file: %s.\n", filename.c_str());
            exit(0);
      } else {
            string pat;
            while (getline(file, pat)) {
                  p.push_back(pat);
            }
      }

      file.close();
      return p;
}

void printHelp() {
      printf("Usage: $ ipmt mode [options]\n");
      printf("INDEX: ipmt index textfile\n"
            "SEARCH: ipmt search pat indexfile\n"
            "-c, --count\n"
            "-h, --help\n"
            "-p, --pattern [pattern_file]\n");
}

string getText(std::string textfile) {
      
      FILE *file = fopen(textfile.c_str(), "rb");
      if (file == NULL) {
            printf("Couldn't read file: %s.", textfile.c_str());
            exit(0);
      }

      fseek(file, 0, SEEK_END);
      int tam = (int)ftell(file);
      fseek(file, 0, SEEK_SET);

      char *data = new char[tam + 1];
      data[tam] = '\0';
      size_t fread_result = fread(data, 1, tam, file);
      if (fread_result != (size_t) tam) {
            printf("Couldn't read file");
            exit(0);
      }
      fclose(file);
      return string(data);
    
}
void BuildIndex (RunInfo info){
      string text = getText(info.textFile);

      SuffixArray sa(text);

      LZ78 lz;
      stringstream ss;

      ss << vectorToString(sa._suffixArray);
      ss << vectorToString(sa._leftLCP);
      ss << vectorToString(sa._rightLCP);
      ss << sa._text;

      
      string raw = ss.str();
      
      lz.LZ78_Compress(raw);


      string outputFile = info.textFile.substr(0, info.textFile.size() - 4) + ".idx";

      lz.WriteToFile(outputFile);

}

void decompressAndSearch(RunInfo info) {
      string text;
      vector<lli> leftLCP, rightLCP, suffixArray;
      LZ78 lz;
      
      text = lz.Decode(info.textFile);

      suffixArray = stringToVector(text);
      leftLCP = stringToVector(text);
      rightLCP = stringToVector(text);       

      SuffixArray sa(suffixArray, text, leftLCP, rightLCP);
      
      lli count;
      for (lli i=0; i < info.patterns.size(); i++) {
            count = sa.search(info.patterns[i], !info.isCountMode);
            if (info.isCountMode) printf("Words found: %d\n", count);
      }
      
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
      optind++;
      info.patterns.clear();

      switch(info.runMode) {
            case INDEX: {
                  if (argc == 3) {
                        info.textFile = string(argv[2]);
                  } else {
                        printf("Invalid format. --help for more info.\n");
                        exit(0);
                  }

                  BuildIndex(info);
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
