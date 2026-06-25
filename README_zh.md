# TriKit Engine

TriKit是一个专注于3D渲染的自研游戏引擎，基于DirectX 12开发，提供高效的图形渲染、输入处理、资源管理和场景管理功能。

## 核心功能

- **DirectX 12渲染**：利用最新的DirectX 12特性，提供高效的3D渲染能力

## 构建方法

1. 确保安装了CMake 3.16或更高版本
2. 确保安装了Windows SDK 10.0.19041.0或更高版本
3. 确保安装了DirectX 12兼容的显卡和驱动

### 构建步骤

```bash
# 创建构建目录
mkdir build
cd build

# 配置CMake
cmake ..

# 构建项目
cmake --build . --config Release
```

## 运行示例

构建完成后，在`build/bin`目录下找到`BasicExample.exe`，运行即可看到一个蓝色背景的窗口，展示了TriKit引擎的基本功能。

## 未来规划

- 完善渲染系统，支持更多3D特性
- 添加物理引擎集成
- 实现高级光照和阴影效果
- 增加材质系统
- 支持更多平台

## 许可证

MIT License

---

[Read this document in English](README.md)