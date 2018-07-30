'''
Created on 19/07/2018

@author: ernesto
'''

# XXX: http://codeforces.com/problemset/problem/220/E

from enum import Enum


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


# I inversion
# F frente
# A atras
# D dentro
# F fueraa
# XXX: https://stackoverflow.com/questions/36932/how-can-i-represent-an-enum-in-python
CASOS_INVERSIONES = Enum("CASOS_INVERSIONES ", "IFD IFF IAD IAF NA")


def determinar_caso(li, ld, n):
    r = CASOS_INVERSIONES.NA
    pf = n.posicion_final
    pi = n.posicion_inicial

    if pf > ld:
        r = CASOS_INVERSIONES.IFF
    else:
        if pf > pi:
            r = CASOS_INVERSIONES.IFD
        else:
            if pf < li:
                r = CASOS_INVERSIONES.IAF
            else:
                if pf < pi:
                    r = CASOS_INVERSIONES.IAD
                else:
                    r = CASOS_INVERSIONES.NA
    return r


class ventana():

    def __init__(self, numse):
        self.li = 0
        self.ld = 0
        self.df = set()
        self.dam = {}
        self.da = set()
        self.ic = 0
        self.numse = numse

    def anadir(self, n):
        self.ld += 1
        assert self.ld == n.posicion_inicial, "el unm {} ld {}".format(n, self.ld)
        assert n.posicion_inicial not in self.da
        
        c = determinar_caso(self.li, self.ld, n)
        assert c != CASOS_INVERSIONES.IFD
        
        self.ic += len(self.df)
        if n.posicion_inicial in self.df:
            self.df.remove(n.posicion_inicial)
        
        if c == CASOS_INVERSIONES.IFF:
            self.df.add(n.posicion_final)
        if c == CASOS_INVERSIONES.IAF:
            self.ic += self.ld - self.li 
            self.da.add(n.posicion_inicial)
        if c == CASOS_INVERSIONES.IAD:
            self.dam[n.posicion_final] = n
            nd = self.numse[n.posicion_final]
            assert self.li <= nd.posicion_inicial <= self.ld
            assert nd.posicion_inicial == n.posicion_final
            self.ic += n.posicion_inicial - n.posicion_final
            
        return self.ic

    def quitar(self, n):
        assert self.li == n.posicion_inicial
        assert n.posicion_inicial not in self.df
        
        c = determinar_caso(self.li, self.ld, n)
        
        assert c != CASOS_INVERSIONES.IAD
        
        if n.posicion_inicial in self.dam:
            assert c != CASOS_INVERSIONES.NA
            no = self.dam[n.posicion_inicial]
            assert determinar_caso(self.li, self.ld, no) == CASOS_INVERSIONES.IAD
            assert no.posicion_final == n.posicion_inicial
            del self.dam[n.posicion_inicial]
            self.da.add(no.posicion_inicial)
        
        e = n.posicion_inicial in self.da
        self.da.remove(n.posicion_inicial)
        
        self.ic -= len(self.da)
        assert self.ic >= 0
            
        self.li += 1
        
        if c == CASOS_INVERSIONES.IAF:
            assert e
        if c == CASOS_INVERSIONES.IFF:
            self.ic -= self.ld - n.posicion_inicial
            self.df.remove(n.posicion_final)
        if c == CASOS_INVERSIONES.IFD:
            nd = self.numse[n.posicion_final]
            self.ic -= n.posicion_final - n.posicion_inicial
            assert self.li <= nd.posicion_inicial <= self.ld
            assert nd.posicion_inicial == n.posicion_final
        return self.ic


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
    v = ventana(numse)
    i = 0
    j = 0
    ni = 0
    r = 0
    while True:
        if i != j and ni <= nmi:
            r += 1
        if ni < nmi:
            j += 1
            if j == len(nums):
                break
            ni = v.anadir(numse[j])
        else:
            i += 1
            ni = v.quitar(numse[i])
        
        caca = fuerza_bruta(list(map(lambda x:x.valor, numse[i:j + 1])))
        assert caca == ni, "caca {} ni {} en nums {}".format(caca, ni, numse[i:j + 1])


nums = [6, 8, 6, 7, 2, 4, 2, 1, 7, 6, 2, 1, 2, 3, 2, 5, 3, 7, 1, 7, 7]
k = 3

print(core(nums, k))
