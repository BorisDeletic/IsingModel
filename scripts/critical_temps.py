import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import math

pd.options.mode.chained_assignment = None  # default='warn'

def conv(s):
    try:
        s=float(s)
    except ValueError:
        pass
    return s


def getMeanMagsWithError(df):
    mean_mag = df.groupby(["n", "T"])["mag"].mean()
    std_mag = df.groupby(["n", "T"])["mag"].std()

    mean_suc = df.groupby(["n", "T"])["susceptibility"].mean()
    std_suc = df.groupby(["n", "T"])["susceptibility"].std()

    res = pd.DataFrame()
    res["mean_mag"] = mean_mag
    res["std_mag"] = std_mag

    res["mean_susceptibility"] = mean_suc
    res["std_susceptibility"] = std_suc

    res["reps"] = df.groupby(["n", "T"])["mag"].count()

    return res.reset_index()


def getCriticalTemperature(dfn):

    mags = np.array(dfn["mean_mag"])
    gradient = np.gradient(mags)

    return gradient


data = []
all_mag = {}
equilibriums = {}

f = open("../results/mean_magnetisations.csv")
while True:
    line1 = f.readline()
    line2 = f.readline()
    if not line2: break #EOF

    n, T, rand, t_eq, meanMag, spinFlucs = line1.split(",")

    mags_str = line2.split(",")[:-1]
    mags = list(map(conv, mags_str))

    data.append({
        "n": int(n),
        "T": float(T),
        "R": int(rand),
        "t_eq": int(t_eq),
        "mag": float(meanMag),
        "susceptibility" : float(spinFlucs)
    })

    all_mag[(int(n), float(T))] = mags
    equilibriums[(int(n), float(T))] = int(t_eq)


f.close()

df = pd.DataFrame(data, columns=data[0].keys())

ns = sorted(df['n'].unique())
Ts = sorted(df['T'].unique())

df_mean = getMeanMagsWithError(df)
print(df_mean)
getCriticalTemperature(df_mean)

fig, ax = plt.subplots()
fig1, ax1 = plt.subplots()
fig3, ax3 = plt.subplots()
#fig2, ax2 = plt.subplots()


for n in ns:
    dfn = df_mean.loc[df_mean["n"]==n]
    dfn["mean_mag"] = dfn["mean_mag"] / n**2

    reps = dfn["reps"].min() #use min value to display

    gradient = np.gradient(dfn["mean_mag"])
    gradient2 = np.gradient(gradient)

    ax.plot(dfn["T"], dfn["mean_mag"], label="lattice, n = {}, reps = {}".format(n, reps), linestyle='dashed', marker='s')
    ax1.plot(dfn["T"], gradient, label = "gradient, n = {}".format(n), linestyle='dashed', marker='s')
    ax3.plot(dfn["T"], gradient2, label = "gradient2, n = {}".format(n), linestyle='dashed', marker='s')


Tc = 2/math.log(1+math.sqrt(2))
print(Tc)
ax.axvline(x=Tc)


n = ns[0]
dfn = df.loc[df["n"]==n]

for n in ns:
    fig2, ax2 = plt.subplots()#

    dfn = df.loc[df["n"]==n]
    Ts = sorted(dfn['T'].unique())

    for T in Ts:
        ax2.plot(all_mag[(n, T)], label = "T = {}".format(T))

        t_eq = equilibriums[(n, T)]
        ax2.axvline(x=t_eq)

    ax2.set_title("Magnetisation vs Time, n = {}".format(n))


ax.set_title("Temperature vs Mean Magnetisation")
ax.legend(loc="lower left")

ax1.set_title("Temperature vs Gradient")
ax1.legend(loc="lower left")
ax3.set_title("Temperature vs Gradient2")
ax3.legend(loc="lower left")

ax2.set_title("Magnetisation vs Time, n = {}".format(n))
ax2.legend(loc="upper right")

plt.show()