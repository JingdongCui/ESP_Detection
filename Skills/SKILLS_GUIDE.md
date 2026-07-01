# Skills 功能简介

本项目 `skills/` 目录下包含四个 skill 包，各有不同用途。

---

## 目录结构

```
skills/
├── Document-skills/     # 文档处理技能集（docx / pdf / xlsx / pptx 等）
├── Find-skills/         # 技能发现与安装助手
├── pua-skills/          # 高绩效文化驱动的 Agent 激励系统
├── superpowers-skills/  # 开发工作流增强技能集
└── SKILLS_GUIDE.md      # 本文档
```

---

## 1. Document-skills — 文档处理技能集

**路径**：`skills/Document-skills/skills/`

包含以下核心 skill：

| Skill | 触发场景 | 核心能力 |
|-------|---------|---------|
| `docx` | 提到 Word 文档、.docx、报告、备忘录、信件 | 创建/读取/编辑 Word 文档，支持目录、页眉页脚、追踪修订 |
| `pdf` | 提到 .pdf 文件或任何 PDF 操作 | 提取文本/表格、合并/拆分、填写表单、OCR、加密 |
| `xlsx` | 提到电子表格、.xlsx/.csv/.tsv 文件 | 创建/编辑 Excel，支持公式、图表、财务模型、数据清洗 |
| `pptx` | 提到演示文稿、幻灯片、.pptx、deck | 创建/编辑 PPT，支持模板、布局设计、演讲者备注 |
| `algorithmic-art` | 生成算法艺术 | 用 JavaScript 生成可视化艺术作品 |
| `canvas-design` | Canvas 图形设计 | 基于 HTML Canvas 的图形设计 |
| `claude-api` | 使用 Claude API 开发 | Claude API / Anthropic SDK 的最佳实践 |
| `frontend-design` | 前端 UI 设计 | 前端界面设计规范与实现 |
| `internal-comms` | 企业内部沟通 | 公司通讯、新闻稿、FAQ 等企业文档 |
| `mcp-builder` | 构建 MCP 服务器 | MCP Server 的设计与实现 |
| `skill-creator` | 创建新 Skill | 帮助设计、测试、发布自定义 Skill |
| `webapp-testing` | Web 应用测试 | 使用 Playwright 进行 Web 自动化测试 |

---

## 2. Find-skills — 技能发现助手

**路径**：`skills/Find-skills/skills/find-skills/SKILL.md`

当你不知道有没有现成 skill 可用时，这个 skill 帮你搜索和安装。

**触发场景**：
- "有没有能做 X 的 skill？"
- "怎么实现 X？"
- "我想扩展 Claude 的能力"

**核心命令**：
```bash
npx skills find [关键词]        # 搜索 skill
npx skills add <owner/repo>     # 安装 skill
npx skills check                # 检查更新
npx skills update               # 更新所有 skill
```

**浏览市场**：https://skills.sh/

---

## 3. pua-skills — Agent 高绩效激励系统

**路径**：`skills/pua-skills/skills/pua/SKILL.md`

模拟大厂高绩效文化，让 Agent 更积极主动、更有 Owner 意识。

**触发场景**：
- 用户明确要求 PUA 模式（`/pua`）
- Agent 反复失败（2次以上）
- 用户表达不满："你再试试"、"别偷懒"、"换个方法"

**支持的"味道"（文化风格）**：

| 味道 | 风格 | 核心方法论 |
|------|------|---------|
| 🟠 阿里 | 底层逻辑·抓手·闭环 | 定目标→追过程→拿结果 |
| 🟡 字节 | ROI·坦诚清晰·务实敢为 | A/B Test·数据驱动 |
| 🔴 华为 | 力出一孔·自我批判 | RCA 根因分析·蓝军自攻击 |
| ⬛ Musk | Ship or die·极限压力 | 质疑→删除→简化→加速→自动化 |
| ⬜ Jobs | 像素级完美·减法优先 | DRI 单人负责·原型驱动 |
| 🔶 Amazon | Customer Obsession | Working Backwards·6-Pager |
| 🟤 Netflix | Keeper Test·职业运动队 | 人才密度>规则密度 |
| 🪟 Microsoft | Connects·Impact Descriptor | PIP/GVSA Gate |

**三条红线**：
1. 闭环意识 — 没有输出证据的完成叫自嗨
2. 事实驱动 — 未验证的归因不是诊断，是甩锅
3. 穷尽一切 — 未走完 5 步禁止放弃

**搭配 skill**：
- `pua:pro` — 自进化基线 + 指令系统
- `pua:p7` / `pua:p9` / `pua:p10` — 不同级别执行模式

---

## 4. superpowers-skills — 开发工作流增强

**路径**：`skills/superpowers-skills/skills/`

为 Claude Code 提供结构化开发工作流，让 Agent 在编码前先思考、先设计。

**核心 skill 列表**：

| Skill | 触发场景 | 作用 |
|-------|---------|------|
| `brainstorming` | 任何创意/功能开发前 | **强制**在写代码前探索需求、提出设计方案并获得用户确认 |
| `writing-plans` | 设计确认后 | 将设计转化为详细实施计划 |
| `executing-plans` | 有实施计划时 | 按计划逐步执行，跟踪进度 |
| `systematic-debugging` | 遇到 bug 时 | 系统化根因分析，防止原地打转 |
| `test-driven-development` | 编写测试时 | TDD 最佳实践，避免测试反模式 |
| `requesting-code-review` | 提交代码前 | 规范化代码审查流程 |
| `receiving-code-review` | 收到审查意见时 | 如何处理代码审查反馈 |
| `subagent-driven-development` | 复杂任务 | 用子 Agent 并行开发，提升效率 |
| `dispatching-parallel-agents` | 需要并行工作时 | 协调多个子 Agent 同时工作 |
| `finishing-a-development-branch` | 分支完成时 | 分支收尾检查清单 |
| `verification-before-completion` | 任务完成前 | 防止虚假完成，强制验证 |
| `using-git-worktrees` | 使用 Git worktree | Git worktree 工作流 |
| `writing-skills` | 创建新 Skill 时 | Skill 编写最佳实践 |

**brainstorming 的硬性规则**：
> 任何创意工作（新功能、新组件、修改行为）**必须**先走 brainstorming，未经设计确认不得写代码。

---

## 如何让 Claude 使用这些 Skills

### 方式一：Claude Code 插件安装（推荐）

在 Claude Code 中运行：
```
/plugin install document-skills@anthropic-agent-skills
```

或注册本地插件目录（如果 `.claude-plugin/` 目录存在）：
```
/plugin add ./skills/Document-skills
/plugin add ./skills/superpowers-skills
```

### 方式二：直接在对话中引用

安装后，直接在对话中提及 skill 名称即可触发：

```
# 触发 docx skill
"帮我创建一个 Word 格式的项目报告"

# 触发 pdf skill
"从这个 PDF 里提取所有表格数据"

# 触发 brainstorming skill
"我想给应用加一个用户登录功能"

# 触发 pua skill
"/pua 帮我解决这个 bug"

# 触发 systematic-debugging
"这个 bug 我试了好几次都没解决"
```

### 方式三：通过 CLAUDE.md 自动加载

在项目根目录的 `CLAUDE.md` 中添加：
```markdown
## Skills
本项目使用以下 skills，遇到对应场景时请自动触发：
- 文档操作：使用 Document-skills（docx/pdf/xlsx/pptx）
- 新功能开发前：使用 superpowers:brainstorming
- 调试困难时：使用 superpowers:systematic-debugging
```

### 方式四：npx skills CLI

```bash
# 搜索可用 skill
npx skills find document

# 全局安装
npx skills add anthropics/skills@document-skills -g -y

# 查看已安装
npx skills list
```

---

## 快速触发参考

| 你想做的事 | 说什么 / 触发词 |
|-----------|--------------|
| 创建 Word 文档 | "生成一个 .docx 报告" |
| 处理 PDF | "从 PDF 提取..." / "合并这些 PDF" |
| 操作 Excel | "创建一个 .xlsx 表格" / "分析这个 CSV" |
| 制作 PPT | "做一个演示文稿" / "创建 deck" |
| 开发新功能前 | "我想添加..." → 自动触发 brainstorming |
| 调试卡壳 | "这个 bug 一直解决不了" → systematic-debugging |
| 提高 Agent 积极性 | `/pua` 或 "你再努力一点" |
| 搜索新 skill | "有没有能做 X 的 skill？" → find-skills |
