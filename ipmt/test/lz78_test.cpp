#include "../src/lz78.hpp"
#include <cstdlib>

using namespace std;

int GetFileSize(std::string fileName){

    ifstream file(fileName.c_str());

    if(!file.is_open())
    {
        return -1;
    }

    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    file.close();

    return fileSize;
}

void TextfileTest(const std::string &textname){
  LZ78 lz78;
  std::vector<string> data;
  std::string compressedFileName = lz78.ParseFileName(textname, "");
  std::string uncompressedFileName = lz78.ParseFileName(textname, UNCOMPRESSED_QUALIFIER);

  lz78.Compress(textname, compressedFileName); 

  lz78.Decompress(compressedFileName, uncompressedFileName, data);    


  int compFileSize = GetFileSize(compressedFileName);
  int origFileSize = GetFileSize(textname); 
  double ratio = (compFileSize / (double) origFileSize);
  cout << "Compression ratio is: " << ratio << endl;     

}


int main(int argc, char const *argv[])
{
    TextfileTest(argv[1]);
    return 0;
}

