/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include <string>
#include <vector>
#include <fstream>
#include "SampleDecoder.h"

int hammingDist(const std::string &str1, const std::string &str2) {
    int count = 0;
    for (int i = 0; i < str1.size(); i++) {
        if (str1[i] != str2[i]) count++;
    }
    return count;
}
int fitness(const std::string &s, const std::vector<std::string> &omega, int th) {
    int count = 0;
    for (const auto &seq : omega) {
        if (hammingDist(s, seq) > th) count++;
    }
    return count;
}
SampleDecoder::SampleDecoder(double threshold) : th(threshold) { }

SampleDecoder::~SampleDecoder() { }

// Runs in \Theta(n \log n):
double SampleDecoder::decode(const std::vector< double >& chromosome) const {

	std::string sol;
	for(int i = 0; i < chromosome.size() ; i++){
		if(chromosome[i] <=0.25){
			sol.append("A");
		}
		else if (chromosome[i]<=0.5){
			sol.append("C");
		}
		else if (chromosome[i]<=0.75){
			sol.append("G");
		}
		else{
			sol.append("T");
		}
	}
	
	std::ifstream inputFile("../../FFMS_all_instances/100-300-001.txt");

	std::vector<std::string> omega;
    std::string seq;

	while (inputFile >> seq) {
        omega.push_back(seq);
    }
	int quality = fitness(sol,omega, th);
	double qual = (double)quality;
	
	return quality;
}
