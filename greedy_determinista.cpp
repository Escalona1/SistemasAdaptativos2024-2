#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <tuple>

using namespace std;
using namespace std::chrono;


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

tuple<string, string, int, float> greedy_ffmsp(ifstream* file, float t) {
    vector<char> sigma = {'A', 'C', 'G', 'T'};
    vector<int> diffs;
    string s, initial_s;
    *file >> s;
    initial_s = s;
    //cout << "start: " << s << endl;
    int n = 0, m =  s.length(), th = m * t, changes = 0;
    float quality = 0;
    string seq;

    file->clear();
    file->seekg(0, ios::beg);

    //calcular hamming distance para cada s de omega y guardarlo en un vector "diffs"
    for(int i=0; file->good();i++){
		*file >> seq;
        if(seq == "")continue;
        //cout << seq << endl;
		diffs.push_back(hammingDist(s,seq));
        seq = "";
        n++;
	}

    //cout << n << " " << m << endl;

    vector<vector<int>> colitions(m, vector<int>(4)); // A C G T

    /*for(int i=0; i<diffs.size();i++){
        cout << "i: " << i << " " << diffs.at(i) << endl;
    }
    
    vector<int>::iterator min = min_element(diffs.begin(), diffs.end()); 
    cout << "min element has value " << *min << " and index ["<< distance(diffs.begin(), min) << "]\n";*/

    file->clear();
    file->seekg(0, ios::beg);
    seq = "";

    // Para cada posición j se calcula la cantidad de A, G, T o C que hay en esa posicion
    for (int i = 0; i < n; i++) {
        *file >> seq;
        for(int j=0; j<m;j++){
            if(seq[j] == 'A')colitions[j][0]++;
            else if(seq[j] == 'C')colitions[j][1]++;
            else if(seq[j] == 'G')colitions[j][2]++;
            else if(seq[j] == 'T')colitions[j][3]++;
        }
    }

    /*for (int i = 0; i < 4; i++) {
        for(int j=0; j<m;j++){
            cout << colitions[j][i] << ".";
        }
        cout << endl << endl;
    }*/

    for(int i=0;i<m;i++){
        //se selecciona el caracter con menor colisiones
        int min = colitions[i][0], index = 0;
        for(int j=1;j<4;j++){
            if(colitions[i][j] < min){
                min = colitions[i][j];
                index = j;
            }
        }
        //se cambia el valor en la solucion por aquel con menor numero de colisiones y se actualiza el vector "diffs"
        if(s[i] != sigma[index]){
            char prevC = s[i];
            s[i] = sigma[index];
            file->clear();
            file->seekg(0, ios::beg);
            seq = "";
            *file >> seq;
            for(int j=1;j<n;j++){
                *file >> seq;
                if(seq[i] != sigma[index] && prevC == seq[i]){
                    diffs[j]++;
                }else if(seq[i] == sigma[index]){
                    diffs[j]--;
                }
            }
            changes++;
        }
       //si el valor minimo del vector "diffs" supera el threshold solicitado, se detiene el algoritmo para ahorra tiempo
        vector<int>::iterator min_diff = min_element(diffs.begin()+1, diffs.end());
        //cout << *min_diff << endl;
        if(*min_diff >= th)break;
    }
    /*for(int i=0; i<diffs.size();i++){
        cout << "i: " << i << " " << diffs.at(i) << endl;
    }*/
    for(int i=0; i<diffs.size();i++){
        if(diffs[i] >= th)quality++;
    }
    quality = (quality/n)*100;
    /*vector<int>::iterator min_diff = min_element(diffs.begin()+1, diffs.end());
    cout << distance(diffs.begin(), min_diff) << ": " << *min_diff << " th: " << th  << endl;*/
    return make_tuple(initial_s, s, changes, quality);
}


int main(int argc, char *argv[])
{
    if(argc < 5){
        cout << "faltan argumentos" << endl;
        return -1;
    }
    string initial_s, s;
    int changes;
    float quality;
    string i_arg = argv[1];
    string str_file = argv[2];
    string th_arg = argv[3];
    float th = stof(argv[4]);
    ifstream myfile(str_file);

    auto start = high_resolution_clock::now();
	tie(initial_s, s, changes, quality) = greedy_ffmsp(&myfile, th);
	auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << "start: " << initial_s << endl;
    cout << "ended: " << s << endl;
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    cout << "total letters changed: " << changes << endl;
    cout << "cardinality of Ps: " << quality << "%" << endl;
	myfile.close();
    return 0;
}