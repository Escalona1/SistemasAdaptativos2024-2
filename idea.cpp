#include <iostream>
#include <string>
#include <vector>

using namespace std;

int hamdist(string st1, string st2){
    int count =0;
    for(int i = 0; i< str.length() ; i++){
        if( st1[i] == st2[i]){
            count++;
        }
    }
    return count;
}
int main(){
    vector<string> strings = {};
    char alfabeto[4] = {'A','C','G','T'};
    string solucion = strings[0];
    vector<int> diffbest;

    //calculamos la diferencia inicial de la solucion
    for(int i =1;i<strings.size(); i++){

        diffbest.push_back(hamdist(solucion,strings[i]));
    
    }

    for(int i = 0; i< solucion.length();i++){

        string nueva_solucion = solucion;
        vector<int> nuevo_vector_diferencia = diffbest;
        
        char modificado = solucion[i];

            for(int j =0;j<4;j++){
            
                solucion[i]= alfabeto[j];
                
                for(int j = 0;j<strings.size(),J=j++){
                if(s[i]!=modificado && !s[i]!=nueva_solucion[i]){
                    diffbest[i]++;
                }else if(s[i] == nueva_solucion[i]){
                 
                    diffbest[i]--;
                
                }
            }
        
        }

    }

}