#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "rc4.h"

using namespace std;

// Reading file input function, return String
string readFile(string file) {
    string line;
    stringstream ss;
    int i = 0;

    ifstream inFile (file);
    if(inFile.is_open()) {
        while (getline (inFile, line)) {
            if(i != 0)
                ss << '\n';
            ss << line ;
            i++;
        }
        inFile.close();
    } else {
        throw runtime_error("Could not open file");
    }

    return ss.str();
}


string useRC4(string str, int size, string cle){
    RC4 rc4;
    int key[256];
    rc4.KSA(cle, key);
    return rc4.PRGA(key, str, size);
}

void threadFunction(int start, int final, string* result, string* strInput, int size, int last, string cle){
    int thisSize = 100;
    for(int i = start; i < final; i++){
        if(last == 1 & i == final-1)
            thisSize = size;
        result[i] = useRC4(strInput[i], thisSize, cle);
    }
}


int main(int argc, char **argv) {
    if(argc != 6) {
        printf("Erreur argument");
        return -1;
    }

    // Files names
    string fileNameIn = argv[3];
    string fileNameOut = argv[4];

    // Read input
    string file = readFile(fileNameIn);

    int nbThread = atoi(argv[2]);

    if(nbThread == 1) {
        // RC4
        RC4 rc4;
        int key[256];
        rc4.KSA(argv[5], key);
        string res = rc4.PRGA(key, file, file.size());

        // Buffer to write in file
        //unsigned long bufferSize = res.size();
        char* buffer = new char[res.size()];
        for(int j = 0; j < res.size(); j++)
            buffer[j] = res[j];

        if(strcmp(argv[1], "-e") == 0) {
            ofstream outFile(fileNameOut + ".rc4");
            outFile.write(buffer, res.size());
            outFile.close();
        } else if(strcmp(argv[1], "-d") == 0){
            fstream outFile(fileNameOut + ".decrypt");
            outFile.write(buffer, res.size());
            outFile.close();
        }

        return 0;
    }


    int sizeBlockStr = 1000000000;
    int sizeOfBlock;
    if(file.size()%sizeBlockStr == 0){
        sizeOfBlock = file.size()/sizeBlockStr;
    } else {
        sizeOfBlock = (file.size()/sizeBlockStr)+1;
    }

    string strInput[sizeOfBlock];
    string result[sizeOfBlock];


    int position = 0;

    for(int i = 0; i < sizeOfBlock; i++){
        strInput[i] = file.substr (position,sizeBlockStr);
        position += 5;
    }



    int partition = sizeOfBlock / nbThread;

    cout << sizeOfBlock << endl;

    int st = 0;
    int fn = st + partition;
    int last = 0;

    for(int j = 0; j < nbThread; j++) {
        int size;
        if(j == nbThread-1) {
            size = (sizeBlockStr*sizeOfBlock) - file.size();
            cout << " Size 0 : " << size << endl;
            size = sizeBlockStr - size;
            cout << " Size 1 : " << size << endl;
        } else {
            size = sizeBlockStr;
            cout << " Size 2 : " << size << endl;
        }



        cout << "Thread : " << j << " Start at : " << st << " Ends at : " << fn << endl;
        thread t = thread(threadFunction, st, fn, &result[0], &strInput[0], size, last, argv[5]);
        t.join();

        int turn = nbThread-1;
        st = fn;

        if(j < nbThread - 2) {
            cout << " thread -1 " << endl;
            fn += partition;
        } else {
            cout << " thread final " << endl;
            fn = sizeOfBlock;
            last = 1;

        }




    }




    if(strcmp(argv[1], "-e") == 0) {
        ofstream outFile(fileNameOut + ".rc4");
        for(int x = 0; x < sizeOfBlock; x ++){
            outFile << result[x];
        }
        outFile.close();

    } else if(strcmp(argv[1], "-d") == 0){
        ofstream outFile(fileNameOut + ".decrypt");
        for(int x = 0; x < sizeOfBlock; x ++){
            outFile << result[x];
        }
        outFile.close();
    }

}

