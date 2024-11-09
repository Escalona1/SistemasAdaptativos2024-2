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

// Decodifica un vector de claves aleatorias a una secuencia sobre el alfabeto sigma
string decode(const vector<float> &keys, int m, const vector<char> &sigma) {
    string s;
    for (int i = 0; i < m; i++) {
        int idx = static_cast<int>(keys[i] * sigma.size()) % sigma.size();
        s += sigma[idx];
    }
    return s;
}

// Algoritmo genético basado en claves aleatorias sesgadas para el problema Far from Most String
tuple<string, int> brkga_ffmsp(const vector<string> &omega, int m, float t, int maxGenerations, int populationSize, float eliteFraction, float mutationRate) {
    vector<char> sigma = {'A', 'C', 'G', 'T'};
    int th = m * t;
    int eliteSize = static_cast<int>(populationSize * eliteFraction);

    // Generar población inicial de vectores de claves aleatorias
    vector<vector<float>> population(populationSize, vector<float>(m));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (auto &individual : population) {
        for (auto &key : individual) {
            key = dis(gen);
        }
    }

    string bestSolution;
    int bestFitness = -1;

    for (int generation = 0; generation < maxGenerations; generation++) {
        vector<pair<vector<float>, int>> fitnessScores;

        // Evaluar aptitud de cada individuo
        for (const auto &individual : population) {
            string decodedSeq = decode(individual, m, sigma);
            int score = fitness(decodedSeq, omega, th);
            fitnessScores.emplace_back(individual, score);

            if (score > bestFitness) {
                bestFitness = score;
                bestSolution = decodedSeq;
            }
        }

        // Ordenar por aptitud
        sort(fitnessScores.begin(), fitnessScores.end(), [](auto &a, auto &b) {
            return a.second > b.second;
        });

        // Selección de padres élite y no élite
        vector<vector<float>> newPopulation;
        for (int i = 0; i < eliteSize; i++) {
            newPopulation.push_back(fitnessScores[i].first); // Mantener élite
        }

        // Cruce sesgado entre padres élite y no élite
        uniform_int_distribution<> dis_elite(0, eliteSize - 1);
        uniform_int_distribution<> dis_non_elite(eliteSize, populationSize - 1);

        for (int i = eliteSize; i < populationSize; i++) {
            const auto &parent1 = fitnessScores[dis_elite(gen)].first;
            const auto &parent2 = fitnessScores[dis_non_elite(gen)].first;
            vector<float> child(m);

            for (int j = 0; j < m; j++) {
                child[j] = dis(gen) < 0.7 ? parent1[j] : parent2[j];
            }

            // Mutación
            for (auto &key : child) {
                if (dis(gen) < mutationRate) {
                    key = dis(gen); // Nueva clave aleatoria
                }
            }

            newPopulation.push_back(child);
        }

        // Reemplazar población con la nueva generación
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
    float eliteFraction = 0.2, mutationRate = 0.05;

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
    auto [bestSolution, bestFitness] = brkga_ffmsp(omega, m, t, maxGenerations, populationSize, eliteFraction, mutationRate);

    cout << "Best Solution: " << bestSolution << endl;
    cout << "Best Fitness: " << bestFitness << endl;

    return 0;
}