#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using namespace std;

int N, T;
vector<vector<int>> A;
int x = 0, y = 0, s = 0;
vector<string> command_list = {"U", "D", "L", "R", "W", "C"};

// 移動可能かどうかを判定
bool can_move(int x, int y, const string& command) {
    if (command == "U") {
        return x > 0;
    } else if (command == "D") {
        return x < N - 1;
    } else if (command == "L") {
        return y > 0;
    } else if (command == "R") {
        return y < N - 1;
    }
    return true;
}

// コマンドを実行
void process_command(vector<vector<int>>& board, int& x, int& y, int& s, const string& command) {
    if (command == "U") {
        x = x - 1;
    } else if (command == "D") {
        x = x + 1;
    } else if (command == "L") {
        y = y - 1;
    } else if (command == "R") {
        y = y + 1;
    } else if (command == "W") {
        board[x][y] ^= s;
    } else if (command == "C") {
        s ^= board[x][y];
    }
}

// スコアを計算
int calc_score(const vector<vector<int>>& board) {
    int score = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            score += board[i][j];
        }
    }
    return score;
}

int main() {
    // 入力
    cin >> N >> T;
    A.resize(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> A[i][j];
        }
    }
    
    // 乱数生成器の初期化
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, command_list.size() - 1);
    
    // ランダムにコマンドを選択
    vector<string> ans;
    while (ans.size() < T) {
        string command = command_list[dis(gen)];
        if (can_move(x, y, command)) {
            process_command(A, x, y, s, command);
            ans.push_back(command);
        }
    }
    
    // 出力
    for (int i = 0; i < T; i++) {
        cout << ans[i] << endl;
    }
    
    return 0;
} 