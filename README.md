# ExpeditionOptimizer_KAI
Optimize schedule of expedition for Kantai Collection and Warship Girls by C++

# 概要
『[ExpeditionOptimizer](https://github.com/YSRKEN/ExpeditionOptimizer)』は実行にRubyとSCIPが必要でした。  
そこで、『[SMIPS](https://github.com/YSRKEN/SMIPS)』のソースを組み込んでスタンドアロン化しました。
(exeファイルはWindows 7以上を想定しています。コード部分はgccでもclangでも通るはず)

## 実行方法
1. リポジトリをcloneするか、zipをダウンロードするか、releaseに置いてあるzipファイルをダウンロードします。
2. run.batを実行します。最適化する対象の指定、資材量指定などを入力すれば計算が実行されます。

## 注意
- 現状、ソルバ部分はただの[SMIPS](https://github.com/YSRKEN/SMIPS)ですので遅いです
- tableで言えば13種以上の遠征を登録すると最適化時間が重く感じるはずです
- まあ[SMIPS](https://github.com/YSRKEN/SMIPS)は高速化は全く意図してないから仕方ない……
- ただしtable内のデータとしてはExpeditionOptimizerと同じにしてあるので自由にいじってください
- 出力される総遠征時間は目安の数値です
- キラ付けや大発には対応していません
- 表示してほしくない遠征があれば、table1.csvやtable2.csvを自前で編集してください。Shift_JIS形式・CR+LFを想定しています
