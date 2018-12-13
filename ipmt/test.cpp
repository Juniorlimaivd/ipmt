
#include "src/suffix_array.hpp"
#include <iostream>
#include <string>

using namespace std;



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

int main() {
    string pat = "love";
    string txt = getText("shakespeare.txt");

    SuffixArray sa(txt);
    cout << sa._suffixArray.size() << endl;
    cout << sa.search(pat, false) << endl;

    return 0;
}