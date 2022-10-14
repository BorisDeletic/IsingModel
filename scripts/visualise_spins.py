import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

from matplotlib import animation


def conv(s):
    try:
        s=float(s)
    except ValueError:
        pass
    return s


f = open("../results/spins.csv")

line = f.readline()
n, steps = line.split(",")
n, steps = int(n), int(steps)

i = 0
spins = np.empty((steps, n, n))
mags = np.empty(steps)
Ts = np.empty(steps)
Hs = np.empty(steps)

while True:
    line1 = f.readline()
    line2 = f.readline()
    if not line2: break #EOF

    T, H, mag = line1.split(",")

    spins_step = np.array(list(map(conv, line2.split(",")[:-1])))
    spins[i] = spins_step.reshape((n,n))

    mags[i] = float(mag)
    Hs[i] = float(H)
    Ts[i] = float(T)

    i+=1


f.close()

mags /= n**2


ns = np.linspace(0, n, n)


fig, (ax1, ax2) = plt.subplots(1, 2)
fig.set_figheight(7)
fig.set_figwidth(14)

time_text = ax1.text(0.05, 0.95,'',horizontalalignment='left',verticalalignment='top', transform=ax1.transAxes)

cax = ax1.pcolormesh(ns, ns, spins[0], vmin=-1, vmax=1, cmap=cm.gray)
line1, = ax2.plot(Hs, label = "H field")
line2, = ax2.plot(mags, label = "Magnetisation")

ax1.set_xlabel("x")
ax1.set_ylabel("y")

ax2.set_xlabel("Time")
ax2.set_ylabel("Applied Field (H) / Magnetisation (M)")
ax2.set_title("Applied Field and Magnetisation of Lattice vs Time")
ax2.grid()
ax2.axhline(0, color="black")
ax2.axvline(0, color="black")
ax2.legend(loc ="lower right")

xdata, ydata1, ydata2 = [], [], []
def animate(i):
    xdata.append(i)
    ydata1.append(Hs[i])
    ydata2.append(mags[i])

    line1.set_data(xdata, ydata1)
    line2.set_data(xdata, ydata2)

    cax.set_array(spins[i].flatten())
    ax1.set_title("Lattice spins below critical temperature\nTime = {}, Temperature = {}".format(i, Ts[i]))

anim = animation.FuncAnimation(fig, animate, interval=100, frames=steps-1, repeat=False)
anim.save('spins_through_hysteresis_n{}.gif'.format(n))



plt.show()
