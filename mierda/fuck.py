'''
Created on 19/07/2018

@author: ernesto
'''

# XXX: http://codeforces.com/problemset/problem/220/E


# XXX: https://gist.github.com/robert-king/5660418
class RangeBit:

    def __init__(self, n):
        sz = 1
        while n >= sz:
            sz *= 2
        self.size = sz
        self.dataAdd = [0] * sz
        self.dataMul = [0] * sz

    def sum(self, i):
        assert i > 0
        add = 0
        mul = 0
        start = i
        while i > 0:
            add += self.dataAdd[i]
            mul += self.dataMul[i]
            i -= i & -i
        return mul * start + add

    def add(self, left, right, by):
        assert 0 < left <= right
        self._add(left, by, -by * (left - 1))
        self._add(right, -by, by * right)

    def _add(self, i, mul, add):
        assert i > 0
        while i < self.size:
            self.dataAdd[i] += add
            self.dataMul[i] += mul
            i += i & -i
    
    def sum_range(self, i, j):
        print("j {} s {}".format(j, self.sum(j)))
        print("i {} s {}".format(i, self.sum(i)))
        return self.sum(j) - self.sum(i - 1)


class numero():

    def __init__(self, valor, posicion_inicial):
        self.valor = valor
        self.posicion_inicial = posicion_inicial
        self.posicion_final = -1;
    
    def __lt__(self, other):
        if self.valor == other.valor:
            r = self.posicion_inicial < other.posicion_inicial
        else:
            r = self.valor < other.valor
        return r

    def __repr__(self):
        return "{}:{}:{}".format(self.valor, self.posicion_inicial, self.posicion_final)


def swap(a, i, j):
    a[i], a[j] = a[j], a[i]


def crea_arreglo_enriquecido(nums):
    numso = list(sorted(nums))
    numse = list(map(lambda t:numero(t[1], t[0]), enumerate(nums)))
    numseo = []
    for i in range(len(nums)):
        if nums[i] != numso[i]:
            numseo.append(numse[i]) 
    
    numseo = list(sorted(numseo))

    j = 0
    for i in range(len(nums)):
        if nums[i] != numso[i]:
            numse[i] = numseo[j]
            j += 1
        numse[i].posicion_final = i

    for i in range(len(nums)):
        while i != numse[i].posicion_inicial:
            swap(numse, i, numse[i].posicion_inicial)
    return numse


def fuerza_bruta(a):
    a_len = len(a)
    r = 0
    for i in range(a_len):
        for j in range(i):
            if a[i] < a[j]:
                r += 1
    return r


def core(nums, nmi):
    numse = crea_arreglo_enriquecido(nums)
    a = (list(map(lambda x:x.posicion_final + 1, numse)))
    print(a)
    i = 0
    j = 0
    ni = 0
    r = 0
    lmin = False
    a_len = len(a)
    bitch = RangeBit(a_len + 2)
    bitch.add(a[0], a[0], 1)
    
    while True:
        if ni <= nmi:
            j += 1
            if j == a_len:
                break
            bitch.add(a[j], a[j], 1)
            ni += bitch.sum_range(a[j] + 1, a_len)
#            print("anadido {} aora {}".format(a[j], ni))
            lmin = True
        else:
            bitch.add(a[i], a[i], -1)
            if a[i] - 1:
                ni -= bitch.sum(a[i] - 1)
#            print("kitado {} aora {}".format(a[i], ni))

            if lmin and ni > nmi:
                n = j - i - 1
#                print("la ventana es  i {} j {} n {}".format(i, j, n))
                r += (n * (n + 1)) >> 1
#                print("r aora {}".format(r))
                
            lmin = False
            i += 1
        
        caca = fuerza_bruta(a[i:j + 1])
        assert caca == ni, "caca {} ni {} en nums {}".format(caca, ni, a[i:j + 1])
        
    j -= 1
    while ni > nmi :
        assert i < j
        bitch.add(a[i], a[i], -1)
        if a[i] - 1:
            ni -= bitch.sum(a[i] - 1)
        i += 1
        caca = fuerza_bruta(a[i:j + 1])
        assert caca == ni, "caca f {} ni {} en nums {}".format(caca, ni, a[i:j + 1])
    
    if i < j:
        assert ni <= nmi
        n = j - i
#        print("la ventana f es  i {} j {} n {}".format(i, j, n))
        r += (n * (n + 1)) >> 1
#        print("r f aora {}".format(r))
        
    return r


nums = [6, 8, 6, 7, 2, 4, 2, 1, 7, 6, 2, 1, 2, 3, 2, 5, 3, 7, 1, 7, 7]
# nums = [1, 3, 2, 1, 7]
#nums = [1, 3, 2]
k = 0

print(core(nums, k))
