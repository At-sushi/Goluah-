// Goluah!! 分割DLL用インターフェースとかそのほか

// 要するに純粋仮想関数な訳だが
#define IF(a) virtual a = 0

class ISystem {
  //サービス
  IF(double DrawBMPText(double x, double y, float z, char *str, DWORD col)); //!< ビットマップテキストを描画
  IF(double DrawBMPTextEx(double x,
                          double y,
                          float z,
                          char *str, //!< ビットマップテキストを描画(拡縮・プロポーショナルオプションつき)
                          DWORD col,
                          float scaX,
                          float scaY,
                          DWORD flags));

  // fps
  IF(DWORD GetFPS()); //!< 実際フレームレート取得

  //タスク
  IF(void AddTask(CTaskBase *task));            //!< 新規タスク追加
  IF(CExclusiveTaskBase *GetCurrentMainTask()); //!< 現在の排他タスクを取得

  //システムグラフィックを取得
  IF(MYSURFACE **GetSystemGraphicSurface()); //!< システムとして保持されているGCD用のビットマップ配列を取得
  IF(GCD_RECT *GetSystemGraphicRect()); //!< システムとして保持されているGCDの矩形配列を取得
  IF(GCD_CELL2 *GetSystemGraphicCell()); //!< システムとして保持されているGCDのセル配列を取得
  IF(GCD_HANTEI *GetSystemGraphicHantei()); //!< システムとして保持されているGCDのあたり判定矩形配列を取得
};

class IDraw {

  //■その他描画
  //ライン描画
  IF(void DrawLine(int sx, int sy, int gx, int gy, DWORD col));
  //バックバッファのクリア
  IF(void ClearBackBuffer());
  //円の描画
  IF(void DrawCircle(int x,
                     int y,
                     int rad,
                     int w,
                     float z = 0,
                     DWORD color = 0xFF55AAFF,
                     BOOL toumei = FALSE,
                     float rot = 0,
                     BOOL rot_y = TRUE,
                     BOOL hosei = TRUE));
  //なにこれ？
  IF(void Draw3DText());
  //テキスト描画
  IF(void DrawBlueText(RECT &r, char *text, int len, DWORD method, DWORD size));
  IF(void DrawRedText(RECT &r, char *text, int len, DWORD method, DWORD size));

  //■便利関数？
  IF(void SetAlphaMode(DWORD alphamode));                //αブレンドのモードを設定
  IF(void EnableZ(BOOL test = TRUE, BOOL write = TRUE)); // Zテスト・ライトの有効・無効を設定

  //■座標変換マトリクス関連
  // for directx8
  IF(void ResetTransformMatrix()); // view & projection
  IF(void SetTransform(BOOL b));
  IF(void SetWorldMatrix(D3DXMATRIX *pmat));
};

__interface IBattleTask{};
