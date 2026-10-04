#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
#include <iomanip>

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

std::vector<std::vector<double>> dftWrap(std::vector<double> inp, double speed) {

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

std::vector<std::vector<double>> fft(std::vector<std::vector<double>> inp) {
    int n {(int)inp.size()};

    if (n <= 1) {return inp;}

    std::vector<std::vector<double>> out (inp.size(), std::vector<double> (2));

    std::vector<std::vector<double>> even (n/2, std::vector<double> (2)); // [0,2,4,...]
    std::vector<std::vector<double>> odd (n/2, std::vector<double> (2));; // [1,3,5,...]
    for (int i {0} ; i < n/2 ; i++) {
        even[i] = inp[2*i];
        odd[i] = inp[2*i+1];
    }
    // prtVec2D(even);
    // prtVec2D(odd);
    even = fft(even);
    odd = fft(odd);

    std::vector<double> rotFac {0, 0};
    std::vector<double> rotOdd {0, 0};
    
    for (int i {0} ; i < n/2 ; i++) {
        rotFac = {std::cos(2 * i * std::numbers::pi / n), -std::sin(2 * i * std::numbers::pi / n)};
        rotOdd[0] = (odd[i][0] * rotFac[0]) - (odd[i][1] * rotFac[1]); // minus due to multiplying two imaginary #
        rotOdd[1] = (odd[i][0] * rotFac[1]) + (odd[i][1] * rotFac[0]);
        out[i][0] = even[i][0] + rotOdd[0];
        out[i][1] = even[i][1] + rotOdd[1];
        out[i+(n/2)][0] = even[i][0] - rotOdd[0];
        out[i+(n/2)][1] = even[i][1] - rotOdd[1];
    }

    return {out};
}

int main(int argc, char* argv[]) {
    // std::ignore = argc;
    // std::ignore = argv;
    std::cout << "FFT!\n";
    int inputLen {5};
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
    std::vector<double> input {makeFreq(std::pow(2,inputLen),2,10)};

    std::cout << "len: " << input.size() << '\n';
    inputLen = (int)input.size();
    // std::vector<double> input {makeFreqs(64, {5,9}, 10)};
    
    std::vector<std::vector<double>> input2D {input.size(), std::vector<double> (2)};

    for (int i = 0 ; i < inputLen ; i++) {
        input2D[i][0] = input[i];
    }

    // std::vector<double> pos (input.size());
    std::vector<std::vector<double>> pos (inputLen, std::vector<double> (2));
    
    // prtVec2D(input2D);

    // std::vector<std::vector<double>> fftResult {fft(input2D)};
    std::cout << std::fixed << std::setprecision(1);
    // prtVec2D(fftResult);
    
    // double peak {};
    // int peakIndex {};
    // for (int i {0} ; i < inputLen ; i++) {
    //     if (fftResult[i][1] < peak) {
    //         peak = fftResult[i][1];
    //         peakIndex = i;
    //     }
    // }
    // std::cout << peakIndex << ": " << peak << '\n';

    double max {};
    int maxIndex {};
    int currAvg {};
    for (int rotSpd {0} ; rotSpd <= std::floor(input.size()) ; rotSpd++) {
        pos = dftWrap(input, rotSpd);
        // prtVec2D(pos);
        currAvg = std::abs(avg2D(pos)[0]);
        if (currAvg > max) {
            maxIndex = rotSpd;
            max = currAvg;
        }
        // std::cout << avg2D(pos)[0] << " , " << avg2D(pos)[1] << '\n';
        // std::cout << rotSpd << "avg: " << std::abs(avg2D(pos)[0]) << " , " << std::abs(avg2D(pos)[1]) << '\n';
    }
    std::cout << maxIndex << ": " << max << '\n';
    
    return 0;
}