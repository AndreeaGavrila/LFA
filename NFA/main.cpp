#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

class NFA
{
    int q;          //nr. stari totale
    int f;          //nr. stari finale
    int *F;         //starea/starile finala/finale
    int v;          //nr. litere continute de alfabet
    char *V;        //alfabetul
    int g;          //cate triplete/tranzitii am
    struct tranz
    {
        int a,b;    //tripletele/tranzitiile
        char c;
    } *G;
    int sc;         //starea curenta
    int exista;     //variabila cu valoarea 0 daca cuvantul nu apartine nu este acceptat si 1 in caz contrar

public:
    NFA(char *fisier)           //constructor
    {
        int i;
        ifstream in(fisier);
        in>>q>>f;               ///citesc nr. stari totale si nr. stari finale
        F=new int[f];           //aloc spatiu pt. starile finale
        for (i=0;i<f;i++)
            in>>F[i];           ///citesc starea/starile finale
        in>>v;                  ///citesc nr. de litere continute de alfabet
        V=new char[v];          //aloc spatiu pt. nr. de litere continute de alfabet
        for (i=0;i<v;i++)
            in>>V[i];           ///citesc literele ce compun alfabetul
        in>>g;                  ///citesc cate triplete am
        G=new tranz[g];         //aloc spatiu pentru triplete
        for (i=0;i<g;i++)
        {
            in>>G[i].a>>G[i].c>>G[i].b;         ///citesc tripletele
        }
        sc=0;                   //starea curenta=0
        exista=0;               //presupun cuvantul ca neapartinand alfabetului
    }

    void show(); //functie ce afiseaza tripletele

    bool verificare(char *cuvant, int sc, int pozitia_in_cuvant);
        //afiseaza true daca cuvantul apartine alfabetului,false in caz contrar

};

void NFA::show()
{
    int i;
    for (i=0;i<g;i++)
        cout<<G[i].a<<" "<<G[i].c<<" "<<G[i].b<<"\n";
}

//Functia ce urmeaza face practic un backtracking, incercand toate combinatiile posibile.
//Daca dupa una sau mai multe combinatii s-a ajuns la sfarsitul cuvantului si intr-o stare finala, inseamna ca ceea
//ce am procesat este OK (returneaza true), alfel daca s-au incercat toate combinatiile posibile si s-a ajuns la sfarsitul
//cuvantului dar nu ma aflu intr-o stare terminala, returnez false
bool NFA::verificare(char *cuvant, int sc, int pozitia_in_cuvant)
{
    int i;
    //daca am terminat cuvantul si nu am depasit numarul de stari
    if ((pozitia_in_cuvant==strlen(cuvant))&&(sc<=q))
    {
        //verific daca ma aflu intr-o stare finala
        for (i=0;i<f;i++)

            //daca avem: "exista" primeste 1, adica cuvantul procesat este OK
            //(variabila exista imi este necesara pentru ca fiind un backtracking,
            //ar fi incercat toate posibilitatile chiar daca nu mai era nevoie
        //(adica cazul cand cuvantul este OK, nu ar fi fost luat in considerare,
        //acel return doar terminandu-mi apelul recursiv si ducandu-ma la instructiunea din varful stivei))

            if (sc==F[i])
            {
                exista=1;
                return true;
            }
    }
    else
    {
        for (i=0;i<g;i++)
        {
    //daca exista este 1, nu il las sa se mai apeleze recursiv, ci ii opresc orice apel recursiv curent sau
    //aflat pe stiva returnand true, astfel fortandu-l sa se termine
            if (exista==1)
                return true;

    //daca if-ul de mai sus nu este executat, testez daca starea mea curenta si litera curenta din cuvant
    // se afla in vreun triplet, iar in caz afirmativ backtracking-ul continua
            if ((G[i].a==sc)&&(G[i].c==cuvant[pozitia_in_cuvant]))
            {
                verificare(cuvant,G[i].b,pozitia_in_cuvant+1);
            }
        }
//daca backtracking-ul s-a terminat fara ca "executa" sa se modifice din 0 in 1,
// inseamna ca nu este OK cuvantul procesat, deci returnam false;
        return false;
    }
}

int main()
{
    //declaram un obiect de tip NFA
    NFA automat1("nfa.in");

    //apelam metoda "verificare" a clase NFA, cu parametrii: cuvantul ce ne dorim a fi testat,
    //starea initiala si pozitia in cuvant
    if (automat1.verificare("abac",0,0)==true)
        cout<<"Da \n";
    else
        cout<<"Nu \n";
        automat1.show();
    return 0;
}

//"aabb"
//2
//2
//0 1
//2
//a b
//4
//0 a 0
//0 b 1
//1 b 1
//1 a 0

