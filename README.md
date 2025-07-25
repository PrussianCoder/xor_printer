# XOR Printer

## プロジェクト構成

```
xor_printer/
├── index.html          # ビジュアライザー（Webアプリ）
├── tools/
│   ├── gen.cpp         # 問題インスタンス生成ツール
│   ├── vis.cpp         # 結果可視化ツール
│   ├── seeds.txt       # 問題生成用のシード値
│   └── in/             # 生成された問題インスタンス（100個）
└── README.md
└── sample_code.py # サンプルコード
```

## 使用方法

### 1. ビジュアライザーの使用
`index.html`をブラウザで開き、以下の操作が可能です：
- **Seed**: 問題インスタンスを変更
- **Input**: 問題データの入力
- **Output**: ソリューション（操作列）の入力
- **再生機能**: ソリューションの動作をステップごとに確認
- **画像保存**: 現在の盤面をPNG形式/GIF形式で保存

### 2. tools/

 詳細は [tools/README.md](tools/README.md) を参照してください。