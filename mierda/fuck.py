'''
Created on 19/07/2018

@author: ernesto
'''

# XXX: http://codeforces.com/problemset/problem/220/E
#  XXX: https://practice.geeksforgeeks.org/problems/magic-triplets/0


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
        r = 0
        if i > 0:
            add = 0
            mul = 0
            start = i
            while i > 0:
                add += self.dataAdd[i]
                mul += self.dataMul[i]
                i -= i & -i
            r = mul * start + add
        return r

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
#        print("j {} s {}".format(j, self.sum(j)))
#        print("i {} s {}".format(i, self.sum(i)))
        return self.sum(j) - self.sum(i - 1)

    def sum_back(self, i):
        return self.sum_range(i, self.size - 1)
    
    def add_point(self, i, by):
        self.add(i, i, by)

    
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
    print("nums {}".format(numseo))

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


def quita(bi, bd, a, i):
    return modifica(bi, bd, a, i, False)


def anade(bi, bd, a, i):
    return modifica(bi, bd, a, i, True)


def modifica(bi, bd, a, i, anade):
    if anade:
        bc = bi
        fac = 1
    else:
        bc = bd
        fac = -1
        
    inv = bi.sum(a[i] + 1) + bd.sum(a[i] - 1)
    bc.add_point(a[i], fac)
    
    return inv


def core(nums, nmi):
    numse = crea_arreglo_enriquecido(nums)
    print("numse {}".format(numse))
    a = list(map(lambda x:x.posicion_final + 1, numse))
    print("a {}".format(list(map(lambda x:x - 1, a))))
    i = 0
    j = 0
    ni = 0
    r = 0
    a_len = len(a)
    
    bitch_izq = RangeBit(a_len + 2)
    bitch_der = RangeBit(a_len + 2)
    
    nif = 0
    for i, x in enumerate(a):
        nif += bitch_der.sum_range(x + 1, a_len)
        bitch_der.add(x, x, 1)
        print("en x {} ({}) nif {}".format(x, numse[i].valor , nif))

    j = 0
    ni = nif
    print("ni ini {}".format(ni))
    for i in range(1, a_len):
        while j < a_len and (j < i or ni > nmi):
            ni -= quita(bitch_izq, bitch_der, a, j)
            print("en j {} se kito {}".format(j, ni))
            j += 1
        r += a_len - j
        print("en i {} j {} anadido {} inv {}".format(i, j, a_len - j, r))
        ni += anade(bitch_izq, bitch_der, a, i)
        print("en i {} se puso {}".format(i, ni))
        
#    print("r f aora {}".format(r))
        
    return r


_, k = [int(x) for x in input().strip().split(" ")]
nums = [int(x) for x in input().strip().split(" ")]
# nums = [6, 8, 6, 7, 2, 4, 2, 1, 7, 6, 2, 1, 2, 3, 2, 5, 3, 7, 1, 7, 7]
# nums = [1, 3, 2, 1, 7]
# nums = [1, 3, 2]
# k = 0

print(core(nums, k))
