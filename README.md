# AviUtl プラグイン - 拡張編集透明化

* version 1.0.0 by 蛇色 - 2022/03/14 初版

拡張編集のウィンドウを透明化します。

## 導入方法

以下のファイルを AviUtl の Plugins フォルダに入れてください。
* Toumei.auf
* Toumei.ini

## 使用方法

指定されたキー (デフォルトでは Alt+Q) を押すと拡張編集のウィンドウがマウス透過状態になり、下にあるウィンドウをクリックできるようになります。もう一度キーを押すと解除されます。

## 設定方法

Toumei.ini をテキストエディタで編集してから AviUtl を起動します。

```ini
[Settings]
exeditTimelineWindowAlphaActive=192
; 拡張編集のタイムラインウィンドウがアクティブなときの透明度を指定します。(0～255)

exeditTimelineWindowAlphaInactive=128
; 拡張編集のタイムラインウィンドウが非アクティブなときの透明度を指定します。(0～255)

exeditTimelineWindowAlphaTransparent=64
; 拡張編集のタイムラインウィンドウがマウス透過状態のときの透明度を指定します。(0～255)

exeditObjectDialogAlphaActive=192
; 拡張編集のオブジェクトダイアログがアクティブなときの透明度を指定します。(0～255)

exeditObjectDialogAlphaInactive=128
; 拡張編集のオブジェクトダイアログが非アクティブなときの透明度を指定します。(0～255)

exeditObjectDialogAlphaTransparent=64
; 拡張編集のオブジェクトダイアログがマウス透過状態のときの透明度を指定します。(0～255)

vkCode=0x51
; ウィンドウをマウス透過状態にする仮想キーコードを指定します。(0x51 == 'Q')
; https://docs.microsoft.com/ja-jp/windows/win32/inputdev/virtual-key-codes

vkModifier=0x12
; ウィンドウをマウス透過状態にする修飾キーコードを指定します。(0x12 == Alt)
; 0 を指定した場合は修飾キーなしになります。
```

## 動作確認

* (必須) AviUtl 1.10 & 拡張編集 0.92 http://spring-fragrance.mints.ne.jp/aviutl/
* (共存確認) patch.aul r14 https://www.nicovideo.jp/watch/sm40087155

## 参考サイト

* Auls https://auls.client.jp/
	* yulib、aulslib を使用しています。
	* 「拡張編集を半透明化」プラグインを参考にしています。
