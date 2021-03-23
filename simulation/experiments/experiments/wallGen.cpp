#include <fstream>  // in/out files
#include <string>   // string operations
#include <vector>   // vector
#include <stdlib.h> // rand
#include <time.h>   //time
using namespace std;

void argosWall(string id, string position, vector<string>& lines) {
    // generate pseudo random numbers between kMin and kMax (wall size)
    float xSize = 1.2 + ((rand() %120) / 100);
    float ySize = 0.1 + ((rand() %40) / 100);
    if (rand()%2 == 1 ) {
        xSize = xSize + ySize;
        ySize = xSize - ySize;
        xSize = xSize - ySize;
    }
    lines.push_back("    <box id= \" " + id+  "\" size=\""+to_string(xSize)+ ","+to_string(ySize)+",2\" movable=\"false\">");
    lines.push_back("      <body position=\""+position+"\" orientation=\"0,0,0\" />");
    lines.push_back("    </box>");
}

void readContentsUntil(string const& filename,string const& stopLine, vector<string>& lines){
    string line ="";
    ifstream infile;
    infile.open(filename);
    while(getline(infile,line)) {
        lines.push_back(line);
        if(line == stopLine) 
            break;
    }
}
void readContentsFrom(string const& filename,string const& startLine, vector<string>& lines){
    string line ="";
    ifstream infile;
    bool startPush = false;
    infile.open(filename);
    while(getline(infile,line)) {
        if(line == startLine) 
            startPush = true;
        if(startPush) 
            lines.push_back(line);
    }
}
void updateContents(vector<string>& lines){
    const int MAX_WALLS = 10;
    const int MIN_WALLS = 7;
    unsigned nWall = rand() % (MAX_WALLS) + MIN_WALLS;
    for (unsigned i =0; i<nWall; ++i) {
        float x = (float) (rand()) /( (float) (RAND_MAX/8.4)) - 4.2;
        float y = (float) (rand()) /( (float) (RAND_MAX/9.6)) - 4.8;
        string pos = to_string(x) + ", " +to_string(y) + ", 0";
        argosWall("wall"+ to_string(i), pos, lines);
    }
}

void WriteContents(string const& filename,vector<string> const& lines){
    ofstream file;
    file.open(filename);
    for (auto & element : lines) {
        file << element <<endl;
    }
    file.close();
}

int main() {
    srand(time(NULL));
    const string filename  = "demo_pdr.argos";
    const string startLine = "    <!-- walls insertion starts here -->";
    const string stopline  = "    <!-- walls insertion ends here -->";
    vector<string> content;
    readContentsUntil(filename, startLine, content);
    updateContents(content);
    readContentsFrom(filename,stopline,content);
    WriteContents(filename, content);
    return 0;
}
