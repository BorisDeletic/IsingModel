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
energy = {}

f = open("../results/heat_capacity.csv")
while True:
    line1 = f.readline()
    line2 = f.readline()
    if not line2: break #EOF

    n, T, rand, energyStd, heatCap = line1.split(",")
    n, T = int(n), float(T)

    energys = list(map(conv, line2.split(",")[:-1]))

    data.append({
        "n": n,
        "T": T,
        "R": int(rand),
        "energyStd": float(energyStd),
        "heatCap": float(heatCap)
    })

    energy[(n, T)] = energys

f.close()

df = pd.DataFrame(data, columns=data[0].keys())

ns = sorted(df['n'].unique())
Ts = sorted(df['T'].unique())

fig, ax = plt.subplots()
fig, ax1 = plt.subplots()

for n in ns:
    dfn = df.loc[(df["n"]==n)].sort_values("T")

    ax.plot(dfn["T"], dfn["heatCap"], label="uniform lattice, n = {}".format(n), linestyle='dashed', marker='s')
    ax1.plot(dfn["T"], dfn["energyStd"], label="lattice, n = {}".format(n), linestyle='dashed', marker='s')





for n in ns:
    fig2, ax2 = plt.subplots()

    dfn = df.loc[df["n"]==n]
    for T in Ts:
        es = energy[(n, T)]
        ax2.plot(es, label = "T = {}".format(T))

    ax2.set_title("Energy vs Time, n = {}".format(n))
    ax2.legend(loc="upper right")


ax.set_title("Temperature vs Heat Capacity")
ax.legend(loc="upper right")

ax1.set_title("Temperature vs Energy Fluctuations")
ax1.legend(loc="upper right")


plt.show()