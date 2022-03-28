import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import math

from matplotlib import animation


def conv(s):
    try:
        s=float(s)
    except ValueError:
        pass
    return s




f = open("../results/spins.csv")

line = f.readline()
n, steps, Ti, Tf = line.split(",")
n, steps, Ti, Tf = int(n), int(steps), float(Ti), float(Tf)
steps = 400
i = 0
spins = np.empty((steps, n, n))
#print(spins[0])

while True:
    line = f.readline()
    if not line: break #EOF

    spins_step = np.array(list(map(conv, line.split(",")[:-1])))
    spins[i] = spins_step.reshape((n,n))

    i+=1

steps = i

f.close()

print(spins)



fig, ax = plt.subplots()
time_text = ax.text(0.05, 0.95,'',horizontalalignment='left',verticalalignment='top', transform=ax.transAxes)

ns = np.linspace(0, n, n)
cax = ax.pcolormesh(ns, ns, spins[0], vmin=-1, vmax=1)

def animate(i):
    cax.set_array(spins[i].flatten())

    if i < 100:
        T = Ti
    elif i < 200:
        T = Tf
    else:
        T = 1.0

    ax.set_title("time step = {}, temperature = {}".format(i, T))

anim = animation.FuncAnimation(fig, animate, interval=100, frames=steps-1)
anim.save('spins_through_cooling_n500.gif')
plt.show()
