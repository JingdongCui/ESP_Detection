# AGENTS.md

## 默认上下文
开始任何任务前，先读取：
- `docs/agent/PROJECT.md`：长期工程信息、架构、硬件、约定、坑点
- `docs/agent/CURRENT.md`：当前现状、当前任务、阻塞点
- `docs/agent/HISTORY.md`：当前任务过程记录

若上述文件或目录不存在，先创建：
- `docs/agent/PROJECT.md`
- `docs/agent/CURRENT.md`
- `docs/agent/HISTORY.md`
- `docs/agent/archive/`

## Git 规则
修改前提交git,若根目录有多个工程，进入工程目录进行git提交。如果工程目录无git,执行init并提交。


## 工作模式

优先使用 CodeGraph 定位符号、调用关系和影响范围。

复杂任务使用计划模式，任何不确定的事情提前问清楚。

复杂任务优先考虑 subagent


## 文档维护
每次任务都维护：
- `docs/agent/CURRENT.md`：当前目标、现状、下一步、阻塞点。超出200行自动压缩，多余信息写入 `docs/agent/HISTORY.md`
- `docs/agent/HISTORY.md`：本任务的重要决策、命令、结果、失败原因
- `docs/agent/PROJECT.md`：长期有效的工程信息、架构、硬件配置、常用命令、约定、已知坑

一个阶段完成后：
1. 将 `CURRENT.md` 和 `HISTORY.md` 整理为 `docs/agent/archive/YYYY-MM-DD-简短任务名.md`
2. 清空并重置 `CURRENT.md` 和 `HISTORY.md`
3. 保留并继续维护 `PROJECT.md`


## 验证要求
修改代码后必须运行相关检查。
1. `idf.py build`
2. `idf.py flash monitor`



## 完成标准
任务完成必须满足：
- 代码改动完成
- 相关文档已更新
- 相关测试/构建/实机验证已执行，或明确记录无法执行原因
