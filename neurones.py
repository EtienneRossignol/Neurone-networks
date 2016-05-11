from random import random,randint
from math import exp

def g(x):
    try:
        return 1/(1+exp(-x))
    except:
        return 0
def g2(x):
    try:
        return exp(-x)/((exp(-x)+1)**2)
    except:
        return 0
'''
class CouheNeurones:
    def __init__(self, tailleG,tailleD):
        self.tailleG=tailleG+1
        self.tailleD=tailleD
        self.poid=[[random()*2-1 for _ in range(self.tailleG)] for i in range(tailleD)]
        self.valeur=[0]*tailleD
        self.erreurD=[0]*tailleD
        self.erreurG=[0]*self.tailleG
        self.v=[0]*tailleD
    def calculerSortie(self, entre):
        entre.append(-1)
        #print (self.poid,entre)
        for i in range(self.tailleD):
            v[i]=0
            for j in range(self.tailleG):
                v[i]+=entre[j]*self.poid[i][j]
            self.valeur[i]=g(v[i])
    def siSortie(self, sortie):
        for i in range(self.tailleD):
            self.erreurD[i]=g2(self.v[i])*(sortie[i]-self.valeur[i])
    def retro(self):
        for i in range(self.tailleG):
            self.erreurG=g2(self)

class ReseauNeurone:
    def __init__(self, *arg):
        self.nbCouches=len(arg)-1
        self.arg=arg
        self.couches=[CouheNeurones(arg[i],arg[i+1]) for i in range(len(arg)-1)]
    def calculerSortie(self, entre):
        self.couches[0].calculerSortie(entre)
        for i in range(1,self.nbCouches):
            self.couches[i].calculerSortie(self.couches[i-1].valeur)
        return self.couches[self.nbCouches-1].valeur
    def retropropogationDuGradient(self,entre,sortie):
        self.calculerSortie(entre)
        self.couches[self.nbCouches-1]'''

apprentissage=1.
class Reseau:
    def __init__(self, *arg):
        self.nbCouches=len(arg)
        self.arg=arg
        self.valeur=[[0]*arg[i]+[-1] for i in range(len(arg))]
        self.dans=[[0]*arg[i]+[-1] for i in range(len(arg))]
        self.erreur=[[0]*(arg[i]+1) for i in range(len(arg))]
        self.nouveauPoid=[[[0]*arg[i+1] for v in range (arg[i]+1)]
                        for i in range(len(arg)-1)]
        self.poid= [[[random()*2-1 for _ in range(arg[i+1])]
            for v in range (arg[i]+1)]for i in range(len(arg)-1)]
    def calculerSortie(self, entre):
        for i in range(len(entre)):
            self.valeur[0][i]=entre[i]
            self.dans[0][i]=entre[i]
        for couche in range(1,self.nbCouches):
            for el in range(self.arg[couche]):
                self.dans[couche][el]=0
                for fils in range(self.arg[couche-1]+1):
                    self.dans[couche][el]+=self.valeur[couche-1][fils]*self.poid[couche-1][fils][el]
                self.valeur[couche][el]=g(self.dans[couche][el])

        return self.valeur[-1][:-1]

    def apprentissageExemple(self,L):
        self.nouveauPoid=[[[0]*self.arg[i+1] for v in range (self.arg[i]+1)]
                        for i in range(len(self.arg)-1)]
        for entre,sortie in L:
            self.calculerSortie(entre)
            self.calculErreur(entre,sortie)
            self.ajoutPoid()
        self.miseAjourPoid()
    def retropropogationDuGradient(self,entre,sortie):
        self.calculerSortie(entre)
        for i in range(self.arg[-1]):
            self.erreur[-1][i]=g2(self.dans[-1][i])*(sortie[i]-self.valeur[-1][i])
        for couche in range(self.nbCouches-2,-1,-1):
            for n in range(self.arg[couche]):
                val=0
                for pere in range(self.arg[couche+1]):
                    val+=self.erreur[couche+1][pere]*self.poid[couche][n][pere]
                self.erreur[couche][n]=g2(self.dans[couche][n])*val

        for couche in range(self.nbCouches-1):
            for el in range(self.arg[couche]+1):
                for pere in range(self.arg[couche+1]):
                    self.poid[couche][el][pere]+=apprentissage*self.valeur[couche][el]*self.erreur[couche+1][pere]
    def ajoutPoid(self):
        for couche in range(self.nbCouches-1):
            for el in range(self.arg[couche]+1):
                for pere in range(self.arg[couche+1]):
                    self.nouveauPoid[couche][el][pere]+=apprentissage*self.erreur[couche+1][pere]
    def miseAjourPoid(self):
        for couche in range(self.nbCouches-1):
            for el in range(self.arg[couche]+1):
                for pere in range(self.arg[couche+1]):
                    self.poid[couche][el][pere]+=self.nouveauPoid[couche][el][pere]

    def calculErreur(self,entre,sortie):
        for i in range(self.arg[-1]):
            self.erreur[-1][i]=g2(self.dans[-1][i])*(sortie[i]-self.valeur[-1][i])
        for couche in range(self.nbCouches-2,-1,-1):
            for n in range(self.arg[couche]):
                val=0
                for pere in range(self.arg[couche+1]):
                    val+=self.erreur[couche+1][pere]*self.poid[couche][n][pere]
                self.erreur[couche][n]=g2(self.dans[couche][n])*val

if __name__=='__main__':
    a=Reseau(1,5,4,1)
    a.retropropogationDuGradient([2],[7])