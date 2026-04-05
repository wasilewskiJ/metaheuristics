#!/usr/bin/env python3
"""
Plot best/avg/worst curves from algorithm CSV logs.

CSV format produced by Logger::dumpToFile():
  NR POKOLENIA:<n>;NAJLEPSZA OCENA:<v>;SREDNIA OCEN:<v>;NAJGORSZA OCENA:<v>;;;

Usage:
  python plot.py                        # auto-discovers *.csv in current dir
  python plot.py ea.csv randomAlg.csv   # explicit files
  python plot.py --out plot.png ea.csv  # save instead of showing
"""

import sys
import re
import argparse
import itertools
from pathlib import Path

# Optional: matplotlib is required
try:
    import matplotlib.pyplot as plt
except ImportError:
    sys.exit("matplotlib is required: pip install matplotlib")


# --------------------------------------------------------------------------- #
# Parsing                                                                      #
# --------------------------------------------------------------------------- #

_LABELED = re.compile(r"NR POKOLENIA:(\d+);NAJLEPSZA OCENA:(\d+);SREDNIA OCEN:(\d+);NAJGORSZA OCENA:(\d+)")
_PLAIN   = re.compile(r"^(\d+);(\d+);(\d+);(\d+)")


def parse_csv(path: Path) -> dict:
    """Return {'generation': [...], 'best': [...], 'avg': [...], 'worst': [...]}

    Supports two formats:
      labeled: NR POKOLENIA:1;NAJLEPSZA OCENA:1183;SREDNIA OCEN:1183;NAJGORSZA OCENA:1183;;;
      plain:   1;1183;1183;1183;;;
    """
    data = {"generation": [], "best": [], "avg": [], "worst": []}
    with open(path) as f:
        for line in f:
            m = _LABELED.search(line) or _PLAIN.match(line)
            if m:
                data["generation"].append(int(m.group(1)))
                data["best"].append(int(m.group(2)))
                data["avg"].append(int(m.group(3)))
                data["worst"].append(int(m.group(4)))
    if not data["generation"]:
        raise ValueError(f"No data found in {path}")
    return data


# --------------------------------------------------------------------------- #
# Plotting                                                                     #
# --------------------------------------------------------------------------- #

# Algorithms that produce a single solution (no population) — plot as a
# horizontal reference line instead of a curve (greedy is deterministic, so
# all "generations" have the same best value).
SINGLE_SOLUTION_ALGOS = {"greedy"}


def plot_algorithm(ax, name: str, data: dict, linestyle: str):
    """Draw best / avg / worst lines for one algorithm onto ax.

    Color encodes the series (blue=best, green=avg, red=worst).
    Linestyle encodes the algorithm (solid, dashed, etc.).
    """
    gen = data["generation"]

    if name.lower() in SINGLE_SOLUTION_ALGOS:
        best_val = data["best"][-1]
        ax.axhline(best_val, linestyle="--", color=SERIES_COLORS["best"],
                   label=f"{name} (best={best_val})")
        return

    ax.plot(gen, data["best"],  color=SERIES_COLORS["best"],  linewidth=1.5, linestyle=linestyle, label=f"{name} – najlepsza")
    ax.plot(gen, data["avg"],   color=SERIES_COLORS["avg"],   linewidth=1.2, linestyle=linestyle, label=f"{name} – średnia")
    ax.plot(gen, data["worst"], color=SERIES_COLORS["worst"], linewidth=1.0, linestyle=linestyle, label=f"{name} – najgorsza")


SERIES_COLORS = {
    "best":  "#1f77b4",  # blue
    "avg":   "#2ca02c",  # green
    "worst": "#d62728",  # red
}

# One linestyle per algorithm (cycles if more than 4 files)
ALGO_LINESTYLES = ["-", "--", "-.", ":"]


def build_plot(files: list[Path], out: Path | None):
    fig, ax = plt.subplots(figsize=(12, 6))

    for i, path in enumerate(files):
        try:
            data = parse_csv(path)
        except (ValueError, OSError) as e:
            print(f"[WARN] Skipping {path}: {e}", file=sys.stderr)
            continue

        ls = ALGO_LINESTYLES[i % len(ALGO_LINESTYLES)]
        plot_algorithm(ax, path.stem, data, ls)

    ax.set_xlabel("Nr pokolenia / iteracji")
    ax.set_ylabel("Wartość funkcji celu (czas)")
    ax.set_title("Porównanie algorytmów – przebieg optymalizacji")
    ax.legend(loc="upper right", fontsize=8)
    ax.set_xlim(left=1)
    ax.grid(True, linestyle="--", alpha=0.4)

    plt.tight_layout()
    if out:
        fig.savefig(out, dpi=150)
        print(f"Saved to {out}")
    else:
        plt.show()


# --------------------------------------------------------------------------- #
# Entry point                                                                  #
# --------------------------------------------------------------------------- #

KNOWN_ALGO_FILES = ["ea.csv", "randomAlg.csv", "greedy.csv", "ts.csv", "sa.csv"]


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("files", nargs="*", help="CSV files to plot (default: auto-discover)")
    parser.add_argument("--out", metavar="FILE", help="Save plot to file instead of showing")
    args = parser.parse_args()

    if args.files:
        paths = [Path(f) for f in args.files]
    else:
        # Auto-discover: prefer known names in order, then any remaining *.csv
        cwd = Path(".")
        ordered = [p for name in KNOWN_ALGO_FILES if (p := cwd / name).exists()]
        others  = sorted(p for p in cwd.glob("*.csv") if p not in ordered)
        paths   = ordered + others
        if not paths:
            sys.exit("No CSV files found. Run with explicit file arguments or place CSVs in the current directory.")

    build_plot(paths, Path(args.out) if args.out else None)


if __name__ == "__main__":
    main()
