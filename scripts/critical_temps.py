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


def openData():
    f = open("../results/zero_field.csv")

    # print file headers
    print(f.readline())
    print(f.readline())
    print(f.readline())
    print(f.readline())

    data = []
    all_mags = {}
    all_energys = {}
    all_correlations = {}

    while True:
        line1 = f.readline()
        line2 = f.readline()
        line3 = f.readline()
        line4 = f.readline()
        if not line4: break #EOF

        n, T, rand, t_eq, t_decor, meanMag, spinVar, heatCap, energyVar = list(map(conv, (line1.split(",")[:-1])))


        mags = list(map(conv, line2.split(",")[:-1]))
        energys = list(map(conv, line3.split(",")[:-1]))
        correlations = list(map(conv, line4.split(",")[:-1]))

        data.append({
            "n": int(n),
            "T": T,
            "R": int(rand),
            "t_eq": int(t_eq),
            "t_decor": int(t_decor),
            "mag": meanMag,
            "susceptibility": spinVar,
            "heatCap": heatCap,
            "energyVar": energyVar
        })

        all_mags[(int(n), T)] = mags
        all_energys[(int(n), T)] = energys
        all_correlations[(int(n), T)] = correlations

    f.close()

    results = pd.DataFrame(data, columns=data[0].keys())

    return results, all_mags, all_energys, all_correlations


def getMeansWithError(df):
    res = pd.DataFrame()

    grouped = df.groupby(["n", "T"])

    res["mean_mag"] = grouped["mag"].mean()
    res["std_mag"] = grouped["mag"].std()

    res["mean_susceptibility"] = grouped["susceptibility"].mean()
    res["std_susceptibility"] = grouped["susceptibility"].std()

    res["mean_t_decor"] = grouped["t_decor"].mean()
    res["std_t_decor"] = grouped["t_decor"].std()

    res["mean_heat_capacity"] = grouped["heatCap"].mean()
    res["std_heat_capacity"] = grouped["heatCap"].std()

    res["mean_energy_var"] = grouped["energyVar"].mean()
    res["std_energy_var"] = grouped["energyVar"].std()

    res["reps"] = grouped["mag"].count()

    return res.reset_index()


def getCriticalTemperature(dfn):
    Tc = dfn.loc[dfn["mean_susceptibility"].idxmax()]["T"]
    return Tc



def plotMagVsTemp(mean_mag):
    fig, ax = plt.subplots()

    for n in ns:
        dfn = mean_mag.loc[mean_mag["n"]==n]
        dfn["mean_mag"] = dfn["mean_mag"] / n**2

        reps = dfn["reps"].min() #use min value to display

        ax.plot(dfn["T"], dfn["mean_mag"], label="lattice, n = {}, reps = {}".format(n, reps), linestyle='dashed', marker='s')

    ax.set_title("Mean Magnetisation vs Temperature ")
    ax.legend(loc="lower left")


def plotSusceptibility(mean_mag):
    fig, ax = plt.subplots()

    for n in ns:
        dfn = mean_mag.loc[mean_mag["n"]==n]
        dfn["mean_susceptibility"] = dfn["mean_susceptibility"] / n**2

        reps = dfn["reps"].min() #use min value to display

        ax.plot(dfn["T"], dfn["mean_susceptibility"], label="lattice, n = {}, reps = {}".format(n, reps), linestyle='dashed', marker='s')

       # Tc = getCriticalTemperature(dfn)
     #   ax.axvline(x = Tc)

    ax.set_title("Susceptibility vs Temperature")
    ax.legend(loc="lower left")


def plotDecorrelationTimeVsTemperature(df):
    fig, ax = plt.subplots()

    for n in ns:
        dfn = df.loc[(df["n"]==n)].sort_values("T")

        ax.plot(dfn["T"], dfn["mean_t_decor"], label="lattice, n = {}".format(n), linestyle='dashed', marker='s')

    ax.set_title("Decorrelation Time vs Temperature")
    ax.legend(loc="lower left")


def plotHeatCapacityVsTemperature(df):
    fig, ax = plt.subplots()

    for n in ns:
        dfn = df.loc[(df["n"]==n)].sort_values("T")

        ax.plot(dfn["T"], dfn["mean_heat_capacity"], label="lattice, n = {}".format(n), linestyle='dashed', marker='s')

    ax.set_title("Heat Capacity vs Temperature")
    ax.legend(loc="upper left")


def plotEnergyVarianceVsTemperature(df):
    fig, ax = plt.subplots()

    for n in ns:
        dfn = df.loc[(df["n"]==n)].sort_values("T")

        ax.plot(dfn["T"], dfn["mean_energy_var"], label="lattice, n = {}".format(n), linestyle='dashed', marker='s')

    ax.set_title("Energy Variance vs Temperature")
    ax.legend(loc="upper left")



def plotMagVsTime(df, mags, n, Ts):
    fig, ax = plt.subplots()

    for T in Ts:
        c=next(ax._get_lines.prop_cycler)['color']

        ax.plot(mags[(n, T)], label = "T = {}".format(T), color = c)

        t_eq = df.loc[(df["n"] == n) & (df["T"] == T)]["t_eq"].iloc[-1]
        mean_mag = df.loc[(df["n"] == n) & (df["T"] == T)]["mag"].iloc[-1]

        ax.axvline(x=t_eq, color=c, linestyle = '--')
        ax.axhline(y=mean_mag, color=c, linestyle = '--')

    ax.set_title("Magnetisation vs Time, n = {}".format(n))
    ax.legend(loc="lower right")



def plotCorrelationVsTime(df, correlations, n, Ts):
    fig, ax = plt.subplots()

    for T in Ts:
        c=next(ax._get_lines.prop_cycler)['color']

        ax.plot(correlations[(n, T)], label = "T = {}".format(T), color=c)

        t_eq = df.loc[(df["n"] == n) & (df["T"] == T)]["t_decor"].iloc[-1]
        ax.axvline(x=t_eq, color=c, linestyle='--')

    ax.set_title("Correlation vs Time, n = {}".format(n))
    ax.legend(loc="lower right")


def plotEnergyVsTime(df, energys, n, Ts):
    fig, ax = plt.subplots()

    for T in Ts:
        c=next(ax._get_lines.prop_cycler)['color']

        ax.plot(energys[(n, T)], label = "T = {}".format(T), color=c)

        t_eq = df.loc[(df["n"] == n) & (df["T"] == T)]["t_decor"].iloc[-1]
        ax.axvline(x=t_eq, color=c, linestyle='--')

    ax.set_title("Energy vs Time, n = {}".format(n))
    ax.legend(loc="lower right")


df_raw, all_mags, all_energys, all_correlations = openData()



ns = sorted(df_raw['n'].unique())
Ts = sorted(df_raw['T'].unique())

df_mean = getMeansWithError(df_raw)



plotMagVsTemp(df_mean)
plotSusceptibility(df_mean)

plotDecorrelationTimeVsTemperature(df_mean)

plotHeatCapacityVsTemperature(df_mean)
plotEnergyVarianceVsTemperature(df_mean)


showTs = [Ts[i] for i in range(1, len(Ts), len(Ts) // 5)]
showTs.append(Ts[-1])

plotMagVsTime(df_raw, all_mags, ns[-1], showTs)
plotCorrelationVsTime(df_raw, all_correlations, ns[-1], showTs)
plotEnergyVsTime(df_raw, all_energys, ns[-1], showTs)



plt.show()