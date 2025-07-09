#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct XorShift {
    vector<uint32_t> data;

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
        vector<uint32_t> tmp(len + ws + 1, 0);
        for (size_t i = 0; i < len; ++i) {
            uint32_t w = data[i];
            tmp[i + ws] ^= w << bs;
            if (bs != 0) {
                tmp[i + ws + 1] ^= w >> (32 - bs);
            }
        }
        if (data.size() < tmp.size()) {
            data.resize(tmp.size(), 0);
        }
        for (size_t i = 0; i < tmp.size(); ++i) {
            data[i] ^= tmp[i];
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
        vector<uint32_t> tmp(len - ws, 0);
        for (size_t i = ws; i < len; ++i) {
            uint32_t w = data[i];
            size_t idx = i - ws;
            tmp[idx] ^= w >> bs;
            if (bs != 0 && i + 1 < len) {
                tmp[idx] ^= data[i + 1] << (32 - bs);
            }
        }
        for (size_t i = 0; i < tmp.size(); ++i) {
            data[i] ^= tmp[i];
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

struct Input {
    int n;
    int t;
    vector<vector<uint32_t>> board;
};

struct Output {
    vector<char> ops;
};

Input parse_input(const string& s) {
    istringstream iss(s);
    Input inp;
    iss >> inp.n >> inp.t;
    inp.board.assign(inp.n, vector<uint32_t>(inp.n));
    for (int i = 0; i < inp.n; ++i) {
        for (int j = 0; j < inp.n; ++j) {
            iss >> inp.board[i][j];
        }
    }
    return inp;
}

Output parse_output(const Input& inp, const string& s, string& err) {
    Output out;
    istringstream iss(s);
    string tok;
    while (iss >> tok) {
        if (tok.size() != 1) {
            err = "Invalid operation token: " + tok;
            return out;
        }
        char c = tok[0];
        if (string("UDLRWCH").find(c) == string::npos) {
            err = "Illegal operation: " + string(1, c);
            return out;
        }
        out.ops.push_back(c);
    }
    if (out.ops.size() > static_cast<size_t>(inp.t)) {
        err = "Operation count exceeds limit";
        out.ops.clear();
    }
    return out;
}

pair<int64_t, string> run(const Input& inp, const Output& out,
                          vector<vector<uint32_t>>& board) {
    board = inp.board;
    size_t x = 0, y = 0;
    uint32_t s = 0;
    size_t step = 0;
    for (char op : out.ops) {
        switch (op) {
        case 'U':
            if (x == 0) {
                return {0, "Move out of board at step " + to_string(step)};
            }
            --x;
            break;
        case 'D':
            if (x + 1 >= static_cast<size_t>(inp.n)) {
                return {0, "Move out of board at step " + to_string(step)};
            }
            ++x;
            break;
        case 'L':
            if (y == 0) {
                return {0, "Move out of board at step " + to_string(step)};
            }
            --y;
            break;
        case 'R':
            if (y + 1 >= static_cast<size_t>(inp.n)) {
                return {0, "Move out of board at step " + to_string(step)};
            }
            ++y;
            break;
        case 'W':
            board[x][y] ^= s;
            break;
        case 'C':
            s ^= board[x][y];
            break;
        case 'H':
            s /= 2;
            break;
        default:
            return {0, "Invalid operation"};
        }
        ++step;
    }
    uint64_t sum = 0;
    for (int i = 0; i < inp.n; ++i) {
        for (int j = 0; j < inp.n; ++j) {
            sum += board[i][j];
        }
    }
    return {static_cast<int64_t>(sum), ""};
}

string color_web(uint32_t val) {
    const uint32_t limit = 1u << 19;
    const uint32_t maxv = 1u << 20;
    double r, hue;
    if (val >= limit) {
        r = pow(log2(static_cast<double>(maxv - val)) / 19.0, 2);
        r = min(max(r, 0.0), 1.0);
        hue = r * 120.0;
    } else {
        r = pow(log2(static_cast<double>(val) + 1) / 19.0, 2);
        r = min(max(r, 0.0), 1.0);
        hue = 240.0 - r * 120.0;
    }
    ostringstream ss;
    ss << "hsl(" << round(hue) << ",100%,70%)";
    return ss.str();
}

string visualize(const vector<vector<uint32_t>>& b) {
    int n = static_cast<int>(b.size());
    int cell = 80;
    int w = n * cell;
    int h = n * cell;

    ostringstream svg;
    svg << "<svg viewBox='0 0 " << w << ' ' << h << "' width='" << w
        << "' height='" << h << "'>";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            svg << "<rect x='" << j * cell << "' y='" << i * cell
                << "' width='" << cell << "' height='" << cell
                << "' fill='" << color_web(b[i][j])
                << "' stroke='#ccc' stroke-width='1'/>";
            svg << "<text x='" << j * cell + cell / 2
                << "' y='" << i * cell + cell / 2
                << "' font-size='14' text-anchor='middle'"
                << " dominant-baseline='central'>" << b[i][j]
                << "</text>";
        }
    }
    svg << "</svg>";
    return svg.str();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: vis <input> <output>" << endl;
        return 1;
    }

    string in_file = argv[1];
    string out_file = argv[2];
    ifstream fin(in_file);
    if (!fin) {
        cerr << "no such file: " << in_file << endl;
        return 1;
    }
    ifstream fout(out_file);
    if (!fout) {
        cerr << "no such file: " << out_file << endl;
        return 1;
    }

    string sin((istreambuf_iterator<char>(fin)), {});
    string sout((istreambuf_iterator<char>(fout)), {});
    Input inp = parse_input(sin);
    string err;
    Output out = parse_output(inp, sout, err);
    vector<vector<uint32_t>> board;
    int64_t score = 0;
    if (err.empty()) {
        auto res = run(inp, out, board);
        score = res.first;
        err = res.second;
    }

    if (!err.empty()) {
        cout << err << "\nScore = 0" << endl;
    } else {
        cout << "Score = " << score << endl;
    }

    string html = "<html><body>" + visualize(board) + "</body></html>";
    ofstream visf("vis.html");
    visf << html;

    return 0;
}
