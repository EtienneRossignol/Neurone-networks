#include<cstdio>
#include<cmath>
#include<ctime>
#include<cstdlib>


inline double G(double x)
{
	if (x < -100)return 0;
	return 3. / (1. + exp(-x));
}
inline double gprime(double x)
{
	if (x < -100)return 0;
	return 3.*exp(-x) / pow((exp(-x) + 1),2);
}/*
inline double G(double x)
{
	return x;
}
inline double gprime(double x)
{
	return 1;
}*/

const double APP = .3;

struct Reseau
{
	int positionNoeud[10];
	int positionPoid[10];
	int tailleCouche[10];
	int nbCouche;
	double valeur[1000];
	double erreur[1000];
	double dans[1000];
	double poid[1000 ]; // indice1 -> couche indice2 ->pere indice3 -> (fils+1)
	double modificationPoid[1000];
	Reseau(int nb, int *val)
	{
		srand(time(NULL));
		nbCouche = nb;
		tailleCouche[0] = *(val);
		positionNoeud[0] = positionPoid[0] = 0;
		for (int i = 1; i < nbCouche; i++)
		{
			positionNoeud[i] = positionNoeud[i - 1] + *(val + i - 1) + 1;
			valeur[positionNoeud[i]-1]=-1;
			dans[positionNoeud[i] - 1] = -1;
			positionPoid[i] = positionPoid[i - 1] + ((*(val + i - 1)+1) * *(val + i));
			tailleCouche[i] = *(val + i);
			//printf("%d %d %d\n", positionNoeud[i], positionPoid[i], tailleCouche[i]);
		}
		valeur[positionNoeud[nbCouche-1]+tailleCouche[nbCouche-1]] = -1;
		dans[positionNoeud[nbCouche - 1] + tailleCouche[nbCouche - 1]] = -1;
		for (int i = 0; i < positionPoid[nbCouche - 1]; i++)
		{
			poid[i] = (((double)rand())/RAND_MAX)*2-1;
			//printf("%f\n", poid[i]);
		}
	}
	void apprentissage(int nbExemple, double *exemples)
	{
		int tailleEntre=tailleCouche[0];
		int tailleBlock = tailleEntre + tailleCouche[nbCouche - 1];
		for (int i = 0; i < positionPoid[nbCouche - 1]; i++)
			modificationPoid[i] = 0;
		for (int ex = 0; ex < nbExemple; ex++)
		{
			calculerSortie(exemples+ex*tailleBlock);
			calculerErreur(exemples + ex*tailleBlock + tailleEntre);
			//printf("%f\n", *(exemples + ex*tailleBlock + tailleEntre));
			ajoutPoid();
		}
		miseAjour();
	}
	void apprentissageSimple(int nbExemple, double *exemples)
	{
		int tailleEntre = tailleCouche[0];
		int tailleBlock = tailleEntre + tailleCouche[nbCouche - 1];
		for (int i = 0; i < positionPoid[nbCouche - 1]; i++)
			modificationPoid[i] = 0;
		for (int ex = 0; ex < nbExemple; ex++)
		{
			calculerSortie(exemples + ex*tailleBlock);
			calculerErreur(exemples + ex*tailleBlock + tailleEntre);
			//printf("%f\n", *(exemples + ex*tailleBlock + tailleEntre));
			ajoutPoid();
			miseAjour();
		}
	}
	void testerExemple(int nbExemple, double *exemples)
	{
		int tailleEntre = tailleCouche[0];
		int tailleBlock = tailleEntre + tailleCouche[nbCouche - 1];
		for (int ex = 0; ex < nbExemple; ex++)
			printf("%f\t",*calculerSortie(exemples + ex*tailleBlock));
	}
	double * calculerSortie(double *entre) // retour -> valeur derniere ligne
	{
		for (int i = 0; i < tailleCouche[0]; i++)
			dans[i] = valeur[i] = *(entre + i);
		for (int couche = 1; couche < nbCouche; couche++)
		{
			for (int n = 0; n < tailleCouche[couche]; n++)
			{
				dans[n + positionNoeud[couche]] = 0;
				for (int fils = 0; fils <= tailleCouche[couche - 1]; fils++)
					dans[n + positionNoeud[couche]] += valeur[fils + positionNoeud[couche - 1]] * 
						poid[positionPoid[couche - 1] + fils + n*(tailleCouche[couche - 1] + 1)];
				valeur[n + positionNoeud[couche]] = G(dans[n + positionNoeud[couche]]);
			}
		}
		return valeur + positionNoeud[nbCouche - 1];
	}
	void calculerErreur(double *sortie)
	{
		for (int i = 0; i < tailleCouche[nbCouche-1]; i++)
			erreur[positionNoeud[nbCouche - 1] + i] = gprime(dans[positionNoeud[nbCouche - 1] + i]) * 
				( (*(sortie + i)) - valeur[positionNoeud[nbCouche - 1] + i]);
		for (int couche = nbCouche - 2; couche >= 0; couche--)
		{
			for (int n = 0; n < tailleCouche[couche]; n++)
			{
				double val = 0;
				for (int pere = 0; pere < tailleCouche[couche + 1]; pere++)
					val += erreur[positionNoeud[couche + 1]+pere]*
						poid[positionPoid[couche] + n + pere*(tailleCouche[couche] + 1)] ;
				erreur[positionNoeud[couche] + n] = gprime(dans[positionNoeud[couche] + n])*val;
			}
		}
	}
	void ajoutPoid()
	{
		for (int couche = 0; couche < nbCouche-1; couche++)
			for (int n = 0; n <= tailleCouche[couche]; n++)
				for (int pere = 0; pere < tailleCouche[couche + 1]; pere++)
					modificationPoid[positionPoid[couche] + n + pere*(tailleCouche[couche] + 1)]+=
						APP*erreur[positionNoeud[couche+1]+pere];
	}
	void miseAjour()
	{
		for (int couche = 0; couche < nbCouche - 1; couche++)
			for (int n = 0; n <= tailleCouche[couche]; n++)
				for (int pere = 0; pere < tailleCouche[couche + 1]; pere++)
					poid[positionPoid[couche] + n + pere*(tailleCouche[couche] + 1)] +=
					modificationPoid[positionPoid[couche] + n + pere*(tailleCouche[couche] + 1)];
	}
};