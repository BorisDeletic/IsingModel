import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def conv(s):
    try:
        s=float(s)
    except ValueError:
        pass
    return s



data = []
correlations = {}

f = open("../results/decorrelation_times.csv")
while True:
    line1 = f.readline()
    line2 = f.readline()
    if not line2: break #EOF

    n, T, rand, decor_t = line1.split(",")
    n, T = int(n), float(T)

    cors = list(map(conv, line2.split(",")[:-1]))

    data.append({
        "n": n,
        "T": T,
        "R": int(rand),
        "decor_t": int(decor_t)
    })

    correlations[(n, T)] = cors

f.close()

df = pd.DataFrame(data, columns=data[0].keys())

ns = sorted(df['n'].unique())
Ts = sorted(df['T'].unique())

fig, ax = plt.subplots()


for n in ns:
    dfn = df.loc[(df["n"]==n)].sort_values("T")

    ax.plot(dfn["T"], dfn["decor_t"], label="uniform lattice, n = {}".format(n), linestyle='dashed', marker='s')



for n in ns:
    fig2, ax2 = plt.subplots()

    dfn = df.loc[df["n"]==n]
    for T in Ts:
        ax2.plot(correlations[(n, T)], label = "T = {}".format(T))

    ax2.set_title("Correlations vs Time, n = {}".format(n))
    ax2.legend(loc="lower right")


ax.set_title("Temperature vs Decorrelation Time")
ax.legend(loc="lower right")


plt.show()