import os.path
import sys


def inp():return int(sys.stdin.readline())
def st():return list(sys.stdin.readline().strip())
def li():return list(map(int,sys.stdin.readline().split()))
def mp():return map(int,sys.stdin.readline().split())

def pr(n):return sys.stdout.write(str(n)+"\n")
def prl(n):return sys.stdout.write(str(n)+"")

if os.path.exists('input.txt'):
    sys.stdin = open( 'input.txt','r')
    sys.stdout = open('output.txt','w')

import numpy as np

import pandas as pd
pd.options.display.max_columns=10
pd.options.display.max_colwidth=12

import matplotlib.pyplot as plt

# Plot Queue Length Over Time
queue_time, queue_length = [], []
with open("queue_data.txt", "r") as f:
    for line in f:
        t, q = map(float, line.split())
        queue_time.append(t)
        queue_length.append(q)

plt.figure(figsize=(10, 5))
plt.plot(queue_time, queue_length, label="Queue Length", color="blue")
plt.xlabel("Time (minutes)")
plt.ylabel("Queue Length")
plt.title("Queue Length Over Time")
plt.legend()
plt.grid()
plt.show()

# Plot Average Waiting Time Per Period
waiting_time_period, avg_waiting_time = [], []
with open("waiting_time.txt", "r") as f:
    for line in f:
        t, w = map(float, line.split())
        waiting_time_period.append(t)
        avg_waiting_time.append(w)

plt.figure(figsize=(8, 5))
plt.bar(waiting_time_period, avg_waiting_time, width=50, color="green", alpha=0.7)
plt.xlabel("Time Period Start (minutes)")
plt.ylabel("Average Waiting Time (minutes)")
plt.title("Average Waiting Time per Time Period")
plt.grid()
plt.show()
