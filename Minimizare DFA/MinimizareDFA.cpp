/*Bibliografie: Codul este inspirat din cartea "Fast brief practical DFA minimization"- Antti Valmari (2011) */
#include <iostream>
#include <algorithm>


 ///Partiție Vizitata
// Aceste două structuri sunt impartite de ambele partiții, pentru a fi mai eficienta dpdv al memoriei
int* marked; // Marked[s] este numărul de elemente marcate din setul s
int* touched; //  (i.e. element marcat) sau vizitat pentru set-uri
int touchedCount = 0; // variabila temporara

struct Partition
{
	int setCount; // numarul de set-uri
	int* elements;
	int* location;
	int* setOf;
	int* first;
	int* past;

	void init(int n) ///incep initializarea elementelor
	{
		setCount = bool(n);
		elements = new int[n];
		location = new int[n];
		setOf = new int[n];
		first = new int[n];
		past = new int[n];
		for (int i = 0; i < n; ++i)
		{
			elements[i] = location[i] = i;
			setOf[i] = 0;
		}
		if (setCount)
		{
			first[0] = 0;
			past[0] = n;
		}
	}

	void mark(int element) ///incep procesul de "vizitare"
	{
		// schimb elementul cu primul nevizitat
		int set = setOf[element];
		int i = location[element];
		int j = first[set] + marked[set];
		//  ma asigur de  faptul că nu voi apela mark[s] pe un element care este deja marcat
		elements[i] = elements[j];
		location[elements[i]] = i;
		elements[j] = element;
		location[element] = j;
		// incrementez numarul de elemente marcate, adica marked si adaug la elementele touched in caz ca este necesar
		if (!marked[set]++)
		{
			touched[touchedCount++] = set;
		}
	}

	void split()
	{
		while (touchedCount)
		{
			int set = touched[--touchedCount];
			int firstUnmarked = first[set] + marked[set];
			if (firstUnmarked == past[set])
			{
				marked[set] = 0;
				continue;
			}

			// din jumătatea mai mică voi crea un nou set
			// dacă are aceeași dimensiune, atunci voi face un set nou din elemntele nevizitate, adica cele nemarcate
			if (marked[set] <= past[set] - firstUnmarked)
			{
				first[setCount] = first[set];
				past[setCount] = first[set] = firstUnmarked;
			}
			else
			{
				past[setCount] = past[set];
				first[setCount] = past[set] = firstUnmarked;
			}
			for (int i = first[setCount]; i < past[setCount]; ++i)
			{
				setOf[elements[i]] = setCount;
			}
			marked[setCount++] = 0;
			marked[set] = 0;
		}
	}
};

Partition blocks; // ma refer la starile blocate
Partition cords; // ma refer la tranzitiile legate
int stateCount; // numarul de stari
int transitionCount; // numarul de tranzitii
int finalStatesCount; // numarul de stari finale
int initialState; // starea initiala
int* transitionTail; // starea de unde pleaca tranzitia
int* transitionLabel; // aici ma refer la litera de pe tranzitie (in exemplu litera a este reprezentata de numarul 1, litera b este 2 etc)
int* transitionHead; // starea in care va fi dusa tranzatia

bool cmp(int i, int j) ///compar eticheta de pe tranzitii
{
	return transitionLabel[i] < transitionLabel[j];
}


/// Tranzitiile Adiacente
int* adjacent;
int* offset;
void make_adjacent(int states[])
{
	for (int state = 0; state <= stateCount; ++state)
		offset[state] = 0;

	for (int transition = 0; transition < transitionCount; ++transition)
		++offset[states[transition]];

	for (int state = 0; state < stateCount; ++state)
		offset[state + 1] += offset[state];

	for (int transition = transitionCount; transition--; )
		adjacent[--offset[states[transition]]] = transition;
}

/// Eliminarea partilor nesemnificative
int reachableCount = 0; // numarul de stari atinse

inline void reach(int state)
{
	int i = blocks.location[state];
	if (i >= reachableCount)
	{
		blocks.elements[i] = blocks.elements[reachableCount];
		blocks.location[blocks.elements[i]] = i;
		blocks.elements[reachableCount] = state;
		blocks.location[state] = reachableCount++;
	}
}

void rem_unreachable(int tail[], int head[])
{
	make_adjacent(tail);
	///parcurg automatul DFA pentru a marca starile accesibile
	for (int i = 0; i < reachableCount; ++i)
	{
		for (int j = offset[blocks.elements[i]]; j < offset[blocks.elements[i] + 1]; ++j)
			reach(head[adjacent[j]]);
	}
	/// elimin starile si tranzitiile nelegate

	int j = 0;
	for (int t = 0; t < transitionCount; ++t)
	{
		if (blocks.location[tail[t]] < reachableCount)
		{
			head[j] = head[t];
			transitionLabel[j] = transitionLabel[t];
			tail[j] = tail[t];
			++j;
		}
	}
	transitionCount = j;
	blocks.past[0] = reachableCount;
	reachableCount = 0;
}




int main()
{
	/// Citesc nr de stari, nr de tranzitii, starea initiala si cate stari finale sunt
	std::cin >> stateCount >> transitionCount >> initialState >> finalStatesCount;

	transitionTail = new int[transitionCount];
	transitionLabel = new int[transitionCount];
	transitionHead = new int[transitionCount];
	blocks.init(stateCount);
	adjacent = new int[transitionCount];
	offset = new int[stateCount + 1];

	/// Citesc tranzitiile
	for (int t = 0; t < transitionCount; ++t)
	{
		std::cin >> transitionTail[t] >> transitionLabel[t] >> transitionHead[t];
		//starea de unde pleaca tranzitia, litera reprezentata printr-un numar de pe tranzitie si starea in care se duce tranzitia
	}


	/// Elimin stările la care nu se poate ajunge din starea inițială și cele din care nu se pot ajunge de la stările finale
	reach(initialState);
	rem_unreachable(transitionTail, transitionHead);
	for (int i = 0; i < finalStatesCount; ++i)
	{
		int q;
		std::cin >> q;
		if (blocks.location[q] < blocks.past[0])
		{
			reach(q);
		}
	}
	finalStatesCount = reachableCount;
	rem_unreachable(transitionHead, transitionTail);


	///Creez prima partitie
	touched = new int[transitionCount + 1];
	marked = new int[transitionCount + 1];
	marked[0] = finalStatesCount;
	if (finalStatesCount)
	{
		touched[touchedCount++] = 0;
		blocks.split();
	}

	///Creez partitia de tranzitie
	cords.init(transitionCount);
	if (transitionCount)
	{
		std::sort(cords.elements, cords.elements + transitionCount, cmp);
		cords.setCount = marked[0] = 0;

		// Trebuie neaparat sa aplic cords.first [0] == 0 în acest moment pentru ca primul set să fie corect
		int currentLabel = transitionLabel[cords.elements[0]];
		for (int i = 0; i < transitionCount; ++i)
		{
			int t = cords.elements[i];
			if (transitionLabel[t] != currentLabel)
			{
				currentLabel = transitionLabel[t];
				cords.past[cords.setCount++] = i;
				cords.first[cords.setCount] = i;
				marked[cords.setCount] = 0;
			}
			cords.setOf[t] = cords.setCount;
			cords.location[t] = i;
		}
		cords.past[cords.setCount++] = transitionCount;
	}


	/// Evidentiez starile blocate si tranzitiile legate
	make_adjacent(transitionHead);
	int b = 1, c = 0, i, j;
	while (c < cords.setCount)
	{
		for (i = cords.first[c]; i < cords.past[c]; ++i)
		{
			blocks.mark(transitionTail[cords.elements[i]]);
		}
		blocks.split(); ++c;
		while (b < blocks.setCount)
		{
			for (i = blocks.first[b]; i < blocks.past[b]; ++i)
			{
				for (j = offset[blocks.elements[i]]; j < offset[blocks.elements[i] + 1]; ++j)
				{
					cords.mark(adjacent[j]);
				}
			}
			cords.split(); ++b;
		}
	}


	/// Numar cate tranzitii si cate stari finale sunt in automatul final
	int mo = 0, fo = 0;
	for (int t = 0; t < transitionCount; ++t)
	{
		if (blocks.location[transitionTail[t]] == blocks.first[blocks.setOf[transitionTail[t]]])
		{
			++mo;
		}
	}
	for (int b = 0; b < blocks.setCount; ++b)
	{
		if (blocks.first[b] < finalStatesCount)
		{
			++fo;
		}
	}

	///Afisez rezultatul obtinut
	std::cout << blocks.setCount << ' ' << mo << ' ' << blocks.setOf[initialState] << ' ' << fo << '\n';
	for (int t = 0; t < transitionCount; ++t)
	{
		if (blocks.location[transitionTail[t]] == blocks.first[blocks.setOf[transitionTail[t]]])
		{
			std::cout << blocks.setOf[transitionTail[t]] << ' ' << transitionLabel[t] << ' ' << blocks.setOf[transitionHead[t]] << '\n';
		}
	}
	for (int b = 0; b < blocks.setCount; ++b)
	{
		if (blocks.first[b] < finalStatesCount)
		{
			std::cout << b << '\n';
		}
	}
}


/* Ex 1:
- 7 stari totale, 14 tranzitii, starea initiala este 0, o stare finala adica 1
 - in urmatoatele randuri sunt reprezentate cele 14 tranzitii astfel:
     de exemplu pentru a III-a si a IV-a tranzitie 1 1 3 si 1 2 2, avem:
din starea 1 pleaca o tranzitie in starea 3 cu litera a, aflata in  mijloc si fiind numarul 1
din starea 1 pleaca o tranzitie in starea 2 cu litera b, aflata in  mijloc si fiind numarul 2
  - la final avem o stare finala care este starea cu numarul 6

- Acest exemplu este prezentat ca exercitiu si in materialul de seminar.
7 14 0 1
0 1 1
0 2 3
1 1 3
1 2 2
2 2 2
2 1 3
3 1 6
3 2 5
4 1 6
4 2 5
5 1 6
5 2 2
6 1 4
6 2 5
6
*/


/* Ex 2:  - 5 stari, 6 tranzitii, starea initiala este 0, 2 stari finale
		  - in urmatoatele randuri sunt reprezentate tranzitiile astfel:
		      de exemplu pentru 1 2 3 si 2 2 3, avem
			  din starea 1 pleaca o tranzitie in starea 3 cu litera a, fiind numarul 1
			  din starea 2 pleaca o tranzitie in starea 3 cu litera b, fiind numarul 2
		  - la final 3 si 4 sunt cele doua stari finale
5 6 0 2
0 0 1
0 1 2
1 2 3
2 2 3
1 3 4
2 3 4
3
4
*/
