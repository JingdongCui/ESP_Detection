# 2026-07-18 队友交接包

## 交付物

- ZIP：`deliverables/ESP32P4_Detection_team_handoff_20260718_with_git.zip`
- 大小：34,145,927 bytes（约 33 MiB）
- SHA256：`ba4e1962a887763b72c80bf7eee49fa3b81ed8402e45fae90d8a7737c5e6c4c2`
- 外部校验文件：同路径追加 `.sha256`

## 包内内容

- `ESP32P4_Detection_standard/`
  - 默认分支：`feat/screen-uvc-stream`
  - HEAD：`a709e335837d3955e0b7e6d639f589f544b56ad6`
- `ESP32P4_Detection_b_busy_60/`
  - 默认分支：`feat/sorter-b-busy-speed-cap`
  - HEAD：`1092a3078f8a30627c39961742b26775dce10f4c`
- `README_交接说明.md`：配置、版本差异、验证状态和常用命令。
- `handoff_docs/`：队友包合并、real IO 栈和 vision_det 栈的详细记录。
- `MANIFEST_SHA256.txt`：388 个包内文件的 SHA256 清单。

两个工程目录均为独立可移动的 Git 仓库，各包含 6 个本地分支和完整提交对象；没有 remote，也不依赖打包机器的 worktree 绝对路径。打包来源为已提交状态，没有带入主工程原有的未提交删除、编辑器配置、CodeGraph、build 或 managed components。

## 校验结果

- `zip -T`：通过。
- 外层 ZIP SHA256：通过。
- 完整解压后 `MANIFEST_SHA256.txt`：全部通过。
- 两仓库 `git fsck --full --no-dangling`：通过。
- 两仓库工作区：干净。
- 两仓库 remote：为空。
- 默认分支及 HEAD：与上述记录一致。

## 验证边界

- 包内两版本均已在生成交付包前通过 `idf.py build`。
- 基础版已经完整烧录并完成约 121 秒识别/S1 压力监视。
- 动态限速版本阶段仅构建，尚未烧录。
- 正常 S1→S2→电机出口机械闭环仍需现场补测。
