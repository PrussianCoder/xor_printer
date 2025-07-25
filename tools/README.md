# Tools

このディレクトリには C++ を使用した入力生成器とスコア計算・ビジュアライズ用のプログラム・サンプルコードを置いています。

## サンプルコード
サンプルコードとして [sample_code.py](sample_code.py) （Python）と [sample_code.cpp](sample_code.cpp) （C++）を用意しています。
このコードを提出するとACが取れますのでご活用ください。 

## 入力生成
はじめにコンパイルを実行してください
```
# 入力生成器
g++ -std=c++23 -O2 gen.cpp -o gen
```
`seeds.txt` に乱数 seed (符号なし 64bit 整数) を列挙し、次のように実行します。
```
./gen seeds.txt               # `in` ディレクトリに入力を出力
./gen seeds.txt -d in2 -v     # 出力先を指定、生成したファイル名と seed を表示
```
生成された入力ファイルは指定したディレクトリに `0000.txt` という形式で保存されます。

## スコア計算とビジュアライゼーション

はじめにコンパイルを実行してください
```
# スコア計算/ビジュアライズ
g++ -std=c++23 -O2 vis.cpp -o vis
```

`in.txt` を入力ファイル、`out.txt` を解答ファイルとして以下を実行します。

```
./vis in.txt out.txt
```
標準出力にスコアが表示され、`vis.html` に盤面の SVG が保存されます。ブラウザで開くことで盤面を確認できます。

## マルチテストケース実行

### 事前準備
- 実行権限: `chmod +x run_tests.sh`を実行

### 複数テストケースの一括実行
`run_tests.sh` を使用してPython/C++両方のソルバーで複数テストケースを効率的に実行できます。引数に実行するコードを指定してください（PythonとC++が対応しています）

```bash
# ソルバーファイル必須（テストケース未指定の場合は0000-0009）
./run_tests.sh sample_code.py

# Pythonファイル + 特定ケース
./run_tests.sh my_solver.py 5
./run_tests.sh sample_code.py 0-9
```

#### 対応形式
- **Python**: `.py` ファイル → `python3` で実行
- **C++**: `.cpp`, `.cxx`, `.cc`, `.C` → 自動コンパイル後実行
- **実行ファイル**: 拡張子なし/実行権限あり → 直接実行

### 出力例
```
Using Python solver: sample_code.py
=== Running test cases: 0 1 2 3 4 5 6 7 8 9 ===
Processing case 0000...
✓ Case 0000: Solution generated successfully
...
=== Summary ===
Solver: sample_code.py
Successful cases: 10/10
Total score: 513719435
Average score: 51371943
```




