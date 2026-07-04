# 2026-07-04 MyAlbums dataset organization

## Goal

- 整理 `/home/kazeform/2026esp/MyAlbums`，用于后续作为训练数据集。
- 将层次混乱的 `Cshot/时间戳` 目录扁平化为 `16_9`、`4_3`、`half` 三类。
- 将照片统一命名为 `jt_001.jpg` 这类格式。
- 整理完成后压缩，方便发给队友。

## Decisions

- 分类规则：
  - 顶层原目录名包含 `16_9` 的归入 `16_9`。
  - 顶层原目录名包含 `half` 的归入 `half`。
  - 未标注的归入 `4_3`。
  - `zt1higher` 没有 `16_9` 或 `half` 标注，因此按规则归入 `4_3`。
- 命名规则：
  - 每个分类目录内按品牌前缀独立编号。
  - 文件名格式为 `jt_001.jpg`、`yd_001.jpg`、`zt_001.jpg`。
  - 原始路径按稳定排序生成编号。
- 生成 `DATASET_MANIFEST.csv`，记录 `group,label,filename,original_path`，便于核对来源。

## Git

- 在 `MyAlbums` 内初始化独立 git，避免把根目录其他未跟踪工程混入。
- 原始基线提交：
  - `830536a checkpoint original album dataset`
- 整理后提交：
  - `8a51bc4 organize album dataset by aspect ratio`

## Result

- 整理后目录：
  - `MyAlbums/16_9`
  - `MyAlbums/4_3`
  - `MyAlbums/half`
  - `MyAlbums/DATASET_MANIFEST.csv`
- 图片数量：
  - `16_9`: 430 张，`jt=200`，`yd=100`，`zt=130`
  - `4_3`: 368 张，`jt=102`，`yd=90`，`zt=176`
  - `half`: 180 张，`jt=94`，`yd=86`
  - 总计：978 张 jpg
- `half` 当前没有 `zt` 图片，因为原始目录中没有 half 标注的 `zt` 文件夹。

## Package

- 压缩包：
  - `/home/kazeform/2026esp/MyAlbums_dataset_20260704.zip`
- 包内内容：
  - `MyAlbums/16_9`
  - `MyAlbums/4_3`
  - `MyAlbums/half`
  - `MyAlbums/DATASET_MANIFEST.csv`
- 不包含 `MyAlbums/.git`。
- 压缩包大小约 `2.9G`。

## Verification

- `zip -T MyAlbums_dataset_20260704.zip` 通过。
- 包内 jpg 数量：978。
- 包内包含 `MyAlbums/DATASET_MANIFEST.csv`。
- 包内分类计数：
  - `16_9=430`
  - `4_3=368`
  - `half=180`
- 本地目录命名校验：所有 jpg 均符合 `jt_###.jpg`、`yd_###.jpg` 或 `zt_###.jpg`，无异常命名。

## Build/Flash

- 本任务只整理照片数据集，不涉及 ESP-IDF 代码。
- 未运行 `idf.py build` 和 `idf.py flash monitor`；原因是不涉及固件代码或板端验证。
