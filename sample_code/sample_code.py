import random

# 入力
N, T = map(int, input().split())
A = [list(map(int, input().split())) for _ in range(N)]

x, y = 0, 0  # 初期位置
s = 0

command_list = ["U", "D", "L", "R", "W", "C"]


# 移動可能かどうかを判定
def can_move(x: int, y: int, command: str) -> bool:
    if command == "U":
        return x > 0
    elif command == "D":
        return x < N - 1
    elif command == "L":
        return y > 0
    elif command == "R":
        return y < N - 1
    return True


# コマンドを実行
def process_command(board: list[list[int]], x: int, y: int, s: int, command: str):
    if command == "U":
        x, y = x - 1, y
    elif command == "D":
        x, y = x + 1, y
    elif command == "L":
        x, y = x, y - 1
    elif command == "R":
        x, y = x, y + 1
    elif command == "W":
        board[x][y] ^= s
    elif command == "C":
        s ^= board[x][y]
    return board, x, y, s


# スコアを計算
def calc_score(board: list[list[int]]) -> int:
    return sum(sum(board[i]) for i in range(N))


# ランダムにコマンドを選択
ans = []
while len(ans) < T:
    command = random.choice(command_list)
    if can_move(x, y, command):
        A, x, y, s = process_command(A, x, y, s, command)
        ans.append(command)

for i in range(T):
    print(ans[i])
