# AviUtl プラグイン - 拡張編集透明化

* version 2.0.0 by 蛇色 - 2022/03/14 トラックバーなどを追加
* version 1.0.0 by 蛇色 - 2022/03/14 初版

拡張編集のウィンドウを透明化します。

## 導入方法

以下のファイルを AviUtl の Plugins フォルダに入れてください。
* Toumei.auf
* Toumei.ini

## 使用方法

チェックボックスにチェックを入れると拡張編集のウィンドウがマウス透過状態になり、下にあるウィンドウをクリックできるようになります。チェックを外すと解除されます。ただし、マウス透過状態のアルファ値が 255 の場合はマウス透過状態になりません。

## 設定方法

Toumei.ini をテキストエディタで編集してから AviUtl を起動します。

```ini
[Settings]
exeditTimelineWindowAlphaActive=224
; 拡張編集のタイムラインウィンドウがアクティブなときの透明度を指定します。(0～255)

exeditTimelineWindowAlphaInactive=192
; 拡張編集のタイムラインウィンドウが非アクティブなときの透明度を指定します。(0～255)

exeditTimelineWindowAlphaTransparent=64
; 拡張編集のタイムラインウィンドウがマウス透過状態のときの透明度を指定します。(0～255)

exeditObjectDialogAlphaActive=224
; 拡張編集のオブジェクトダイアログがアクティブなときの透明度を指定します。(0～255)

exeditObjectDialogAlphaInactive=192
; 拡張編集のオブジェクトダイアログが非アクティブなときの透明度を指定します。(0～255)

exeditObjectDialogAlphaTransparent=64
; 拡張編集のオブジェクトダイアログがマウス透過状態のときの透明度を指定します。(0～255)
```

## 動作確認

* (必須) AviUtl 1.10 & 拡張編集 0.92 http://spring-fragrance.mints.ne.jp/aviutl/
* (共存確認) patch.aul r14 https://www.nicovideo.jp/watch/sm40087155

## 参考サイト

* Auls https://auls.client.jp/
	* yulib、aulslib を使用しています。
	* 「拡張編集を半透明化」プラグインを参考にしています。
