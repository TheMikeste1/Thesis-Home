__all__ = ['numberline']

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


def __setup(ax: plt.Axes, xmin: float, xmax: float):
    # https://matplotlib.org/2.0.2/examples/ticks_and_spines/tick-locators.html
    ax.spines['right'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.yaxis.set_major_locator(ticker.NullLocator())
    ax.spines['top'].set_color('none')
    ax.xaxis.set_ticks_position('bottom')
    ax.tick_params(which='major', width=1.00)
    ax.tick_params(which='major', length=5)
    ax.tick_params(which='minor', width=0.75)
    ax.tick_params(which='minor', length=2.5)
    ax.set_xlim(xmin, xmax)
    ax.set_ylim(0, 1)
    ax.patch.set_alpha(0.0)


def numberline(points: [], xmin, xmax, width=6) -> plt.Axes:
    fig, ax = plt.subplots(1, 1)
    fig.set_size_inches(width, 0.5)

    __setup(ax, xmin, xmax)
    ax.xaxis.set_major_locator(ticker.AutoLocator())
    ax.xaxis.set_minor_locator(ticker.AutoMinorLocator())
    ax.scatter(points, np.zeros(len(points)), clip_on=False, zorder=3)

    return ax
