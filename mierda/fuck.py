'''
Created on 19/07/2018

@author: ernesto
'''

# XXX: http://codeforces.com/problemset/problem/220/E

from collections import namedtuple

numero = namedtuple("numero", "valor posicion ordenados")


def compara_numeros(na, nb):
    r = 0
    ordenados = na.ordenados
    if na.valor == ordenados[na.posicion] or nb.valor == ordenados[nb.posicion]:
        r = 0
    else:
        r = na.valor - nb.valor
        if not r:
            r = na.posicion - nb.posicion
    return r
