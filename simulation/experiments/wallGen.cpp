#include <fstream>  // in/out files
#include <string>   // string operations
#include <vector>   // vector
#include <stdlib.h> // rand
#include <time.h>   //time
using namespace std;

void argosWall(string id, string position, vector<string>& lines) {
    lines.push_back("    <box id= \" " + id+  "\" size=\"0.2,0.2,2\" movable=\"false\">");
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
    const int MAX_WALLS = 30;
    const int MIN_WALLS =  5;
    const int POS_LIMIT =  2; // limit of the walls (assuming it's a square.)
    srand(time(NULL));
    unsigned nWall = rand() % (MAX_WALLS + MIN_WALLS) + MIN_WALLS; // minimum of 5 walls
    for (unsigned i =0; i<nWall; ++i) {
        int x = rand() % (2*POS_LIMIT) - POS_LIMIT; // between -pos_limit and + poslimit
        int y = rand() % (2*POS_LIMIT) - POS_LIMIT; // between -pos_limit and + poslimit
        string pos = to_string(x) + ", " +to_string(y) + ", 2";
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
