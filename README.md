# Asteroid-GameEngine (Archive)

以 **C++17 + OpenGL** 從零開始實作的自製遊戲引擎學習專案。這是開發過程的封存版本（archive branch），主分支請見：[kwangsing3/Asteroid-GameEngine](https://github.com/kwangsing3/Asteroid-GameEngine)。

> ⚠️ 此為學習用途的 Spaghetti Code 研究版本，供參考用途。

## 實作的引擎模組

| 模組 | 檔案 | 說明 |
|------|------|------|
| 圖形引擎 | `GraphicEngine/` | OpenGL 渲染管線封裝 |
| 網格載入 | `AGE_Mesh`, `AGE_Model` | Assimp 3D 模型匯入 |
| 骨骼動畫 | `AGE_SkeletalMesh` | 骨骼網格與動畫 |
| 攝影機 | `Camera` | 視角控制 |
| 射線偵測 | `Raycast` | 場景射線碰撞 |
| 場景管理 | `SceneManager`, `World` | 場景物件管理 |
| Actor 系統 | `Actor`, `Component/` | 元件化物件架構 |
| 檔案系統 | `AGE_FileSystem`, `AGE_FileBrowser` | 資產載入 |
| Shader | `Shader/` | GLSL Shader 管理 |
| 紋理 | `Texture/` | 材質載入（stb_image）|

## 技術棧

- **C++17**
- **OpenGL** — 圖形渲染
- **CMake 3.10+** — 建置系統
- **glfw** — 視窗與 OpenGL 上下文
- **glm** — 數學函式庫（向量、矩陣）
- **bullet3** — 物理引擎
- **assimp** — 3D 模型匯入
- **imgui** — 即時除錯介面
- **pugixml** — XML 解析（場景設定）

## 建置方式

### 需求

- CMake 3.10+
- C++17 相容編譯器（MSVC / GCC / Clang）
- OpenGL 支援的顯示驅動

### 建置

```bash
# Windows（Visual Studio）
cmake -B build -G "Visual Studio 17 2022"
cmake --build build

# 或使用 CMakeSettings.json（Visual Studio 直接開啟）
```

## 授權

MIT — 學習用途，歡迎參考。