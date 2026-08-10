# GitHub 工程发布与完整历史修正

## 目标

- 将板端工程发布到 `git@github.com:JingdongCui/ESP_Detection.git`。
- 上位机工程作为 `esp32_host_no_inference/` 子目录一并发布。
- 使用 `/home/kazeform/.ssh/github163` 完成 SSH 鉴权。
- GitHub 必须能够看到板端本地提交历史，而不是只有一个源码快照提交。

## 来源

- 板端：`ESP32P4_Detection`，提交 `a709e335837d3955e0b7e6d639f589f544b56ad6`，32 个提交。
- 上位机：`esp32_host_no_inference`，提交 `72be055757677c919aaf26d6d20e0589c5219669`，12 个提交。

## 实施与结果

- 远端原为空仓库，首次源码快照提交为 `4ab8ace`；发现该形式不保留本地历史后立即修正。
- 以板端完整历史为 `main` 主线，将上位机完整历史通过无关历史合并接入，并把上位机工作树放到 `esp32_host_no_inference/`。
- 最终远端 `main` 为合并提交 `76110c4997abf155b792077745b616e0ba8bc576`，两个父提交分别为板端和上位机来源提交。
- 使用 `--force-with-lease` 将仅包含本次快照的远端分支安全替换为完整历史版本。

## 验证

- 使用 `github163` 从 GitHub 全新克隆成功。
- 远端 `HEAD` 与 `refs/heads/main` 均为 `76110c4997abf155b792077745b616e0ba8bc576`。
- 全新克隆统计为 45 个可达提交：板端 32、上位机 12、合并提交 1。
- `git cat-file` 确认远端对象库包含板端 `a709e33` 和上位机 `72be055`。
- 顶层目录确认板端位于仓库根目录，上位机位于 `esp32_host_no_inference/`。
- 本次仅重组并发布已有提交，没有修改板端代码；因此不重复执行 `idf.py build`、`idf.py flash monitor`。上位机代码同样未改动。

## GitHub 显示说明

- 仓库提交列表可显示完整历史。
- 提交是否计入某个 GitHub 用户个人主页的 Contributions，取决于提交邮箱是否已绑定并验证到该 GitHub 账号；当前板端主要邮箱为 `1427203015@qq.com`，另有 1 个提交使用 `dev@localhost`。

## 前序阶段摘要

- 前序 Qt 上位机展示模式和本地云端回复动画已完成，构建、CTest 与离屏启动验证通过；详细长期信息已保留在 `PROJECT.md`。
