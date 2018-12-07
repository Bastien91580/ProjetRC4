using namespace std;

class RC4 {
public:
    // Création de la clé ou matrice de permutation
    void KSA(string key, int* result){
        int i;
        int j = 0;
        int tmp;
        long keyLength = key.size();

        for(i = 0; i < 255; i++)
            result[i] = i;

        for(i = 0; i < 255; i++){
            j = (j + result[i] + key[i % keyLength]) % 256;
            tmp = i;
            i = j;
            j = tmp;
        }
    }



    string PRGA(int* S, string input, int size){
        int i = 0;
        int j = 0;
        int k = 0;

        //long sizeInput = strlen();
        string resultChiffrement;

        /*if(sizeInput != 100)
            cout << "Size Lecture : " << sizeInput << endl;*/

        int tmp;

        while(k < size){
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            tmp = S[i];
            S[i] = S[j];
            S[j] = tmp;

            int octChiff = S[(S[i] + S[j]) % 256];
            resultChiffrement += (char) octChiff xor (input[k]);

            k++;
        }

        return resultChiffrement;
    }

};