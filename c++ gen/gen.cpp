#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <random>
#include <cstdint>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{

    fs::path outDir = "C:\\work\\test_project\\colibri_TP\\initialFiles";
    std::string fileName = "longlongtext.txt";
    std::size_t size = 100;


    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 255);

    std::vector<char> data(size);
    for (std::size_t i = 0; i < size; ++i) {
        data[i] = static_cast<char>(dist(rng));
    }

    fs::path filePath = outDir / fileName;
    std::ofstream out(filePath, std::ios::binary);
    if (!out) {
        std::cerr << "Cannot open for writing: " << filePath << "\n";
        return 1;
    }
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
    out.close();

    std::cout << "Generated: " << filePath
              << " (" << size << " bytes)\n";
    return 0;
}