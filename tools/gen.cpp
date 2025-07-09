#include <cstdint>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct XorShift {
    vector<uint32_t> data;  // little-endian words

    explicit XorShift(uint64_t seed) {
        uint64_t x = seed + 1;
        data = {
            static_cast<uint32_t>(x & 0xffffffffu),
            static_cast<uint32_t>((x >> 32) & 0xffffffffu),
        };
    }

    void trim() {
        while (data.size() > 1 && data.back() == 0) {
            data.pop_back();
        }
    }

    void xor_shift_left(size_t bits) {
        size_t ws = bits / 32;
        size_t bs = bits % 32;
        size_t len = data.size();
        vector<uint32_t> temp(len + ws + 1, 0);
        for (size_t i = 0; i < len; ++i) {
            uint32_t w = data[i];
            temp[i + ws] ^= w << bs;
            if (bs != 0) {
                temp[i + ws + 1] ^= w >> (32 - bs);
            }
        }
        if (data.size() < temp.size()) {
            data.resize(temp.size(), 0);
        }
        for (size_t i = 0; i < temp.size(); ++i) {
            data[i] ^= temp[i];
        }
        trim();
    }

    void xor_shift_right(size_t bits) {
        size_t ws = bits / 32;
        size_t bs = bits % 32;
        size_t len = data.size();
        if (ws >= len) {
            return;
        }
        vector<uint32_t> temp(len - ws, 0);
        for (size_t i = ws; i < len; ++i) {
            uint32_t w = data[i];
            size_t idx = i - ws;
            temp[idx] ^= w >> bs;
            if (bs != 0 && i + 1 < len) {
                temp[idx] ^= data[i + 1] << (32 - bs);
            }
        }
        for (size_t i = 0; i < temp.size(); ++i) {
            data[i] ^= temp[i];
        }
        trim();
    }

    uint32_t next_u32() {
        xor_shift_left(13);
        xor_shift_right(7);
        xor_shift_left(17);
        return data[0];
    }
};

string gen_input(uint64_t seed) {
    XorShift rng(seed);
    const int N = 10;
    const int T = 1000;

    stringstream ss;
    ss << N << ' ' << T << '\n';
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            uint32_t val = rng.next_u32() >> 12;
            ss << val;
            if (j + 1 < N) {
                ss << ' ';
            }
        }
        ss << '\n';
    }
    return ss.str();
}

int main(int argc, char* argv[]) {
    string seeds_file;
    string dir = "in";
    bool verbose = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-d" || arg == "--dir") {
            if (i + 1 < argc) {
                dir = argv[++i];
            }
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg[0] == '-') {
            cerr << "Unknown option: " << arg << '\n';
            return 1;
        } else {
            seeds_file = arg;
        }
    }

    if (seeds_file.empty()) {
        cerr << "Usage: gen <seeds.txt> [-d DIR] [-v]" << endl;
        return 1;
    }

    if (!filesystem::exists(dir)) {
        filesystem::create_directory(dir);
    }

    ifstream fin(seeds_file);
    if (!fin) {
        cerr << "no such file: " << seeds_file << endl;
        return 1;
    }

    if (verbose) {
        cout << "file,seed" << endl;
    }

    string line;
    size_t id = 0;
    while (getline(fin, line)) {
        if (line.empty()) {
            continue;
        }
        uint64_t seed;
        try {
            seed = stoull(line);
        } catch (...) {
            cerr << "parse failed: " << line << endl;
            return 1;
        }

        string input = gen_input(seed);
        if (verbose) {
            cout << setw(4) << setfill('0') << id << ',' << seed << endl;
        }
        ostringstream name;
        name << setw(4) << setfill('0') << id << ".txt";
        ofstream fout(dir + '/' + name.str());
        fout << input;
        id++;
    }

    return 0;
}
