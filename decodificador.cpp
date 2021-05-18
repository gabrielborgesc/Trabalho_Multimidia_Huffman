#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class Input{

private:
    string magicNumber;
    int colNumber;
    int lineNumber;
    string maxval;
    map<string, int> codesMapping;
    vector<string> codes;

    vector<int> getValuesFromLine(string line){
        string aux = "";
        vector<int> values;
            for(int i=0; i<line.size(); i++){
                if( aux != "" && (line[i] == ' ')){
                    int value = stoi(aux);
                    values.push_back(value);
                    aux = "";
                }
                else{
                    aux+=line[i];
                }
            }
            return values;
    }

    void mapLine(string line){
        string aux = "";
        string code;
        int value;
            for(int i=0; i<line.size(); i++){
                if( aux != "" && line[i] == ' '){
                        if(i == line.size() - 1){
                            value = stoi(aux);
                        }
                        else{
                            code = aux;
                        }
                    aux = "";
                }
                else{
                    aux+=line[i];
                }
            }
            codesMapping[code]=value;
    }

public:

    string getMagicNumber(){
        return magicNumber;
    }

    int getColNumber(){
        return colNumber;
    }

    string getMaxval(){
        return maxval;
    }

    int getLineNumber(){
        return lineNumber;
    }

    vector<string> getCodes(){
        return codes;
    }

    map<string, int> getCodesMapping(){
        return codesMapping;
    }

    void readFile(string filename){
        ifstream file(filename);
        string line;
        getline(file, magicNumber); //magicNumber
        getline(file, line); //col line
        vector<int> colLine = getValuesFromLine(line);
        colNumber = colLine[0];
        lineNumber = colLine[1];
        getline(file, maxval); //maxval

        while(1){ //read mapping
            getline(file, line);
            if(line == "")
                break;
            mapLine(line);

        }

        while(getline(file, line)){ //read code sequence
            codes.push_back(line);
        }
    }

};

class Output{

public:
    void writeFile(string filename, Input input){

        ofstream fileOut(filename);

        fileOut<<input.getMagicNumber()<<endl;
        fileOut<<input.getColNumber()<<"  "<<input.getLineNumber()<<endl;
        fileOut<<input.getMaxval()<<endl;

        vector<string> codes = input.getCodes();
        map<string, int> codesMapping = input.getCodesMapping();
        int colNumber = input.getColNumber();

        for(int i = 0; i < codes.size(); i++){
            fileOut << codesMapping[codes[i]]<<" ";
            if( (i+1) %  colNumber == 0)
                fileOut<<endl;
        }
        fileOut.close();
    }

};

int main () {

    string filenames[2] = {"lena.ascii", "baboon_ascii"};
    for(int i=0;i<2;i++){
        string inputFile = filenames[i]+".huff";
        string outputFile = filenames[i]+".huff.pgm";

        Input input = Input();
        input.readFile(inputFile);

        Output output = Output();
        output.writeFile(outputFile, input);
    }


}
