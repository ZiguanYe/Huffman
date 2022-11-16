#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include "huffman.h"
using namespace std;

// Complete
//takes in the filename and returns a vector containing the sequence of 4-hex digit strings representing each UTF-16 character
RawTextVector AsciiHexIO::read(const char* filename)
{
    RawTextVector text;
    ifstream ifile(filename);
    if(ifile.fail()){
        throw std::invalid_argument("Bad input filename");
    }
    while(!ifile.fail()) {
        char c;
        string word;
        for(int i=0; i < 4; i++){
            ifile >> c;
            if(ifile.fail()){
                if(i > 0) {
                    throw std::runtime_error(
                        "Bad file format...did not find multiple of 4 hex characters");
                }
                else {
                    break; // normal exit
                }
            }
            else {
                word += c;
            }
        }
        if(word.size() == 4) {
            text.push_back(word);
        }
    }
    ifile.close();
    return text;
}

// Complete
// takes a vector containing those 4-hex digit strings and writes them to a file
void AsciiHexIO::write(const char* filename, const RawTextVector&  text)
{
    const size_t NUM_WORDS_PER_LINE = 8;
    ofstream ofile(filename);
    if(ofile.fail()){
        throw std::runtime_error("Unable to open output file");
    }
    size_t i = 0;
    for( const auto & word : text ){
        ofile << word;
        if(i % NUM_WORDS_PER_LINE != NUM_WORDS_PER_LINE-1 ){
            ofile << " ";
        }
        else {
            ofile << endl;
        }
        i++;
    }
    ofile.close();
}

// Complete
//takes the compressed data (e.g. inText = 100011110111101011010) along with the code/key mapping (e.g. { 0:0069, 11:0073, 100:006d, 101:0070} ) and saves a text file with that data
void AsciiHuffmanIo::writeCodedText(
    const char* filename, 
    const CompressedData& inText, 
    const CodeKeyMap& code)
{
    ofstream ofile(filename);
    ofile << code.size() << endl;
    // copy from map to vector so we can sort
    typedef pair<string,string> StrStrPair;
    vector< StrStrPair > codesToSort(code.begin(), code.end());
    std::sort(
        codesToSort.begin(), codesToSort.end(), 
        [] (const StrStrPair& s1, const StrStrPair& s2) -> bool
            { return (s1.first.size() < s2.first.size()) ||
                     (s1.first.size() == s2.first.size() && s1.first < s2.first); } );
    for(const auto& pair : codesToSort) {
        ofile << pair.first << " " << pair.second << endl;
    }
    ofile << inText << endl;
    ofile.close();
}

// 4
// 0 0069
// 11 0073
// 100 006d
// 101 0070
// 100011110111101011010

// To be completed
//reads in files of the format created by writeCodedText and sets the CodeKeyMap and CompressedData string (outText) with the contents from the file
void AsciiHuffmanIo::readCodedText(
    const char* filename, 
    CompressedData& outText, 
    CodeKeyMap& code)
{
    // clear old contents of the output parameters that this function
    // should fill in
    code.clear();
    outText.clear();

    // Complete the code below
    ifstream ifile(filename);
    if(ifile.fail()){
        throw runtime_error("Cannot open file");
    }

    //read in number of keys
    int numKeys=0;
    ifile >> numKeys;
    
    if (ifile.fail())
    {
      cerr << "unable to read number of keys " << endl;
      ifile.close();
    }

    string codeLine;
    //skip first line
    getline(ifile, codeLine);
    
    //read the next numKeys lines
    for(int i=0; i<numKeys; i++)
    {
        getline(ifile, codeLine);
        stringstream ss(codeLine);
        
        string codeStr;
        string keyStr;
        ss >> codeStr >> keyStr;
        
        code.insert(make_pair(codeStr, keyStr));
    }
    
    //the last line of file is outText
    getline(ifile, outText);

}

// To be completed
void HuffmanCoder::compress(
    const RawTextVector& inText, 
    CompressedData& outText, 
    CodeKeyMap& codes)
{
    // Clear old contents of the output parameters that this function should fill in
    codes.clear();
    outText.clear();

    // Add your code here
    
    //新起一个map来记下frequency，读取intext的每一个元素，如果已经在freqMap里就++，不在就insert一个新的pair
    KeyFrequencyMap freqMap;
    for(RawTextVector::const_iterator It = inText.begin(); It!=inText.end(); ++It)
    {
        //the key has not be counted before
        if(freqMap.find(*It) == freqMap.end())
        {
            freqMap.insert(make_pair(*It, 1));
        }
        //frequency of this key increases by 1
        else
        {
            freqMap[*It]++;
        }
    }
    
    //for testing frequency:
//    for(KeyFrequencyMap::iterator It = freqMap.begin(); It!=freqMap.end(); ++It)
//    {
//        cout << It->first << "frequency is: " << It->second << endl;
//    }
    
    //freqMap<keys(string), frequency>, use this to make HeapItem and push it into heap
    //如果freqMap是正确的，创建一个新的binary min-heap，把存有key和freq的map内元素存成heapItem并push到heap里
    Heap<HeapItem> huffHeap(2);
    HeapItem newItem;
    
    for(KeyFrequencyMap::iterator It = freqMap.begin(); It!=freqMap.end(); ++It)
    {
        newItem.total = It->second;
        set<string> key;
        key.insert(It->first);
        newItem.keys = key;
        
        huffHeap.push(newItem);
    }
    
    //binary min-heap, where each item has total(size_t) and keys(set<string>)
    
    //create key-code map (map<"006h", "101">)
    KeyCodeMap kcMap;
    
    while(huffHeap.size()!=1)
    {
        //save first item and pop
        HeapItem temp1 = huffHeap.top();
        huffHeap.pop();
        //save second item and pop
        HeapItem temp2 = huffHeap.top();
        huffHeap.pop();

        //iterate the set of keys of the HeapItem
        for(set<string>::iterator It = temp1.keys.begin(); It!=temp1.keys.end(); ++It)
        {
            if(kcMap.find(*It)==kcMap.end())
            {
                kcMap.insert(make_pair(*It, "0"));
            }
            else
            {
                kcMap[*It] = "0" + kcMap[*It];
            }
        }
        
        for(set<string>::iterator It2 = temp2.keys.begin(); It2!=temp2.keys.end(); ++It2)
        {
            if(kcMap.find(*It2)==kcMap.end())
            {
                kcMap.insert(make_pair(*It2, "1"));
            }
            else
            {
                kcMap[*It2] = "1" + kcMap[*It2];
            }
        }
        
        HeapItem newItem;
        newItem.total = temp1.total + temp2.total;
        for(set<string>::iterator It2 = temp2.keys.begin(); It2!=temp2.keys.end(); ++It2)
        {
            temp1.keys.insert(*It2);
        }
        newItem.keys = temp1.keys;
        
        huffHeap.push(newItem);
    }
    
    
    for(RawTextVector::const_iterator It = inText.begin(); It!=inText.end(); ++It)
    {
        outText += kcMap[*It];
    }
    
    
    for(KeyCodeMap::iterator It = kcMap.begin(); It!=kcMap.end(); ++It)
    {
        codes.insert(make_pair(It->second, It->first));
    }


}

// To be completed
void HuffmanCoder::decompress(
    const CompressedData& inText, 
    const CodeKeyMap& codes, 
    RawTextVector& outText)
{
    // Clear old contents of the output parameter
    outText.clear();
    // Add your code here
    string inputCode;

    // create the decompressed text by reading one char at a time and, 
    // since Huffman codes are prefix codes, once the string matches 
    // a key in the code map, just substitute the word and start again
    
    string tmpCode="";
    
    for(CompressedData::const_iterator It=inText.begin(); It!=inText.end(); ++It)
    {
        tmpCode+=*It;
        //code not found in map, move on to the next iterator
        if(codes.find(tmpCode) == codes.end())
        {
            continue;
        }
        //code found in map
        else
        {
            outText.push_back(codes.at(tmpCode));
            tmpCode = "";
        }
    }
    
}

// Complete
double HuffmanCoder::ratio(const RawTextVector& inText, const CompressedData& outText, const CodeKeyMap& codes)
{
    // 2 bytes per entry
    double rawSize = 2 * inText.size(); 
    // each character in outText is a bit so convert to bytes
    double compressedSize = (outText.size() + 7) / 8; 
    for(const auto& pair : codes) {
        compressedSize += 2 + pair.first.size()+1; // 2 bytes for 16-bit val + string of 1,0 + null char
    }
    cout << "Original size (bytes): " << rawSize << endl;
 
    cout << "Compressed size (bytes): " << compressedSize << endl;
    double compressionRatio = rawSize / compressedSize;
    cout << "Compression ratio: " << setprecision(2) << fixed << compressionRatio << endl;
    return compressionRatio;
}
