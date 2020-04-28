//DFA_to_REGEX

#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>

using namespace std;

/*
                                MATRICE SIMBOLURI

        Nume               |      Variabila    |   Tip
        =========================================================================
        nr stari           |      nrStari      |   int
        -------------------------------------------------------------------------
        starile            |      Q            |   vector <int>
        -------------------------------------------------------------------------
        litere             |      Sigma        |   vector <string>
        -------------------------------------------------------------------------
        stare intiala      |      q0           |   int
        -------------------------------------------------------------------------
        nr starilor finale |      nrFinale     |   int
        -------------------------------------------------------------------------
        starile finale     |      F            |   vector <int>
        -------------------------------------------------------------------------
        tranzitii          |      delta        |   map <pair <int, string>, int>
        -------------------------------------------------------------------------
        lista cuvinte      |      cuvant       |   string

*/

bool nouInit = false, nouFinal = false;

void citireDate(vector<int>& Q, vector<string>& Sigma, vector<int>& F, int& q0, map <pair <int, string>, int>& delta)
{
    ifstream fin("fa.in");

    /// Citirea numarului de stari
    int nrStari;
    fin >> nrStari;

    /// Citirea starilor
    for (int i = 0; i < nrStari; ++i)
    {
        int q;
        fin >> q;
        Q.push_back(q);
    }

    /// Citirea alfabetului Sigma
    string litera;
    do  //Citesc din fisier litere (de tip string) pana cand gasesc in fisier caracterul ';'. Atunci ma opresc
    {
        fin >> litera; 

        if (litera != ";" && litera != " ") //daca litera curenta NU este caracterul ';' sau ' ', retin litera
        {
            Sigma.push_back(litera);
        }
    } while (litera != ";");


    /// Citirea starii initiale
    fin >> q0;


    /// Citirea numarului de stari finale
    int nrFinale;
    fin >> nrFinale;


    /// Citirea starilor finale, pe baza numarului de stari finale citite anterior
    for (int i = 0; i < nrFinale; ++i)
    {
        int f;
        fin >> f;
        F.push_back(f);
    }


    /// Citirea tranzitiilor
    int stare1, stare2;
    litera.clear();
    do
    {
        fin >> stare1 >> litera >> stare2;  //citesc sub forma urmatoare: starea1 --litera--> starea2
        if (stare1 == -1)
            break;
        delta[{stare1, litera}] = stare2; //retin in delta (i.e.tranzitia) maparea respectiva de tip < <starea1,litera>, starea2 >
        litera.clear();                //apoi noua stare1 va lua locul fostei stari2,(i.e. starea2 devine starea1) si asteptand noua litera din citire
    } while (stare1 != -1);  //citesc aceste tranzitii pana cand noua stare1 va fi -1 si ma opresc

    fin.close();
}

bool inStare(int x, vector<int> Q)  //functia verifica daca x se afla in multimea starilor Q
{
    for (int st : Q)
        if (x == st)
            return true;
    return false;
}


void initFinal(int& q0, vector<string>& Sigma, vector<int>& Q, map<pair<int, string>, int>& delta, vector<int>& F)
{
    //functia realizeaza modificarea parametrilor primiti: starea initiala, multimea literelor citite, multimea starilor, tranzitiile si multimea starilor finale

    bool ok = true;

    for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end(); it++) //parcurg tranzitia
        if (it->second == q0)
        {
            ok = false;
            goto et;
        }
et:
    if (!ok)         
    {                       //in starea q1 am avut sigur o tranzitie de intrare si astfel
        nouInit = true;     //creez starea initiala q0 cu tranzitia λ
        int nouSt = 0;
        delta[{nouSt, "$"}] = q0;  // $ va reprezenta λ
        q0 = nouSt;
        Q.push_back(0);

    } 

    //nu stiu daca am o tranzitie de iesire din starea mea finala 
    //deci voi adauga pentru siguranta o noua stare finala cu tranzitia λ
   
    int cardF = 0;
    for (int i : F)  //pentru fiecare i in multimea starilor finale
        cardF++;    //calculez cardinalul lui F

    if (cardF > 1)
    {
        ok = false;
        goto et1;  //sar la eticheta1
    }

    ok = true;
    for (int finala : F) // parcurg fiecare stare finala din multimea starilor finale F
        for (string lit : Sigma) // parcurg fiecare litera din alfabetul meu
            
            // cazul defavorabil: este daca pot continua cu aceasta litera si mai departe de starea finala si voi marca acest lucru cu fals
            if (inStare(delta[{finala, lit}], Q) && delta[{finala, lit}] != 0)
            {
                ok = false;
                goto et1;  //sar la eticheta1
            }

et1:  //eticheta1
    if (!ok)
    {
        nouFinal = true;
        
        int last = 0;  // daca o stare finala a intrat in cazul defavorabil, voi crea alta stare finala
        for (int i : Q)
            if (i != 0)
                last = i;

        int nouSt = last + 1;  
        for (int finala : F)
            delta[{finala, "$"}] = nouSt;  //pun de asemenea si λ tranzitia

        F.clear();
        F.push_back(nouSt);
    }

    if (nouInit || nouFinal)
        Sigma.push_back("$");
}

string drum(int nod1, int nod2, map<pair<int, string>, int> delta)
{  
    for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end(); it++)
        if (it->first.first == nod1 && it->second == nod2)
            return it->first.second;
    return "";  //returnez sirul vid pentru ca se intelege de la sine ca este by default λ

// avem acum o stare initiala si o stare finala, eventual cu λ-tranzitii
}

void sterge(int nod, vector<string>& Sigma, map<pair<int, string>, int>& delta, vector<int> Q)
{       
   //la stergerea unui nod, tin cont de fiecare tranzitie de intrare si de iesire

    //pentru a face acest lucru, creez doi vectori:
    vector<int> iesire; // unul pentru tranzitiile de intrare
    vector<int> intrare;// celalalt pentru iesire

    for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end(); it++)
    {
        if (it->first.first == nod && it->second != nod)
            iesire.push_back(it->second);

        if (it->first.first != nod && it->second == nod)
            intrare.push_back(it->first.first);
    }

    for (int i : intrare)       //pentru fiecare i din vectorul de intrare
        for (int j : iesire)   //pentru fiecare j din vectorul de iesire
            if (drum(i, j, delta) != "") //daca exista un drum diferit de cel by default cu λ, il formez:
            {
                string nouDrum = drum(i, j, delta);
                nouDrum = nouDrum + " + (" + drum(i, nod, delta) + ") ($+" + drum(nod, nod, delta) + ")* (" + drum(nod, j, delta) + ")";
               //reunesc la noul drum cu '+': drumul i-nod concatenat prin ' ' cu  drumul: (  λ + bucla nod-nod (adica '*')  concatenat cu drumul nod-j )

                for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end();)
                    if (it->first.first == i && it->second == j)
                    {
                        delta.erase(it++); //sterg tranzitia si parcurg mai departe
                    }
                    else it++;   //sau doar parcurg mai departe

                delta[{i, nouDrum}] = j;  //reactualizez tranzitia noului drum 

                Sigma.push_back(nouDrum); //si vectorul de litere 
            }


            else
            {  //altfel pastrez in noul drum concatenarea drumurilor i-nod, bucla nod-nod si nod-j
                string nouDrum = "(" + drum(i, nod, delta) + ") ($+" + drum(nod, nod, delta) + ")* (" + drum(nod, j, delta) + ")";
                delta[{i, nouDrum}] = j;   //actualizez tranzitia
                Sigma.push_back(nouDrum);  //si vectorul de litere
            }

    for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end();)
        if (it->first.first == nod || it->second == nod)
        {
            delta.erase(it++); //sterg si in acest caz tranzitia si parcurg mai departe
        }
        else it++;  // sau doar parcurg mai departe 
}

void etichetare(int nod1, int nod2, map<pair<int, string>, int>& delta)
{
    int nr = 1;
    string etich = "";  //creez noua eticheta de pe tranzitia de la nod1 la nod2
    for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end(); it++)
        if (it->first.first == nod1 && it->second == nod2)
        {
            if (nr == 1)
            {
                etich = it->first.second;   //actualizez eticheta daca sunt prima data aici cu variabila it din parcurgerea pe tranzitii
                nr++;
            }
            else
            {
                etich = etich + "+" + it->first.second;  //creez reuniunea tranzitiilor
                nr++;
            }
        }

    if (nr != 2)
    {
        for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end();)
            if (it->first.first == nod1 && it->second == nod2)
            {
                delta.erase(it++); //sterg tranzitia si trec mai departe
            }
            else it++;

        delta[{nod1, etich}] = nod2; //reabilitez tranzitia de la nod1 la nod2 prin eticheta creata
        delta[{nod1, etich}] = nod2;
    }

}

int main()
{
    /// Datele pentru DFA->REGEX

    vector <int> Q;         // multimea starilor initiale
    vector <string> Sigma;  // multimea neterminalelor
    vector <int> F;         // multmea starilor finale
    int  q0;               // starea intiala
    map <pair <int, string>, int> delta; // functia de tranzitie mapata din < pereche (nod-litera), urmatorul nod>

    citireDate(Q, Sigma, F, q0, delta);  //citesc datele de intrare

    initFinal(q0, Sigma, Q, delta, F);  //se modifica acesti parametri corespunzator

    int cardF = 0;
    for (int i : F)
        if (cardF <= i)
            cardF = i;

    for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end(); it++)
        if (it->first.first != it->second)
            etichetare(it->first.first, it->second, delta); //parcurg fiecare eticheta si o formez

    for (int nod : Q)  //pentru fiecare nod in multimea starilor
    {
        string eticheta = "";  //creez noua eticheta
        int nr = 1;
        for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end(); it++)
            if (it->first.first == nod && it->second == nod)
            {
                if (nr == 1)
                {
                    eticheta = it->first.second;  //daca sunt in cazul cand este prima intrare
                    nr++;
                }
                else
                {
                    eticheta = eticheta + "+" + it->first.second;  //eticheta noua se va forma prin reuniunea cu eticheta curenta si tranzitia respectiva
                    nr++;
                }
            }

        if (nr >= 2)
        {
            for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end();)
                if (it->first.first == nod && it->second == nod)
                {
                    delta.erase(it++);   //dupa aceea sterg tranzitia si parcurg mai departe
                }
                else it++;   //altfel doar parcurg mai departe

            delta[{nod, eticheta}] = nod;
        }
    }

    int inc, fin;    //tin evidenta daca este formata o stare initiala si finala cu variabilele de tip bool
    if (nouInit && nouFinal)
    {
        inc = 1, fin = cardF;
    }

    if (!nouInit && nouFinal)
    {
        inc = 2, fin = cardF;
    }

    if (nouInit && !nouFinal)
    {
        inc = 1, fin = cardF;
    }

    if (!nouInit && !nouFinal)
    {
        inc = 2, fin = cardF;
    }

    for (int i = inc; i < fin; ++i)
        sterge(i, Sigma, delta, Q);  //sterg tranzitia respectiva

    string result;    //incarc in rezultat expresia obtinuta
    for (map<pair<int, string>, int>::iterator it = delta.begin(); it != delta.end(); it++)
    {
        cout << it->first.second;
        result = it->first.second;
        break;
    }


    // afisez aici Varianta scurta cu mai putine paranteze

    cout << "\n\nREGEX simplificat pentru DFA:\n";
    for (int i = 0; i < result.length(); ++i)
    {
        if (result[i] == '(' && result[i + 1] == '$' && result[i + 2] == ')' && result[i + 3] != '+')
            i += 3;

        else if (result[i] == '(' && result[i + 2] == ')' && result[i + 3] != '*')
        {
            cout << result[i + 1];
            i += 2;
        }

        else if (result[i] == '(' && result[i + 1] == ')' && result[i + 2] == '*')
            i += 3;

        else if (result[i] == '(' && result[i + 1] == '$' && result[i + 2] == '+' && result[i + 3] == ')' && result[i + 4] == '*')
            i += 4;

        else cout << result[i];
    }
    cout << ")\n";



    //afisez de asemenea aceasta varianta cu mai putine paranteze si intr-un fisier de iesire
    ofstream fcout("fa.out");
    fcout << "\nREGEX simplificat pentru DFA:\n";
    for (int i = 0; i < result.length(); ++i)
    {
        if (result[i] == '(' && result[i + 1] == '$' && result[i + 2] == ')' && result[i + 3] != '+')
            i += 3;

        else if (result[i] == '(' && result[i + 2] == ')' && result[i + 3] != '*')
        {
            fcout << result[i + 1];
            i += 2;
        }

        else if (result[i] == '(' && result[i + 1] == ')' && result[i + 2] == '*')
            i += 3;

        else if (result[i] == '(' && result[i + 1] == '$' && result[i + 2] == '+' && result[i + 3] == ')' && result[i + 4] == '*')
            i += 4;

        else fcout << result[i];
    }
    fcout << ")\n";
    fcout.close();


    return 0;
}


/* 
Varianta initiala a expresiei:
(((a) ($+)* (b)) ($+(b) ($+)* (b))* (a)) ($+((a) ($+)* (b)) ($+(b) ($+)* (b))* (a))* ($)


Varianta mai scurta a expresiei:
((a  b) ($+b  b)* a) ($+(a  b) ($+b  b)* a)* )

*/