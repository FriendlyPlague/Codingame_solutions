import sys
import math

#https://www.codingame.com/training/medium/death-first-search-episode-1

n, l, e = [int(i) for i in input().split()]
lList = []
for i in range(l):
    n1, n2 = [int(j) for j in input().split()]
    lList.append((n1,n2))
ei = []
for i in range(e):
    ei.append(int(input()))
class nodePath():
    def __init__(self,rn):
        self.rn = rn
        self.rl = []
        self.ni = []
        for i in lList:
            if self.rn in i:
                self.rl.append(i)
                self.ni.append([])
                if self.rn == i[0]:
                    self.ni[len(self.rl)-1].append(i[1])
                if self.rn == i[1]:
                    self.ni[len(self.rl)-1].append(i[0])
        self.iteration = 1
    def next_iteration(self):
        tl = []
        counter = 0
        for i in self.ni:
            tl.append([])
            for j in i:
                for c in lList:
                    if j == c[0]:
                        tl[counter].append(c[1])
                    if j == c[1]:
                        tl[counter].append(c[0])
            counter += 1
        self.ni = tl
        self.iteration += 1
    def reset(self):
        self.ni = []
        counter = 0
        for i in self.rl:
            self.ni.append([])
            if i[0] == self.rn:
                self.ni[counter].append(i[1])
            elif i[1] == self.rn:
                self.ni[counter].append(i[0])
            counter += 1
n_paths = []
killedLinks = []
for i in ei:
    n_paths.append(nodePath(i))
while True:
    si = int(input())
    ans = []
    cont = True
    while cont:
        for np in n_paths:
            if cont == False:
                    break
            for i in range(len(np.ni)):
                if cont == False:
                    break
                for j in np.ni[i]:
                    if j == si:
                        ans = np.rl[i]
                        del np.ni[i]
                        killedLinks.append(np.rl.pop(i))
                        lList.remove(ans)
                        cont = False
                        break
        for np in n_paths:
            np.next_iteration()
    for np in n_paths:
        np.reset()
    print(*ans, sep=" ")