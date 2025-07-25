#!/bin/bash

# マルチ言語ソルバーでテストケースを実行してスコア計算を行うスクリプト
# 使用例:
#   ./run_tests.sh my_solver.py         # Pythonファイル, 0000-0009
#   ./run_tests.sh my_solver.cpp 5      # C++ファイル, 0005のみ
#   ./run_tests.sh ./my_exe 3 5 7       # 実行ファイル, 0003,0005,0007
#   ./run_tests.sh sample_code.py 0-9   # 範囲指定

# 引数解析
solver_file=""
test_cases=()

if [ $# -eq 0 ]; then
    echo "Error: Solver file must be specified as the first argument."
    echo "Usage: $0 <solver_file> [test_cases...]"
    echo ""
    echo "Examples:"
    echo "  $0 my_solver.py           # Python file"
    echo "  $0 my_solver.cpp 5        # C++ file, case 0005 only"
    echo "  $0 ./my_exe 3 5 7         # Executable, cases 0003,0005,0007"
    echo "  $0 sample_code.py 0-9     # Range specification (0000-0009)"
    echo ""
    echo "Supported file types:"
    echo "  - Python: .py files"
    echo "  - C++: .cpp, .cxx, .cc, .C files (auto-compiled)"
    echo "  - Executables: files with execute permission"
    exit 1
fi

# 最初の引数は必ずソルバーファイル
solver_file="$1"
shift

# 残りの引数をテストケースとして処理
if [ $# -eq 0 ]; then
    # テストケースが指定されない場合はデフォルトで0-9
    for i in {0..9}; do
        test_cases+=($i)
    done
else
    # 引数をテストケースとして解析
    for arg in "$@"; do
        if [[ $arg =~ ^[0-9]+-[0-9]+$ ]]; then
            # 範囲指定 (例: 0-9, 10-19)
            start=$(echo $arg | cut -d'-' -f1)
            end=$(echo $arg | cut -d'-' -f2)
            for ((i=start; i<=end; i++)); do
                test_cases+=($i)
            done
        elif [[ $arg =~ ^[0-9]+$ ]]; then
            # 単一の数値
            test_cases+=($arg)
        else
            echo "Error: Invalid test case argument '$arg'. Use numbers or ranges (e.g., 0-9)"
            exit 1
        fi
    done
fi

# 重複を除去してソート
test_cases=($(printf '%s\n' "${test_cases[@]}" | sort -nu))

# ソルバーファイルの存在確認と実行コマンド生成
if [ ! -f "$solver_file" ] && [ ! -x "$solver_file" ]; then
    echo "Error: Solver file '$solver_file' not found or not executable"
    exit 1
fi

# ファイル拡張子とタイプによる実行コマンド決定
execute_command=""
temp_executable=""

get_extension() {
    echo "${1##*.}"
}

extension=$(get_extension "$solver_file")

case "$extension" in
    "py")
        execute_command="python3 $solver_file"
        echo "Using Python solver: $solver_file"
        ;;
    "cpp"|"cxx"|"cc"|"C")
        # C++ファイルの場合、自動コンパイル
        temp_executable="./temp_solver_$$"
        echo "Compiling C++ solver: $solver_file"
        if ! g++ -std=c++23 -O2 "$solver_file" -o "$temp_executable"; then
            echo "Error: Failed to compile C++ solver '$solver_file'"
            exit 1
        fi
        execute_command="$temp_executable"
        echo "Compiled successfully: $temp_executable"
        ;;
    *)
        # 拡張子がない場合や他の拡張子の場合、実行ファイルとして扱う
        if [ -x "$solver_file" ]; then
            execute_command="$solver_file"
            echo "Using executable: $solver_file"
        else
            echo "Error: '$solver_file' is not executable. For scripts, ensure they have appropriate shebang and execute permission."
            exit 1
        fi
        ;;
esac

echo "=== Running test cases: ${test_cases[*]} ==="
echo

# outディレクトリが存在しない場合は作成
if [ ! -d "out" ]; then
    echo "Creating out directory..."
    mkdir -p out
fi

# テストケース実行
for i in "${test_cases[@]}"; do
    # 4桁の0埋め形式にフォーマット
    case_num=$(printf "%04d" $i)
    echo "Processing case $case_num..."
    
    # ソルバーを実行（入力リダイレクト、出力リダイレクト）
    $execute_command < in/$case_num.txt > out/$case_num.txt
    
    if [ $? -eq 0 ]; then
        echo "✓ Case $case_num: Solution generated successfully"
    else
        echo "✗ Case $case_num: Failed to generate solution"
        continue
    fi
done

echo
echo "=== Calculating scores ==="
echo

# 各ケースのスコア計算
total_score=0
successful_cases=0

for i in "${test_cases[@]}"; do
    # 4桁の0埋め形式にフォーマット
    case_num=$(printf "%04d" $i)
    if [ -f "out/$case_num.txt" ]; then
        echo "Calculating score for case $case_num..."
        
        # visコマンドを実行してスコア計算
        score=$(./vis in/$case_num.txt out/$case_num.txt 2>/dev/null | grep -o '[0-9]*' | tail -1)
        
        if [ ! -z "$score" ]; then
            echo "✓ Case $case_num: Score = $score"
            total_score=$((total_score + score))
            successful_cases=$((successful_cases + 1))
        else
            echo "✗ Case $case_num: Failed to calculate score"
        fi
    else
        echo "✗ Case $case_num: Output file not found"
    fi
done

echo
echo "=== Summary ==="
echo "Solver: $solver_file"
echo "Successful cases: $successful_cases/${#test_cases[@]}"
echo "Total score: $total_score"

if [ $successful_cases -gt 0 ]; then
    average_score=$((total_score / successful_cases))
    echo "Average score: $average_score"
fi

# 一時的なC++実行ファイルをクリーンアップ
if [ ! -z "$temp_executable" ] && [ -f "$temp_executable" ]; then
    rm "$temp_executable"
    echo "Cleaned up temporary executable: $temp_executable"
fi

echo
echo "Done!" 