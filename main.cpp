#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>

void prtVec(std::vector<double> vec) {
    for (double i : vec) {
        std::cout << i << ',';
    }
    std::cout << '\n';
}

std::vector<double> makeFreq(double len, double freq, double str) {
    std::vector<double> vec (len);
    for (int i = 0 ; i < len ; i++) {
        vec[i] = str * std::sin(std::numbers::pi*(double)i*freq*2/len);
        // std::cout << vec[i] << '\n';
    }
    return vec;
}

std::vector<double> makeFreqs(double len, std::vector<double> freqs, double str) {
    std::vector<double> vec (len);
    for (int i = 0 ; i < len ; i++) {
        vec[i] = 0;
        for (int j = 0 ; j < (int)freqs.size() ; j++) {
            vec[i] += str * std::sin(std::numbers::pi*(double)i*freqs[j]*2/len);
        }
        // std::cout << vec[i] << '\n';
    }
    return vec;
}

std::vector<double> cmbnFreq(std::vector<double> vec1, std::vector<double> vec2) {
    if (vec1.size() != vec2.size()) {
        std::cout << "Inputs are different length";
        return {};
    }
    std::vector<double> out (vec1.size());
    for (int i = 0 ; i < (int)out.size() ; i++) {
        out[i] = vec1[i] + vec2[i];
    }
    return out;
}

std::vector<double> wrap(std::vector<double> inp, double speed) {
    std::vector<double> pos (inp.size());
    if (speed == 0) {
        for (int i = 0; i < (int)inp.size() ; i++) {
            pos[i] = inp[i];
        }
    } else {    
        for (int i = 0; i < (int)inp.size() ; i++) {
            pos[i] = inp[i] * std::cos(((2 * speed) - 1) * (double)i * std::numbers::pi / inp.size());
        }
    }
    return pos;
}

double avg(std::vector<double> vec) {
    double average {0};
    for (int i : vec) {
        average += i;
    }
    average = average/vec.size();
    return average;
}

int main(int argc, char* argv[]) {
    // std::ignore = argc;
    // std::ignore = argv;
    std::cout << "FFT!\n";
    int inputLen {10};
    if (argc > 1) {
        inputLen = std::stoi((std::string)argv[1]);
    }
    // std::cout << std::stoi((std::string)argv[1]) << '\n';

    // std::vector<double> freq1 {makeFreq(1000, 349, 100)};
    // std::vector<double> freq2 {makeFreq(1000, 370, 100)};
    // std::vector<double> freq3 {makeFreq(1000, 440, 100)};
    // std::vector<double> input {cmbnFreq(freq1, freq2)};
    
    // std::vector<double> input {makeFreq(100, 3, 10)};
    // std::vector<double> input {makeFreqs(1000, {11,349,370,440}, 100)};
    // std::vector<double> input {10,10,10,10,10,-10,-10,-10,-10,-10,10,10,10,10,10,-10,-10,-10,-10,-10,0};
    std::vector<double> input {makeFreq(inputLen,1,10)};
  
    std::vector<double> pos (input.size());

    pos = wrap(input, 0);

    // if (rotSpd != -1) {
    //     pos = wrap(input, rotSpd);
    //     std::cout << "avg for " << rotSpd << ": " << avg(pos) << '\n';
    // }
    int max {};
    int maxIndex {};
    int currAvg {};
    for (int rotSpd {0} ; rotSpd <= std::floor(input.size()/2) ; rotSpd++) {
        pos = wrap(input, rotSpd);
        currAvg = std::abs(avg(pos));
        if (currAvg > max) {
            maxIndex = rotSpd;
            max = currAvg;
        }
        // std::cout << rotSpd << "avg: " << std::abs(avg(pos)) << '\n';
    }
    std::cout << maxIndex << ": " << max << '\n';
    
    return 0;
}