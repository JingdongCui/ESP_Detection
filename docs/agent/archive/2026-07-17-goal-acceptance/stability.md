# 长稳与推理验收

## 严格短周期结果

- 生产性能基线 `60c9f8a`：5×60，共 300 个 `wb_only` 样本。
- P50/P95/max：67.312/71.393/132.003 ms。
- `>=150 ms`：0；`>=500 ms`：0。
- 这组满足 waybill 单阶段的数值门槛，但 reset 是 CP2102N RTS，且没有 logo 级联、真实包裹与六类业务负载，不能作为 goal 完整推理验收。
- 最终生产 `c26dba8` 全量重刷后 1×60：P50/P95/max 67.848/71.494/72.046 ms，零 `>=150/500 ms`。

## 61 分钟结果

- `60c9f8a` 连续运行 3660.179 s。
- boot/fatal/guard rejection/连接检查失败：1/0/0/0。
- 1048 样本 P50/P95/max：67.308/74.273/217.760 ms。
- `>=150 ms`：8；`>=500 ms`：0。
- 24/24 task 快照完整，heap integrity=ok；所有应用任务最低剩余均 `>=512 B` 且 `>=20%`。

结论：连续性、P50/P95 和零 500 ms 回退通过；长稳全样本 max<=150 ms 失败。另一个未加 guard 的版本在 2412.116 s 发生 `dl_mc0` Instruction access fault，已作为失败证据保留。

## 原始证据

- `docs/agent/run_logs/2026-07-17-inference-ui-isolated-5x60.json`
- `docs/agent/run_logs/2026-07-17-final-production-v2-latency-1x60.json`
- `docs/agent/run_logs/2026-07-17-stability-ui-isolated-61min.json`
- `docs/agent/run_logs/2026-07-17-stability-ui-isolated-61min.log`
- `docs/agent/run_logs/2026-07-17-stability-61min.json`
- `docs/agent/run_logs/2026-07-17-stability-61min.log`
