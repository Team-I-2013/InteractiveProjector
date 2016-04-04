# InteractiveProjector

qキーにてアクティベーション終了後、画面左上にボタンを2つ表示するよう追加しました。
あわせて、以下の関数を追加しました。

int get_current_mode(void) ;//現在のモードを取得
int change_mode(void);//モードを切り替える

それぞれの戻り値は選択中のモードで、CONTROL_MODE または DRAWING_MODE で返されます。
