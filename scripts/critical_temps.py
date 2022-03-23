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
all_mag = {}
equilibriums = {}

f = open("../results/mean_magnetisations.csv")
while True:
    line1 = f.readline()
    line2 = f.readline()
    if not line2: break #EOF

    n, T, rand, t_eq, meanMag = line1.split(",")

    magss = line2.split(",")[:-1]
    mags = list(map(conv, magss))

    data.append({
        "n": int(n),
        "T": float(T),
        "R": int(rand),
        "t_eq": int(t_eq),
        "mag": float(meanMag),
    })

    all_mag[(int(n), float(T))] = mags
    equilibriums[(int(n), float(T))] = int(t_eq)


f.close()

df = pd.DataFrame(data, columns=data[0].keys())

ns = sorted(df['n'].unique())
Ts = sorted(df['T'].unique())

fig, ax = plt.subplots()
fig2, ax2 = plt.subplots()


for n in ns:
    dfn_rand = df.loc[(df["n"]==n) & (df["R"] == 1)].sort_values("T")
    dfn_uni = df.loc[(df["n"]==n) & (df["R"] == 0)].sort_values("T")

    print(dfn_rand)
    mags_r = abs(dfn_rand["mag"]) / n**2
    mags_u = abs(dfn_uni["mag"]) / n**2

    ax.plot(dfn_rand["T"], mags_r, label="random lattice, n = {}".format(n), linestyle='dashed', marker='s')
    ax.plot(dfn_uni["T"], mags_u, label="uniform lattice, n = {}".format(n), linestyle='dashed', marker='s')


n = ns[0]
dfn = df.loc[df["n"]==n]

for T in Ts:
    ax2.plot(all_mag[(n, T)], label = "T = {}".format(T))

    t_eq = equilibriums[(n, T)]
    ax2.axvline(x=t_eq)


ax.set_title("Temperature vs Mean Magnetisation")
ax.legend(loc="lower right")

ax2.set_title("Magnetisation vs Time, n = {}".format(n))
ax2.legend(loc="lower right")

plt.show()