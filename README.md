# XOR Printer

## プロジェクト構成

```
xor_printer/
│── sample_code/        # サンプルコード
├── tools/              # ローカルテスター
│   ├── gen.cpp         # 問題インスタンス生成ツール
│   ├── vis.cpp         # 結果可視化ツール
│   ├── seeds.txt       # 問題生成用のシード値
│   ├── run_tests.sh    # マルチテストケース実行スクリプト
│   └── in/             # 生成された問題インスタンス（100個）
├── index.html          # ビジュアライザー（Webアプリ）
└── README.md
```

## 詳細説明

### sample_code（サンプルコード）
サンプルコードとして [sample_code.py](sample_code/sample_code.py) （Python）と [sample_code.cpp](sample_code/sample_code.cpp) （C++）を用意しています。
このコードを提出するとACが取れますのでご活用ください。 

### tools/　（ローカルテスター）
この問題の入力データ（seed0-99）、および入力生成器とスコア計算・ビジュアライズ用のプログラム・マルチテストケース実行スクリプトが格納されています。

使い方の詳細は [tools/README.md](tools/README.md) を参照してください。


### index.html（ビジュアライザ）
生成された入力に対して解答プログラムをローカル実行し、プログラムの出力を Output 欄に貼り付けると、ビジュアライズ結果が表示されます。

こちらからも閲覧できます：[リンク](https://prussiancoder.github.io/xor_printer/)

使い方はビジュアライザ内の「使い方」を参照してください。