import matplotlib.pyplot as plt
import pandas as pd


def openData():
    f = open("../results/hysteresis.csv")

    new_set = True
    data = []

    while True:
        line = f.readline()
        if not line: break #EOF

        if line == "\n":
            new_set = True
            continue

        if new_set:
            n, T = line.split(",")
            n, T = int(n), float(T)
            new_set = False
            continue

        H, M = line.split(",")
        H, M = float(H), float(M)

        data.append({
            "n": n,
            "T": T,
            "H": H,
            "M": M / n**2,
        })

    results = pd.DataFrame(data, columns=data[0].keys())
    return results


df = openData()

print(df)

fig, ax = plt.subplots()
ax.axhline(0, color='gray')
ax.axvline(0, color='gray')

#for T in df["T"].unique():
for T in [2.1, 2.3, 2.6]:
    dfn = df.loc[df["T"]==T]

    ax.plot(dfn["H"], dfn["M"], label = "T = {}".format(T))
    ax.set_title("Hysteresis Loop, n = {}".format(dfn["n"].min()))

    ax.set_xlabel("Applied Field (H)")
    ax.set_ylabel("Magnetisation (M)")

    ax.grid()

ax.legend()
plt.show()
