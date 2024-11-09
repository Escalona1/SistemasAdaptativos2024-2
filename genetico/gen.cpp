#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <tuple>
#include <chrono>

using namespace std;

// Función para calcular la distancia de Hamming
int hammingDist(const string &str1, const string &str2) {
    int count = 0;
    for (int i = 0; i < str1.size(); i++) {
        if (str1[i] != str2[i]) count++;
    }
    return count;
}

// Función de aptitud: cuenta cuántas secuencias en omega tienen una distancia de Hamming mayor que t con respecto a s
int fitness(const string &s, const vector<string> &omega, int th) {
    int count = 0;
    for (const auto &seq : omega) {
        if (hammingDist(s, seq) > th) count++;
    }
    return count;
}

// Generación aleatoria de una secuencia de longitud m
string randomSequence(int m, const vector<char> &sigma) {
    string s;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, sigma.size() - 1);

    for (int i = 0; i < m; i++) {
        s += sigma[dis(gen)];
    }
    return s;
}

// Cruce entre dos secuencias para producir un hijo
string crossover(const string &parent1, const string &parent2) {
    string child = parent1;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, parent1.size() - 1);

    for (int i = 0; i < parent1.size(); i++) {
        if (dis(gen) % 2 == 0) {
            child[i] = parent2[i];
        }
    }
    return child;
}

// Mutación en una secuencia con probabilidad de mutación
void mutate(string &s, const vector<char> &sigma, float mutationRate) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    uniform_int_distribution<> dis_char(0, sigma.size() - 1);

    for (int i = 0; i < s.size(); i++) {
        if (dis(gen) < mutationRate) {
            s[i] = sigma[dis_char(gen)];
        }
    }
}

// Algoritmo genético para el problema Far from Most String
tuple<string, int> genetic_ffmsp(const vector<string> &omega, int m, float t, int maxGenerations, int populationSize, float mutationRate) {
    vector<char> sigma = {'A', 'C', 'G', 'T'};
    int th = m * t;

    // Generar población inicial
    vector<string> population;
    for (int i = 0; i < populationSize; i++) {
        population.push_back(randomSequence(m, sigma));
    }

    // Algoritmo genético
    string bestSolution;
    int bestFitness = -1;

    for (int generation = 0; generation < maxGenerations; generation++) {
        vector<pair<string, int>> fitnessScores;

        // Evaluar la aptitud de cada individuo
        for (const auto &individual : population) {
            int score = fitness(individual, omega, th);
            fitnessScores.emplace_back(individual, score);
            if (score > bestFitness) {
                bestFitness = score;
                bestSolution = individual;
            }
        }

        // Selección por torneo
        vector<string> newPopulation;
        for (int i = 0; i < populationSize; i++) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, populationSize - 1);

            int a = dis(gen), b = dis(gen);
            string parent1 = fitnessScores[a].second > fitnessScores[b].second ? fitnessScores[a].first : fitnessScores[b].first;

            a = dis(gen); b = dis(gen);
            string parent2 = fitnessScores[a].second > fitnessScores[b].second ? fitnessScores[a].first : fitnessScores[b].first;

            // Cruce
            string child = crossover(parent1, parent2);

            // Mutación
            mutate(child, sigma, mutationRate);

            newPopulation.push_back(child);
        }

        // Reemplazar la población con la nueva
        population = move(newPopulation);

        // Imprimir la mejor solución de la generación actual
        cout << "Generation " << generation << " Best Fitness: " << bestFitness << endl;
    }

    return make_tuple(bestSolution, bestFitness);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <input_file> <t>" << endl;
        return -1;
    }

    ifstream inputFile(argv[1]);
    float t = stof(argv[2]);
    int maxGenerations = 1000, populationSize = 100;
    float mutationRate = 0.05;

    if (!inputFile) {
        cout << "Error opening file" << endl;
        return -1;
    }

    // Leer las secuencias desde el archivo
    vector<string> omega;
    string seq;
    while (inputFile >> seq) {
        omega.push_back(seq);
    }

    int m = omega[0].size();
    auto [bestSolution, bestFitness] = genetic_ffmsp(omega, m, t, maxGenerations, populationSize, mutationRate);

    cout << "Best Solution: " << bestSolution << endl;
    cout << "Best Fitness: " << bestFitness << endl;

    return 0;
}