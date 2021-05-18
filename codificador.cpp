#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class Input {

private:

    string magicNumber;
    string colLine;
    string maxval;
    vector<int> allValues;
    vector<int> values;
    map <int, int> frequencyMapping;
    vector<int> frequencyList;

    bool isPresent(int value){
        for(int i=0; i< values.size(); i++)
            if(values[i] == value)
                return true;
        return false;
    }

public:

    string getMagicNumber(){
        return magicNumber;
    }

    string getColLine(){
        return colLine;
    }

    string getMaxval(){
        return maxval;
    }

    vector<int> getAllValues(){
        return allValues;
    }

    vector<int> getValues(){
        return values;
    }

    int getFrequencyMapping(int value){
        return frequencyMapping[value];
    }

    vector<int> getFrequencyList(){
        return frequencyList;
    }

    void readFile(string filename){
        //get pixels values
        ifstream file(filename);
        string line;
        getline(file, magicNumber); //P2
        getline(file, line); //Text
        getline(file, colLine); //col line
        getline(file, maxval); //maxval

        Input input = Input();

        while (getline(file, line)) { // read lines
            add(line);
        }

        file.close();

        generateFrequencyList();
    }

    void add(string line){
        string aux = "";
        for(int i=0; i<line.size(); i++){
            if( aux != "" && (line[i] == ' ' || line[i] == '\n')){
                int value = stoi(aux);
                if(!isPresent(value))
                    values.push_back(value);

                frequencyMapping[value]++;
                allValues.push_back(value);
                aux = "";
            }
            else{
                aux+=line[i];
            }
        }
    }

    void generateFrequencyList(){
        for(int i = 0; i < values.size(); i++){
            frequencyList.push_back(frequencyMapping[values[i]]);
        }
    }

    void print() {
        int t=0;
        cout<<"size: "<<allValues.size()<<endl;
        for(int i = 0; i<allValues.size(); i++){
                t+=i;
        }
        cout<<"t: "<<t;
        cout<<"rodou todos"<<endl;
    }

    float meanSize(map<int, string> codesMapping){
        float sum = 0;
        for(int i=0; i<values.size(); i++){
                int value = values[i];
            sum += codesMapping[value].size() * frequencyMapping[value];
        }
        return sum/allValues.size();
    }

};

class Output{

public:
    void writeFile(string filename, Input input, map<int, string> codesMapping){
        ofstream fileOut(filename);

        fileOut<<input.getMagicNumber()<<endl;
        fileOut<<input.getColLine()<<" "<<endl;
        fileOut<<input.getMaxval()<<endl;

        vector<int> values = input.getValues();
        for(int i=0; i<values.size(); i++){
            fileOut << codesMapping[values[i]] << " " <<values[i]<<" "<<endl;
        }

        fileOut<<endl;

        vector<int> allValues = input.getAllValues();
        for(int i=0; i<allValues.size(); i++){
            fileOut<<codesMapping[allValues[i]]<<endl;
        }
        fileOut.close();
    }

};

class Huffman{

    int freq;
    int value;
    Huffman *right, *left;

public:

    Huffman(){
        value = 0;
        right = NULL;
        left = NULL;
    }

    Huffman(Huffman& node){
        this->freq = node.freq;
        this->value = node.value;
        this->right = node.right;
        this->left = node.left;
    }

    void setFreq(int Freq){freq = Freq;}
    void setValue(int Value){value = Value;}
    void setLeft(Huffman *Left){left = Left;}
    void setRight(Huffman *Right){right = Right;}
    int getFreq(){return freq;}
    int getValue(){return value;}
    Huffman* getLeft(){return left;}
    Huffman* getRight(){return right;}
};

void BubbleSort(vector <Huffman*>& node){

    int i, j;

    Huffman *aux;

    aux = new Huffman();

    for(i=0; i<node.size(); i++)
        for(j=0; j<node.size(); j++){
            if(node[i]->getFreq() > node[j]->getFreq() && i<j){
                aux = node[i];
                node[i] = node[j];
                node[j] = aux;
            }
        }
}

void encode(Huffman *Tree, string &code, map<int, string> &codesMapping){
    if(Tree->getLeft() == NULL && Tree->getRight() == NULL){
        codesMapping[Tree->getValue()] = code;
        return;
    }

    code = code +'0';
    encode(Tree->getLeft(), code, codesMapping);
    code.erase(code.end()-1);

    code = code +'1';
    encode(Tree->getRight(), code, codesMapping);
    code.erase(code.end()-1);
}

Huffman *buildTree(vector <int> &values, vector <int> &freq, Huffman *Tree){
    vector <Huffman*> node(values.size());

    int i;

    for(int i=0; i<values.size(); i++){
        node[i] = new Huffman();
        node[i]->setFreq(freq[i]);
        node[i]->setValue(values[i]);
    }


    while(node.size()>1){
        BubbleSort(node);

        Huffman *h1, *h2;

        h1=node[0];
        h2=node[1];

        node.erase(node.begin());
        node.erase(node.begin());

        Huffman *aux = new Huffman();

        aux->setLeft(h1);
        aux->setRight(h2);
        aux->setFreq(h1->getFreq()+h2->getFreq());

        node.push_back(aux);
    }

    return node[0];
}

int main() {
    string filenames[2] = {"lena.ascii", "baboon_ascii"};
    for(int i=0;i<2;i++){
        string inputFilename = filenames[i]+".pgm";
        string outputFilename = filenames[i]+".huff";
        Input input = Input();
        input.readFile(inputFilename);

        vector<int> values = input.getValues();
        vector<int> freq = input.getFrequencyList();

       Huffman *Tree;
       Tree = buildTree(values, freq, Tree);


        string code = "";
        map <int, string> codesMapping;

        encode(Tree, code, codesMapping);

        float mean_size = input.meanSize(codesMapping);
        cout<<"tamanho medio das palavras codigo de "<<filenames[i]<<": "<<mean_size<<endl;

        Output output = Output();
        output.writeFile(outputFilename, input, codesMapping);

    }

    return 0;
}

