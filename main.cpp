#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
#include <iomanip>
#include <chrono>

void prtVec1D(std::vector<double> vec) {
    for (double i : vec) {
        std::cout << i << ',';
    }
    std::cout << '\n';
}

void prtVec2D(std::vector<std::vector<double>> vec) {
    for (std::vector<double> i : vec) {
        std::cout << i[0] << " , " << i[1] << '\n';
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

std::vector<double> dctWrap(std::vector<double> inp, double speed) {
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

// returns array of wrapped positions
std::vector<std::vector<double>> dftWrapSlow(std::vector<double> inp, double speed) {

    std::vector<std::vector<double>> pos (inp.size(), std::vector<double> (2));

    if (speed == 0) {
        for (int i = 0; i < (int)inp.size() ; i++) {
            pos[i][0] = inp[i];
        }
    } else {    
        for (int i = 0; i < (int)inp.size() ; i++) {
            pos[i][0] = inp[i] * std::cos(((2 * speed) - 1) * (double)i * std::numbers::pi / inp.size());
            pos[i][1] = inp[i] * std::sin(((2 * speed) - 1) * (double)i * std::numbers::pi / inp.size());
        }
    }
    return pos;
}

// adds positions immediately after calculation and returns that value
std::vector<double> dftWrap(std::vector<double> inp, double speed) {

    std::vector<double> pos (2);

    if (speed == 0) {
        for (int i = 0; i < (int)inp.size() ; i++) {
            pos[0] += inp[i];
        }
    } else {    
        for (int i = 0; i < (int)inp.size() ; i++) {
            pos[0] += inp[i] * std::cos(((2 * speed) - 1) * (double)i * std::numbers::pi / inp.size());
            pos[1] += inp[i] * std::sin(((2 * speed) - 1) * (double)i * std::numbers::pi / inp.size());
        }
    }
    return pos;
}

double avg1D(std::vector<double> vec) {
    double average {0};
    for (double i : vec) {
        average += i;
    }
    // average = average/vec.size();
    return average;
}

std::vector<double> avg2D(std::vector<std::vector<double>> vec) {
    std::vector<double> average {0,0};
    for (std::vector<double> i : vec) {
        average[0] += i[0];
        average[1] += i[1];
    }
    average[0] = average[0]/vec.size();
    average[1] = average[1]/vec.size();
    return average;
}

std::vector<std::vector<double>> dft(std::vector<double> inp) {

    std::vector<std::vector<double>> out (inp.size(), std::vector<double> (2));
    std::vector<double> pos (2);

    // std::vector<double> pos (inp.size());
    
    for (int rotSpd {0} ; rotSpd < (int)inp.size() ; rotSpd++) {
        pos = dftWrap(inp, rotSpd);
        out[rotSpd][0] = pos[0];
        out[rotSpd][1] = pos[1];
    }

    return out;
}

std::vector<std::vector<double>> fft(std::vector<std::vector<double>> inp) {
    int n {(int)inp.size()};
    if (n <= 1) {return inp;}

    std::vector<std::vector<double>> out (inp.size(), std::vector<double> (2));

    // Split the input
    std::vector<std::vector<double>> even (n/2, std::vector<double> (2)); // [0,2,4,...]
    std::vector<std::vector<double>> odd (n/2, std::vector<double> (2));; // [1,3,5,...]
    for (int i {0} ; i < n/2 ; i++) {
        even[i] = inp[2*i];
        odd[i] = inp[2*i+1];
    }

    // Recursive call
    even = fft(even);
    odd = fft(odd);

    // "Twiddle"
    std::vector<double> rotFac {0, 0};
    std::vector<double> rotOdd {0, 0};
    
    // Recombine
    for (int i {0} ; i < n/2 ; i++) {
        rotFac = {std::cos(2 * i * std::numbers::pi / n), -std::sin(2 * i * std::numbers::pi / n)};

        // basically (x+1)(x+2) but imaginary instead.
        rotOdd[0] = (odd[i][0] * rotFac[0]) - (odd[i][1] * rotFac[1]); // minus due to multiplying two imaginary #
        rotOdd[1] = (odd[i][0] * rotFac[1]) + (odd[i][1] * rotFac[0]);

        // real
        out[i][0] = even[i][0] + rotOdd[0];
        out[i+(n/2)][0] = even[i][0] - rotOdd[0];

        // imaginary
        out[i][1] = even[i][1] + rotOdd[1];
        out[i+(n/2)][1] = even[i][1] - rotOdd[1];
    }

    return {out};
}

int main(int argc, char* argv[]) {
    std::cout << "FFT!\n";
    std::string testAlg {};
    int repeats {5};
    int inputLen {5};
    int inputFreq {1};
    if (argc > 1) {
        if ((std::string)argv[1] == "test") {
            testAlg = (std::string)argv[2];
            inputLen = std::stoi((std::string)argv[3]);
        } else {
            inputLen = std::stoi((std::string)argv[1]);
        }
    }
    inputLen = std::pow(2,inputLen);

    // { test inputs
        std::vector<double> freq1 {makeFreq(1000, 349, 100)};
        std::vector<double> freq2 {makeFreq(1000, 370, 100)};
        std::vector<double> freq3 {makeFreq(1000, 440, 100)};
        std::vector<double> testInput1 {cmbnFreq(freq1, freq2)};

        std::vector<double> testInput2 {makeFreqs(1000, {349,370,440}, 100)};
        
        std::vector<double> testInput3 {makeFreq(100, 3, 10)};
        std::vector<double> testInput4 {10,10,10,10,10,-10,-10,-10,-10,-10,10,10,10,10,10,-10,-10,-10,-10,-10};
        std::vector<double> testInput5 {makeFreqs(64, {5,9}, 10)};
    // }
    
    std::cout << std::fixed << std::setprecision(2);
    
    std::cout << "Input length: " << inputLen << '\n';
    
    // 1D input (for DFT)
    std::vector<double> input {makeFreq(inputLen,inputFreq,10)};

    // 2D input (for FFT)
    std::vector<std::vector<double>> input2D {input.size(), std::vector<double> (2)};
    for (int i = 0 ; i < inputLen ; i++) {
        input2D[i][0] = input[i];
    }

    // DFT AND FFT

    if (testAlg == "dft") {
        std::vector<int> durations (repeats);
        for (int i {0} ; i < repeats ; i++) {
            auto begin {std::chrono::high_resolution_clock::now()};
            std::vector<std::vector<double>> dftResult {dft(input)};
            auto end {std::chrono::high_resolution_clock::now()};
            
            auto duration {std::chrono::duration_cast<std::chrono::microseconds>(end-begin)};
            
            durations[i] = duration.count();
            
            double max {};
            int maxIndex {};
            for (int i {0} ; i < (int)dftResult.size() ; i++) {
                if (dftResult[i][0] > max) {
                    maxIndex = i;
                    max = dftResult[i][0];
                }
            }
            std::cout << maxIndex << ": " << max << '\n';
            
            if (inputFreq != maxIndex) {
                std::cout << "MISMATCH IN INPUT AND OUTPUT FREQUENCY\n";
            }
        }
        std::cout << "Times in us\n";
        for (int i : durations) {
            std::cout << i << ',';
        }
        std::cout << '\n';
    }

    if (testAlg == "fft") {
        std::vector<int> durations (repeats);
        for (int i {0} ; i < repeats ; i++) {
            auto begin {std::chrono::high_resolution_clock::now()};
            std::vector<std::vector<double>> fftResult {fft(input2D)};
            auto end {std::chrono::high_resolution_clock::now()};
            
            auto duration {std::chrono::duration_cast<std::chrono::microseconds>(end-begin)};
            
            durations[i] = duration.count();
            
            double peak {};
            int peakIndex {};
            for (int i {0} ; i < inputLen ; i++) {
                if (fftResult[i][1] < peak) {
                    peak = fftResult[i][1];
                    peakIndex = i;
                }
            }
            std::cout << peakIndex << ": " << peak << '\n';
            
            if (inputFreq != peakIndex) {
                std::cout << "MISMATCH IN INPUT AND OUTPUT FREQUENCY\n";
            }
        }
        std::cout << "Times in us\n";
        for (int i : durations) {
            std::cout << i << ',';
        }
        std::cout << '\n';
    }
        
    return 0;
}