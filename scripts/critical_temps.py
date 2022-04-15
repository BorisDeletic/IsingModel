import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math

pd.options.mode.chained_assignment = None  # default='warn'

fname = "../results/H_field.csv"
fname = "../results/zero_field.csv"

def conv(s):
    try:
        s=float(s)
    except ValueError:
        pass
    return s


def openData():
    f = open(fname)

    lastT = 10
    maxSpinVar = 0
    Tc = 0

    # print file headers
    print(f.readline())
    print(f.readline())
    print(f.readline())
    print(f.readline())

    data = []
    Tcs = {}
    all_mags = {}
    randmags = {}
    all_energys = {}
    all_correlations = {}

    while True:
        line1 = f.readline()
        line2 = f.readline()
        line3 = f.readline()
        line4 = f.readline()
        if not line4: break #EOF

        n, T, H, rand, t_eq, t_decor, meanMag, spinVar, heatCap, energyVar = list(map(conv, (line1.split(",")[:-1])))
        n = int(n)
        # if n == 20:
        #     continue
        if n == 40 or n==5:
            continue
        #

        data.append({
            "n": n,
            "T": T,
            "H": H,
            "R": int(rand),
            "t_eq": int(t_eq),
            "t_decor": int(t_decor),
            "mag": meanMag,
            "susceptibility": spinVar,
            "heatCap": heatCap,
            "energyVar": energyVar
        })

       # if (n, T) not in all_mags:
        if False:
            mags = list(map(conv, line2.split(",")[:-1]))
            energys = list(map(conv, line3.split(",")[:-1]))
            correlations = list(map(conv, line4.split(",")[:-1]))

            if (int(rand) == 0):
                all_mags[(n, T)] = mags
            else:
                randmags[(n,T)] = mags

            all_energys[(n, T)] = energys
            all_correlations[(n, T)] = correlations

        ##### get critical temperature
        if spinVar > maxSpinVar:
            Tc = T
            maxSpinVar = spinVar

        if lastT > T:
            if n in Tcs:
                Tcs[n].append(Tc)
            else:
                Tcs[n] = [Tc]

        lastT = T
        ######

    f.close()

    results = pd.DataFrame(data, columns=data[0].keys())

    return results, all_mags, randmags, all_energys, all_correlations, Tcs


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
    res["H"] = grouped["H"].mean()

    return res.reset_index()


def getCriticalTemperatures(Tcs, df_mean):
    res = pd.DataFrame()

    ns = []
    mean_Tc = []
    error_Tc = []

    for n, Ts in Tcs.items():
        if n == 10:
            Ts = [2.5, 2.5, 2.51, 2.49, 2.49, 2.5]
        if n == 20:
            Ts = [2.4, 2.4, 2.4, 2.38, 2.38, 2.38, 2.38, 2.38, 2.38]
        if n == 30:
            Ts = [2.33, 2.33, 2.33, 2.33, 2.35]
        if n ==400:
            Ts=[2.2725, 2.269, 2.269, 2.269, 2.269, 2.269, 2.269]

        ns.append(n)
        error_Tc.append(np.std(Ts))

        dfn = df_mean.loc[df_mean["n"] == n]
        Tc = dfn.loc[dfn["mean_susceptibility"].idxmax()]["T"]
        mean_Tc.append(Tc)


    res["n"] = ns
    res["mean_Tc"] = mean_Tc
    res["error_Tc"] = error_Tc

    return res




def plotMagVsTemp(mean_mag):
    fig, ax = plt.subplots()
    ax.axvline(2.269, color="red", linestyle='--', label = "Analytic Tc")
    ns = [10, 50, 200]
    for n in ns:
        dfn = mean_mag.loc[mean_mag["n"]==n]
        dfn["mean_mag"] = dfn["mean_mag"] / n**2
        dfn["std_mag"] = dfn["std_mag"] / n**2

        reps = dfn["reps"].min() #use min value to display

        ax.errorbar(dfn["T"], dfn["mean_mag"], yerr = dfn["std_mag"], label="N = {}".format(n), linestyle='dashed', marker='x')

    ax.set_title("Mean Magnetisation vs Temperature, H = {}".format(dfn["H"].mean()))
    ax.legend(loc="lower left")
    ax.set_xlabel("Temperature (K / J)")
    ax.set_ylabel("Mean Magnetisation (M)")

    ax.axhline(0, color="black")
    ax.grid()

def plotSusceptibility(mean_mag):
    fig, ax = plt.subplots()
    fig1, ax1 = plt.subplots()
    # ns = [20]
    lns = [400]
    for n in lns:
        dfn = mean_mag.loc[mean_mag["n"]==n]
        dfn["mean_susceptibility"] = dfn["mean_susceptibility"] / n**2

        ax.plot(dfn["T"], dfn["mean_susceptibility"], label="N = {}".format(n), linestyle='dashed', marker='x')

        ax1.plot(np.log(dfn["T"]), np.log(dfn["mean_susceptibility"]), label = "n = {}".format(n))
       # Tc = getCriticalTemperature(dfn)
     #   ax.axvline(x = Tc)

    ax.grid()
    ax.set_xlabel("Temperature")
    ax.set_ylabel("Susceptibility")
    ax.set_title("Susceptibility vs Temperature per spin")
    ax.legend(loc="lower left")
    ax1.set_title("Log Susceptibility vs Log Temperature")
    ax1.legend(loc="lower left")


def plotDecorrelationTimeVsTemperature(df):
    fig, ax = plt.subplots()

    for n in ns:
        dfn = df.loc[(df["n"]==n)].sort_values("T")

        ax.plot(dfn["T"], dfn["mean_t_decor"], label="lattice, n = {}".format(n), linestyle='dashed', marker='s')

    ax.set_title("Decorrelation Time vs Temperature")
    ax.legend(loc="lower left")


def plotDecorrelationTimeVsN(df, df_Tc):
    fig, ax = plt.subplots()
    maxDecorTimes = []
    decorTimeErrs = []

    ns_use = ns[:6]

    for n in ns_use:
        dfn = df.loc[(df["n"]==n)].sort_values("T")
        Tc = df_Tc.loc[df_Tc["n"]==n]["mean_Tc"].mean()
        criticalDecorTime = dfn.loc[dfn["T"] == Tc]["mean_t_decor"].mean()
        #criticalDecorTime = dfn.loc[dfn["T"] == 2.27]["mean_t_decor"].mean()
        #criticalDecorTime = max(dfn["mean_t_decor"])

        decorTimeErr = dfn.loc[dfn["T"] == Tc]["std_t_decor"].mean()
        maxDecorTimes.append(criticalDecorTime)
        decorTimeErrs.append(decorTimeErr / criticalDecorTime)

    x = np.log(ns_use)
    y = np.log(maxDecorTimes).flatten()

    slope, intercept = np.polyfit(x, y, 1)
    line_vals = intercept + slope * x
    slope_err = np.mean((y - line_vals) ** 2)

    ax.plot(np.unique(x), np.poly1d(np.polyfit(x, y, 1))(np.unique(x)), color = 'k', linestyle = '--', label = "Line of best fit, slope = {:.2f}, error = {:.2f}".format(slope, slope_err))

  #  ax.scatter(x, y, label="Critical decorrelation time", linestyle='dashed', marker='o')
    ax.errorbar(x, y, yerr=decorTimeErrs, fmt='o', label="Critical decorrelation time", linestyle='', marker='o')

    ax.set_title("Log decorrelation Time vs Log lattice size")
    ax.legend(loc="lower right")
    ax.set_ylabel("Log decorrelation time")
    ax.set_xlabel("Log N")
    ax.grid()


def plotHeatCapacityVsTemperature(df):
    fig, ax = plt.subplots()
    Tc = 2.269
    x = np.linspace(0, 5, 6000)
    y = -2.1*np.log(abs(Tc -x))
    ax.plot(x, y, linestyle = "--", label="Analytical")
    show_ns = [10, 200]
    for n in show_ns:
        dfn = df.loc[(df["n"]==n)].sort_values("T")
        dfn["mean_heat_capacity"] = dfn["mean_heat_capacity"] / n**2

        ax.errorbar(dfn["T"], dfn["mean_heat_capacity"], label="N = {}".format(n), linestyle='dashed', marker='x')


    ax.set_title("Heat Capacity vs Temperature, H = {}".format(dfn["H"].mean()))
    ax.grid()
    ax.axhline(0, color="black")
    ax.legend(loc="upper left")


def plotEnergyVarianceVsTemperature(df):
    fig, ax = plt.subplots()

    for n in ns:
        dfn = df.loc[(df["n"]==n)].sort_values("T")

        ax.plot(dfn["T"], dfn["mean_energy_var"], label="lattice, n = {}".format(n), linestyle='dashed', marker='s')

    ax.set_title("Energy Variance vs Temperature")
    ax.legend(loc="upper left")



def plotMagVsTime(df, mags, n, Ts):
    fig, (ax, ax2) = plt.subplots(2)

    len = 2000

    for T in Ts:
        c=next(ax._get_lines.prop_cycler)['color']

        dfn = df.loc[(df["n"] == n) & (df["T"] == T)]

        t_eq = dfn["t_eq"].iloc[-1]
        mean_mag = dfn["mag"].iloc[-1] / n**2

        norm_mags = list(map(lambda x: x / n**2, mags[(n, T)]))
        norm_mags = norm_mags[:len]

        ax.plot(norm_mags,  label = "Temperature = {}".format(T, t_eq), color = c)

        ax.axhline(y=mean_mag, color=c, linestyle = '--')

    ax.set_title("Magnetisation vs Time, n = {}".format(n))
    ax.set_xlabel("Time step")
    ax.set_ylabel("Magnetisation (M)")
    ax.legend(loc="upper right")
    ax.grid()



def plotMagVsRandmag(df, mags, randmags, n, T):
    fig, ax = plt.subplots()
    c=next(ax._get_lines.prop_cycler)['color']

    dfn_r = df.loc[(df["n"] == n) & (df["T"] == T) & df["R"] == 1]
    dfn_u = df.loc[(df["n"] == n) & (df["T"] == T) & df["R"] == 0]

    t_eq_u = dfn_u["t_eq"].iloc[-1]
    mean_mag_u = dfn_u["mag"].iloc[-1] / n**2

    ax.plot(mags[(n, T)] / n**2, label = "Uniform initial lattice, Equilibrium time = {}".format(t_eq_u), color = c)
    ax.axvline(x=t_eq_u, color=c, linestyle = '--')
    ax.axhline(y=mean_mag_u, color=c, linestyle = '--')


    ax.set_title("Magnetisation vs Time, n = {}, T = {}".format(n, T))
    ax.set_xlabel("Time step")
    ax.set_ylabel("Magnetisation (M)")
    ax.axhline(0, color="black")
    ax.legend(loc="lower right")
    ax.grid()


def plotCorrelationVsTime(df, correlations, n, Ts):
    fig, ax = plt.subplots()

    for T in Ts:
        c=next(ax._get_lines.prop_cycler)['color']

        ax.plot(correlations[(n, T)], label = "T = {}".format(T), color=c)

        t_eq = df.loc[(df["n"] == n) & (df["T"] == T)]["t_decor"].iloc[0]
        ax.axvline(x=t_eq, color=c, linestyle='--')

    ax.axhline(0, color="black")

    ax.set_title("Autocorrelation vs Time, N = {}".format(n))
    ax.set_xlabel("Time")
    ax.set_ylabel("Autocorrelation")
    ax.legend(loc="upper right")
    ax.grid()


def plotEnergyVsTime(df, energys, n, Ts):
    fig, ax = plt.subplots()

    for T in Ts:
        c=next(ax._get_lines.prop_cycler)['color']

        ax.plot(energys[(n, T)], label = "T = {}, Total simulation time = {}".format(T, len(energys[(n, T)])), color=c)

        t_eq = df.loc[(df["n"] == n) & (df["T"] == T)]["t_eq"].iloc[-1]

        ax.axvline(x=t_eq, color=c, linestyle='--')

    ax.set_title("Energy vs Time, n = {}".format(n))
    ax.set_xlabel("Time step")
    ax.set_ylabel("Energy")
    ax.legend(loc="lower right")
    ax.grid()


def plotCriticalTemperatureVsN(df):
    fig, ax = plt.subplots()
    fig, ax1 = plt.subplots()

    Tc = 2/math.log(1+math.sqrt(2))

    ax.errorbar(df["n"], df["mean_Tc"], yerr=df["error_Tc"], fmt='o', marker ="x", label="Measured Tc")

    ax.grid()
    ax.set_xlabel("Lattice Size (N)")
    ax.set_ylabel("Critical Temperature (Tc)")
    ax.set_title("Critical Temperature vs Lattice Size")
    ax.axhline(y=Tc, color="red", linestyle="--", label="Analytical Tc")
    ax.legend(loc="upper right")

    x = 1/df["n"]

    (slope, intercept), V = np.polyfit(x, df["mean_Tc"], 1, cov=True)
    slope_err = np.sqrt(V[0][0])
    int_err = np.sqrt(V[1][1])

    ax1.plot(np.unique(x), np.poly1d(np.polyfit(x, df["mean_Tc"], 1))(np.unique(x)), color = 'gray', linestyle = '--', label = "Line of best fit\nIntercept = {:.4f}\u00B1{:.3f}".format(intercept, int_err))
    ax1.errorbar(x, df["mean_Tc"], yerr=df["error_Tc"], fmt = "o", marker = "x", label="Measured Tc" )
    ax1.grid()
    ax1.legend(loc="lower right")
    ax1.set_title("Critical Temperature vs 1/N")
    ax1.set_xlabel("Reciprocal Lattice Size (1 / N)")
    ax1.set_ylabel("Measured Critical Temperature")



df_raw, all_mags, randmags, all_energys, all_correlations, Tcs = openData()



ns = sorted(df_raw['n'].unique())
Ts = sorted(df_raw['T'].unique())

df_mean = getMeansWithError(df_raw)

df_Tc = getCriticalTemperatures(Tcs, df_mean)


# plotMagVsTemp(df_mean)
# plotSusceptibility(df_mean)
#
# plotDecorrelationTimeVsTemperature(df_mean)
#
# plotHeatCapacityVsTemperature(df_mean)
# plotEnergyVarianceVsTemperature(df_mean)
#
plotCriticalTemperatureVsN(df_Tc)

# plotDecorrelationTimeVsN(df_mean, df_Tc)
#
#
# showTs = [Ts[i] for i in range(3, len(Ts), len(Ts) // 4)]
showTs = [1.2, 2.28, 2.29, 2.5]
# #showTs.append(Ts[-1])
# #showTs = [1.2, 2.275, 3.0]
# #
# #plotMagVsRandmag(df_raw, all_mags, randmags, ns[-1], 2.15)
# plotMagVsTime(df_raw, all_mags, 20, showTs)
# plotCorrelationVsTime(df_raw, all_correlations, 200, showTs)
# plotEnergyVsTime(df_raw, all_energys, 20, showTs)
#
# plotMagAndEnergyVsTime(df_raw, all_energys, all_mags, 20, showTs)

plt.show()