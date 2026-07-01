"""
A股回测：日K突破60日均线 + 周K首次突破60日均线
数据源：腾讯财经（前复权）+ 新浪财经股票列表
持有5个交易日后胜率统计
"""
import requests, json, re, time, random
import numpy as np
import pandas as pd
from concurrent.futures import ThreadPoolExecutor, as_completed

BACKTEST_START = "2025-05-10"
BACKTEST_END   = "2026-05-10"
DATA_START     = "2024-08-01"   # 需要足够历史计算MA60
MA_PERIOD      = 60
HOLD_DAYS      = 5
SAMPLE_SIZE    = 300
MAX_WORKERS    = 4

sess = requests.Session()
sess.trust_env = False
sess.headers.update({"Referer": "https://finance.qq.com"})


# ── 1. 获取股票列表 ────────────────────────────────────────────────────────────

def get_stock_list() -> list[str]:
    """从新浪财经获取全部A股代码（sh/sz前缀格式）"""
    stocks = []
    for node in ["hs_a"]:
        page = 1
        while True:
            url = (
                "https://vip.stock.finance.sina.com.cn/quotes_service/api/json_v2.php"
                f"/Market_Center.getHQNodeData?page={page}&num=100&sort=symbol"
                f"&asc=1&node={node}&symbol=&_s_r_a=page"
            )
            try:
                r = sess.get(url, timeout=10)
                data = json.loads(r.text)
                if not data:
                    break
                for item in data:
                    stocks.append(item["symbol"])  # e.g. sh600000 / sz000001
                if len(data) < 100:
                    break
                page += 1
                time.sleep(0.08)
            except Exception as e:
                print(f"  股票列表第{page}页获取失败: {e}")
                break
    return stocks


# ── 2. 获取历史数据 ────────────────────────────────────────────────────────────

def fetch_history(symbol: str) -> pd.DataFrame | None:
    """腾讯财经前复权日线数据，格式 [date,open,close,high,low,vol]"""
    url = (
        "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get"
        f"?_var=kline_dayqfq&param={symbol},day,{DATA_START},{BACKTEST_END},2000,qfq"
    )
    try:
        r = sess.get(url, timeout=15)
        raw = re.search(r"=(.+)", r.text).group(1)
        obj = json.loads(raw)
        stock_data = obj["data"][symbol]
        days = stock_data.get("qfqday") or stock_data.get("day")
        if not days or len(days) < MA_PERIOD + HOLD_DAYS + 10:
            return None
        df = pd.DataFrame(days, columns=["date", "open", "close", "high", "low", "vol"])
        df["date"]  = pd.to_datetime(df["date"])
        df["close"] = df["close"].astype(float)
        return df.sort_values("date").reset_index(drop=True)
    except Exception:
        return None


# ── 3. 信号检测 ────────────────────────────────────────────────────────────────

def find_signals(df: pd.DataFrame) -> list:
    df = df.copy()
    df["ma60"] = df["close"].rolling(MA_PERIOD).mean()

    # 日线金叉：收盘从下方穿越MA60
    df["daily_cross"] = (
        (df["close"] > df["ma60"]) &
        (df["close"].shift(1) <= df["ma60"].shift(1))
    )

    # 周线金叉：每周末收盘 vs 当周末日线MA60值
    weekly = (
        df.set_index("date")[["close", "ma60"]]
        .resample("W-FRI").last()
        .dropna()
    )
    weekly["w_cross"] = (
        (weekly["close"] > weekly["ma60"]) &
        (weekly["close"].shift(1) <= weekly["ma60"].shift(1))
    )
    cross_weeks = set(weekly[weekly["w_cross"]].index.to_period("W"))

    df["week_p"]      = df["date"].dt.to_period("W")
    df["weekly_flag"] = df["week_p"].isin(cross_weeks)
    df["signal"]      = df["daily_cross"] & df["weekly_flag"]

    mask = (df["date"] >= BACKTEST_START) & (df["date"] <= BACKTEST_END)
    return df.loc[mask & df["signal"], "date"].tolist()


# ── 4. 评估信号 ────────────────────────────────────────────────────────────────

def evaluate(df: pd.DataFrame, sig_date) -> tuple:
    df2 = df.set_index("date")
    if sig_date not in df2.index:
        return None, None
    idx = df2.index.get_loc(sig_date)
    if idx + HOLD_DAYS >= len(df2):
        return None, None
    entry = df2.iloc[idx]["close"]
    exit_ = df2.iloc[idx + HOLD_DAYS]["close"]
    ret   = (exit_ - entry) / entry * 100
    return ret > 0, round(ret, 2)


def process_stock(symbol: str) -> list:
    df = fetch_history(symbol)
    if df is None:
        return []
    results = []
    for sig in find_signals(df):
        ok, ret = evaluate(df, sig)
        if ok is not None:
            results.append((symbol, sig, ret, ok))
    return results


# ── 5. 主流程 ─────────────────────────────────────────────────────────────────

def main():
    print("正在获取A股列表...", flush=True)
    all_stocks = get_stock_list()
    print(f"共获取 {len(all_stocks)} 只股票，随机抽样 {SAMPLE_SIZE} 只进行回测", flush=True)

    random.seed(42)
    sample = random.sample(all_stocks, min(SAMPLE_SIZE, len(all_stocks)))

    all_results = []
    done = 0

    with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
        futures = {executor.submit(process_stock, s): s for s in sample}
        for future in as_completed(futures):
            res = future.result()
            all_results.extend(res)
            done += 1
            if done % 60 == 0 or done == len(sample):
                total = len(all_results)
                wins  = sum(1 for r in all_results if r[3])
                wr    = wins / total * 100 if total else 0
                print(f"  进度 {done}/{len(sample)} | 信号 {total} 个 | 胜率 {wr:.1f}%", flush=True)

    if not all_results:
        print("未找到任何信号，请检查数据或参数")
        return

    returns = [r[2] for r in all_results]
    wins    = sum(1 for r in all_results if r[3])
    total   = len(all_results)
    wr      = wins / total * 100
    pos     = [r for r in returns if r > 0]
    neg     = [r for r in returns if r <= 0]

    # 逐月统计
    monthly: dict[str, list] = {}
    for sym, date, ret, ok in all_results:
        ym = str(date)[:7]
        monthly.setdefault(ym, [0, 0])
        monthly[ym][0] += 1
        if ok:
            monthly[ym][1] += 1

    print("\n" + "=" * 60)
    print("  A股回测：日K + 周K 双重突破60日均线策略")
    print("=" * 60)
    print(f"  回测区间    : {BACKTEST_START} ~ {BACKTEST_END}")
    print(f"  抽样股票    : {len(sample)} 只 / 全市场 {len(all_stocks)} 只")
    print(f"  触发信号总数: {total}")
    print(f"  成功信号数  : {wins}")
    print(f"  ★ 胜率      : {wr:.2f}%")
    print(f"  平均收益    : {np.mean(returns):+.2f}%")
    print(f"  中位数收益  : {np.median(returns):+.2f}%")
    if pos: print(f"  盈利均幅    : +{np.mean(pos):.2f}%  (最大 +{max(pos):.2f}%)")
    if neg: print(f"  亏损均幅    : {np.mean(neg):.2f}%  (最大 {min(neg):.2f}%)")

    print("\n  逐月胜率分布:")
    for ym in sorted(monthly):
        n, w = monthly[ym]
        bar = "▓" * int(w / n * 20)
        print(f"    {ym}  {n:3d}个信号  胜率{w/n*100:5.1f}%  {bar}")

    print("\n  收益区间分布:")
    bins = [(-99, -5), (-5, -2), (-2, 0), (0, 2), (2, 5), (5, 10), (10, 99)]
    for lo, hi in bins:
        cnt = sum(1 for r in returns if lo <= r < hi)
        pct = cnt / total * 100
        bar = "█" * int(pct / 2)
        print(f"    [{lo:+4d}%,{hi:+4d}%)  {cnt:4d}次 ({pct:4.1f}%)  {bar}")
    print("=" * 60)


if __name__ == "__main__":
    main()
