#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

int hammingDist(string str1, string str2) 
{ 
    int i = 0, count = 0; 
    while (str1[i] != '\0') { 
        if (str1[i] != str2[i]) 
            count++; 
        i++; 
    } 
    return count; 
} 

string greedy_ffmsp(ifstream* file, float t) {
    vector<char> sigma = {'A', 'C', 'G', 'T'};
    string s;
    *file >> s;
    int m =  s.length();
	int th = m * t;
    string seq, tempS;
    int maxCount, count;
    vector<char> bestChars;

    random_device rd;
    mt19937 gen(rd());

    // Para cada posición j en la secuencia s
    for (int j = 0; j < m; j++) {
        maxCount = 0;
        bestChars.clear();

        for (char c : sigma) {
            tempS = s;
            tempS[j] = c;

            count = 0;
            for(int i=0; file->good();i++){
			    *file >> seq;
				if(hammingDist(tempS, seq) > th){
                    count++;
                }
			}
            file->clear();
            file->seekg(0, ios::beg);

            if (count > maxCount) {
                maxCount = count;
                bestChars.clear();
                bestChars.push_back(c);
            } else if (count == maxCount) {
                bestChars.push_back(c);
            }
        }

        uniform_int_distribution<> dist(0, bestChars.size() - 1);
        s[j] = bestChars[dist(gen)];
    }

    return s;
}


int main(int argc, char *argv[])
{
	/*while (--argc > 0){
		printf("%s ", argv[argc]);
		printf("\n");
	}*/
    if(argc < 5){
        cout << "faltan argumentos" << endl;
        return -1;
    }
    string i_arg = argv[1];
    string str_file = argv[2];
    string th_arg = argv[3];
    float th = stof(argv[4]);

    ifstream myfile(str_file);
	string result = greedy_ffmsp(&myfile, th);
	cout << result << endl;
	myfile.close();
    return 0;
}

/*TODO
finalizar la lectura de argumentos
*/