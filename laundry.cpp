#include <iostream>
#include <fstream>
#include <algorithm>
#include "stack.h"

using namespace std;

int main(int argc, char* argv[])
{
  if(argc < 3){
    cerr << "Please provide the input and output filenames" << endl;
    return 1;
  }
  // Add your code here

    ifstream ifile(argv[1]);
    if(ifile.fail())
    {
        cout << "cannot open input file" << endl;
        return 0;
    }
    
    ofstream ofile(argv[2]);
    
    Stack<string> towels;
    int numTow = 0;
    int numIn;
    
    while(ifile >> numIn)
    {
        if (numIn == 0)
        {
            towels.push("black");
            numTow++;
        }
        else if (numIn == -1)
        {
            towels.push("gray");
            numTow++;
        }
        else
        {
            int times = min(numTow, numIn);
            for(int i=0; i < times; i++)
            {
                ofile << towels.top() << " ";
                towels.pop();
                numTow--;
            }
            ofile << endl;
        }
    }

  return 0;
}
