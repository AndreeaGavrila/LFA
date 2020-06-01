# Bibliografie: Codul este inspirat din cartea "Elements of the Theory of Computation" - Lewis H.R., Papadimitriou C.H.(1998) 
#-------------------------------------------------------------------------------------------------------------------------------------

#Transformare - o gramatica independenta de context (CFG) intr-o gramatica in forma normala Chomsky (CNF).
#-------------------------------------------------------------------------------------------------------------------------------------


# functia (convert_to_String) - primeste ca input o gramatica independenta de context (CFG), gramatica este configurata direct in codul sursa.
#                           - ne asumam faptul ca a fost introdusa corect o gramatica independenta de context (CFG).
def convert_to_String():                  

        #EXEMPLUL 1:
        CFG = "S -> SAS | aB | a"
        CFG+= "\nA -> B | S"
        CFG+= "\nB -> b | e"

        #EXEMPLUL 2:
        #CFG = "S -> A | B"
        #CFG+="\nA -> aB | bS | b"
        #CFG+="\nB -> AB | Ba"
        #CFG+="\nC -> AS | b"


        #EXEMPLUL 3:
        #CFG = "S -> Ad | c"
        #CFG+="\nA -> S | dc | e"
        
       
        #EXEMPLUL 4:
        #CFG = "S -> ASB"
        #CFG+="\nA -> aAS | a | e"
        #CFG+="\nB -> SbS | A | bb"


        #EXEMPLUL 5:
        #CFG = "S -> SAB | BS | b"
        #CFG+= "\nA -> abA | Bbb | e"
        #CFG+= "\nB -> Sa | a"


        # elimin din gramatica citita toate spatiile si eventualele returns si tab-uri introduse 
        CFG = CFG.replace(" ", "")
        CFG = CFG.replace("\r", "")
        CFG = CFG.replace("\t", "")
        
        # returnez in final o gramatica CFG de tip string, cu toate spatiile in plus eliminate
        return CFG

   

# functia (convert_to_CNF) - preia gramatica CFG din functia anterioara si o transforma in CNF

def convert_to_CNF(CFG):
        print("------- Transformare  CFG in CNF: -------\n\n")
        

        # PAS 1: se creaza o noua stare initiala (de exemplu S0)
        start = CFG[0]   # selectez starea initiala din CFG
        prima_linie = start + "0->" + start # si la aceasta concatenez '0'
        CNF = prima_linie + "\n" + CFG # pentru a forma noua stare initiala
        pas1 = CNF

        print("PAS 1: \n\n" + pas1 + "\n\n")


        # PAS 2: elimin toate λ-producțiile

        gramatica_mea = CNF.split("\n")  # impart gramatica CNF in string-uri separate pentru fiecare linie
        count = sum('e' in s for s in gramatica_mea) #numar λ-producțiile, λ este reprezentat prin 'e'
        mesaj = "PAS 2: \n"

        while count > 0:

            for loop in range(count): # pentru fiecare λ-producții 
                for k in range(len(gramatica_mea)):  # pentru fiecare rand
                    for i in range(len(gramatica_mea[k])):   # pentru fiecare productie din fiecare rand
                        if gramatica_mea[k][i] == "e": # verific daca neterminalul are o  λ-producție
                            
                            neterminal = gramatica_mea[k][0]   # gasesc neterminalul respectiv
                            #este cazul daca neterminalul care are o λ–producţie NU are şi alte producţii
                           
                            for j in range(len(gramatica_mea)):  # caut printre productii sa vad
                               # daca neterminalul care are o λ–productie are si alte productii, atunci
                                                          
                                
                                loc = gramatica_mea[j].find(neterminal)
                                
                                if loc == -1:
                                    continue
                                
                                if loc == 0:
                                    loc = gramatica_mea[j].find(neterminal, 1)
                               
# Stim ca o gramatică este scrisa în forma normala Chomsky daca are doar productii care au in membrul stang un neterminal, 
# iar în membrul drept fie un terminal, fie două neterminale.
#𝐴->𝑎
#𝐴->𝐵C , unde 𝐴,𝐵,𝐶 ∈𝑁 = {A, B, C, …} mulţimea de simboluri neterminale, 𝑎∈𝑇 = {a, b, c, ...} mulţimea de simboluri terminale


                        # Cazul 1: Neterminalul nu este in combinatii cu alte litere (de exemplu: "A" )
                                if (gramatica_mea[j][loc - 1] == ">" or gramatica_mea[j][loc - 1] == "|") and (
                                       gramatica_mea[j][loc + 1] == "|" or gramatica_mea[j][loc + 1] == "\n"):
                                    
                                    gramatica_mea[j] = gramatica_mea[j] + "|e" 
                                    # fiind doar acest neterminal de lungime 1, se va înlocui cu λ, adica 'e'
                                    #  (dar şi această producţie va fi ulterior eliminată)

                                    # elimin  λ-producția originala
                                    if "|e" in gramatica_mea[k]:   # daca mai exista o productie inaintea λ-productiei = 'e' 
                                        gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin
                                      
                                    elif ">e" in gramatica_mea[k]:     # sau daca λ-productia = 'e' este singura din neterminal
                                        gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "") # este eliminata de asemenea

                                   
                #----------------------------------------------------------------------------------------------------------------   

                         # Cazul 2: Neterminalul este aflat in combinatii si cu alte litere sau alte neterminale; Avem urmatoarele cazuri:

                                 # a) daca neterminalul este urmat de alte 2 simboluri terminale (litere) / neterminale
                                 # (de exemblu: "AXX" sau "Axx")
                                if loc < len(gramatica_mea[j]) - 2:
                                                                 
                                    if (gramatica_mea[j][loc + 1] != "|" and gramatica_mea[j][loc + 1] != "\n") and (
                                            gramatica_mea[j][loc + 1] != neterminal and gramatica_mea[j][loc + 2] != "|" and (
                                            gramatica_mea[j][loc + 2] != "\n")) and (
                                            gramatica_mea[j][loc + 2] != neterminal):

                                        gramatica_mea[j] = gramatica_mea[j] + "|" + gramatica_mea[j][loc + 1:loc + 3]

                                        # elimin λ-producția originala 
                                        if gramatica_mea[k][i - 1] == "|": # daca mai exista o productie inaintea λ-productiei = 'e' 
                                            gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin

                                        elif gramatica_mea[k][i - 1] == ">":   # sau daca λ-productia = 'e' este singura din neterminal
                                            gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea


                                    # b) daca sunt 2 neterminale, iar intre acestea apare un alt simbol terminal (litera) / neterminal
                                    # (de exemplu: "AXA" or "AxA")
                                    if (gramatica_mea[j][loc + 1] != "|" and gramatica_mea[j][loc + 1] != "\n") and (
                                            gramatica_mea[j][loc + 1] != neterminal) and gramatica_mea[j][loc + 2] != "|" and (
                                            gramatica_mea[j][loc + 2] != "\n") and (
                                            gramatica_mea[j][loc + 2] == neterminal):

                                        gramatica_mea[j] = gramatica_mea[j] + "|" + \
                                                  gramatica_mea[j][loc + 1:loc + 3] + "|" + \
                                                  gramatica_mea[j][loc:loc + 2] + "|" + \
                                                  gramatica_mea[j][loc + 1]

                                        # elimin λ-producția originala 
                                        if gramatica_mea[k][i - 1] == "|":    # daca mai exista o productie inaintea λ-productiei = 'e' 
                                            gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin
                                        elif gramatica_mea[k][i - 1] == ">":   # sau daca λ-productia = 'e' este singura din neterminal
                                            gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea


                                    ## c) daca 2 neterminale sunt urmate de inca un alt simbol terminal (litera) / neterminal
                                    # (de exemplu: "AAX" or "AAx")
                                    if gramatica_mea[j][loc + 1] == neterminal and gramatica_mea[j][loc + 2] != "|" and (
                                            gramatica_mea[j][loc + 2] != "\n") and gramatica_mea[j][loc + 2] != "" and(
                                            gramatica_mea[j][loc + 2] != neterminal) and gramatica_mea[j][loc] == neterminal:
                                        gramatica_mea[j] = gramatica_mea[j] + "|" + gramatica_mea[j][loc + 2]

                                        # elimin λ-producția originala 
                                        if gramatica_mea[k][i - 1] == "|":    # daca mai exista o productie inaintea λ-productiei = 'e' 
                                            gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin
                                        elif gramatica_mea[k][i - 1] == ">":   # sau daca λ-productia = 'e' este singura din neterminal
                                            gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea

                                #continui cu celalalt caz:
                                if loc <= len(gramatica_mea[j]) - 2:

                                 # d) daca neterminalul este precedat si urmat de exact un alt simbol terminal (litera) / neterminal
                                 # (de exemplu: "XAX" or "xAx")
                                   
                                    if (gramatica_mea[j][loc - 1] != "|" and gramatica_mea[j][loc - 1] != ">") and (
                                            gramatica_mea[j][loc - 1] != neterminal) and (
                                            gramatica_mea[j][loc + 1] != "|" and gramatica_mea[j][loc + 1] != "") and (
                                            gramatica_mea[j][loc + 1] != neterminal) and gramatica_mea[j][loc] == neterminal:

                                        gramatica_mea[j] = gramatica_mea[j] + "|" + gramatica_mea[j][loc - 1] + gramatica_mea[j][loc + 1]

                                        # elimin λ-producția originala 
                                        if gramatica_mea[k][i - 1] == "|":    # daca mai exista o productie inaintea λ-productiei = 'e' 
                                            gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin
                                        elif gramatica_mea[k][i - 1] == ">":   # sau daca λ-productia = 'e' este singura din neterminal
                                            gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea


                                    # e) daca neterminalul este precedat doar de exact un alt simbol terminal (litera) / neterminal
                                    # (de exemplu: "XA" or "xA") 
                                    if (gramatica_mea[j][loc - 1] != "|" and gramatica_mea[j][loc - 1] != ">") and (
                                            gramatica_mea[j][loc + 1] == "|" or gramatica_mea[j][loc + 1] == "") and (
                                            gramatica_mea[j][loc - 2] == "|" or gramatica_mea[j][loc - 2] == ">"):
                                        gramatica_mea[j] = gramatica_mea[j] + "|" + gramatica_mea[j][loc - 1]

                                       # elimin λ-producția originala 
                                        if gramatica_mea[k][i - 1] == "|":    # daca mai exista o productie inaintea λ-productiei = 'e' 
                                            gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin
                                        elif gramatica_mea[k][i - 1] == ">":   # sau daca λ-productia = 'e' este singura din neterminal
                                            gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea


                                    # f) daca neterminalul este urmat doar de exact un alt simbol terminal (litera) / neterminal
                                    # (de exemplu: "AX" or "Ax") 
                                    if (gramatica_mea[j][loc + 1] != "|" and gramatica_mea[j][loc + 1] != "") and (
                                            gramatica_mea[j][loc - 1] == "|" or gramatica_mea[j][loc - 1] == ">") and (
                                            gramatica_mea[j][loc + 2] == "|" or gramatica_mea[j][loc + 2] == ""):
                                        gramatica_mea[j] = gramatica_mea[j] + "|" + gramatica_mea[j][loc + 1]

                                         # elimin λ-producția originala 
                                        if gramatica_mea[k][i - 1] == "|":    # daca mai exista o productie inaintea λ-productiei = 'e' 
                                            gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin
                                        elif gramatica_mea[k][i - 1] == ">":   # sau daca λ-productia = 'e' este singura din neterminal
                                            gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea


                                    # g) daca 2 neterminale sunt precedate doar de exact un alt simbol terminal (litera) / neterminal
                                    # (de exemplu: "XAA" or "xAA") 
                                    if (gramatica_mea[j][loc - 1] != neterminal and gramatica_mea[j][loc - 1] != "|") and (
                                            gramatica_mea[j][loc - 1] != ">" and gramatica_mea[j][loc + 1] == neterminal):
                                        gramatica_mea[j] = gramatica_mea[j] + "|" + gramatica_mea[j][loc - 1]

                                       # elimin λ-producția originala 
                                        if gramatica_mea[k][i - 1] == "|":    # daca mai exista o productie inaintea λ-productiei = 'e' 
                                            gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin
                                        elif gramatica_mea[k][i - 1] == ">":   # sau daca λ-productia = 'e' este singura din neterminal
                                            gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea


                                # h) daca neterminalul este precedat de alte doua simboluri terminale (litere) / neterminale
                                # (de exemplu: "XXA" or "xxA") 
                                if (gramatica_mea[j][loc - 1] != "|" and gramatica_mea[j][loc - 1] != ">") and (
                                        gramatica_mea[j][loc - 2] != "|" and gramatica_mea[j][loc - 2] != ">") and (
                                        gramatica_mea[j][loc - 2] != neterminal):
                                    gramatica_mea[j] = gramatica_mea[j] + "|" + gramatica_mea[j][loc - 2:loc]

                                       # elimin λ-producția originala 
                                    if gramatica_mea[k][i - 1] == "|":    # daca mai exista o productie inaintea λ-productiei = 'e' 
                                        gramatica_mea[k] = gramatica_mea[k].replace("|e", "")  # o elimin
                                    elif gramatica_mea[k][i - 1] == ">":  # sau daca λ-productia = 'e' este singura din neterminal
                                        gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea


                                    # i) daca neterminalul este precedat de exact un alt simbol terminal (litera) / neterminal
                                    # (de exemplu: "XA" or "xA") 
                                    if (gramatica_mea[j][loc - 1] != "|" and gramatica_mea[j][loc - 1] != ">") and (
                                            gramatica_mea[j][loc - 2] == "|" or gramatica_mea[j][loc - 2] == ">"):
                                        gramatica_mea[j] = gramatica_mea[j] + "|" + gramatica_mea[j][loc - 1]

                                       # elimin λ-producția originala 
                                        if gramatica_mea[k][i - 1] == "|":     # daca mai exista o productie inaintea λ-productiei = 'e' 
                                            gramatica_mea[k] = gramatica_mea[k].replace("|e", "") # o elimin
                                        elif gramatica_mea[k][i - 1] == ">":    # sau daca λ-productia = 'e' este singura din neterminal
                                            gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k], "")  # este eliminata de asemenea


                # verific daca lista nu a ramas cu o valoare vida
                if gramatica_mea[len(gramatica_mea) - 1] == "":
                    gramatica_mea.remove(gramatica_mea[len(gramatica_mea) - 1])

                # transform inapoi la tipul string
                string = ""
                for i in range(len(gramatica_mea)):
                    string = string + gramatica_mea[i] + "\n"
                    CNF = CNF.replace(CNF[0:], string)
                count = sum('e' in s for s in gramatica_mea)

                pas2 = CNF

                # concatenez ce am obtinut pana acum si afisez pe ecran PASUL 2:
                print(mesaj + "\n" + pas2 + "\n")
                mesaj = ""


        #----------------------------------------------------------------------------------------------------------

        # PAS 3: elimin productia S0->S (prin copierea lui S in S0) 
        #și elimin toate neterminalele singure din conținutul unei productii

        nou = start + "0" + CNF[7:CNF.find("\n", 6)]   #prima linie a fost initializata cu exact 6 caractere, deci linia 2 va incepe cu indexul 7

        CNF = CNF.replace(prima_linie, nou)   #inlocuiesc productia de pe prima linie (S0->S) cu productia originala a lui S, 
                                            #in scopul de a elimina S0->S prin copierea lui S pe prima linie

        # elimin orice productie care contine un singur neterminal
        # deoarece vrem să nu mai avem în gramatică, producţii care au ca membru drept un neterminal.
        gramatica_mea = CNF.split("\n")  # impart gramatica CNF in siruri separate pentru fiecare linie
        copy = ""
        
        for k in range(len(gramatica_mea)):
            for i in range(len(gramatica_mea[k])-1):
                if i != 0:
                    
                    # pentru neterminalele din prima parte a productiei
                    if gramatica_mea[k][i-1] == ">" and gramatica_mea[k][i+1] == "|" and gramatica_mea[k][i].isupper():
                        
                        neterminal = gramatica_mea[k][i]
                        
                        for j in range(len(gramatica_mea)-1):
                            if gramatica_mea[j][0] == neterminal:
                                copy = gramatica_mea[j][gramatica_mea[j].find(">")+1:]

                        gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i], copy) # actualizez in urma inlocuirii respective

                    # pentru neterminalele aflate in mijlocul productiei
                    elif gramatica_mea[k][i - 1] == "|" and gramatica_mea[k][i + 1] == "|" and gramatica_mea[k][i].isupper():
                        
                        neterminal = gramatica_mea[k][i]

                        for j in range(len(gramatica_mea) - 1):
                            if gramatica_mea[j][0] == neterminal:
                                copy = gramatica_mea[j][gramatica_mea[j].find(">") + 1:]

                        gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i], copy) # actualizez in urma inlocuirii respective

                    # pentru neterminalele de la sfarsitul productiei
                    elif gramatica_mea[k][i-1] == "|" and gramatica_mea[k][i+1] == "" and gramatica_mea[k][i].isupper():

                        neterminal = gramatica_mea[k][i]

                        for j in range(len(gramatica_mea) - 1):
                            if gramatica_mea[j][0] == neterminal:
                                copy = gramatica_mea[j][gramatica_mea[j].find(">") + 1:]

                        gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i], copy) # actualizez in urma inlocuirii respective

                    # si pentru celelalte neterminale de la sfarsitul productiei
                    elif gramatica_mea[k][-2] == "|" and gramatica_mea[k][-1].isupper():

                        neterminal = gramatica_mea[k][-1]

                        for j in range(len(gramatica_mea)-1):
                            if gramatica_mea[j][0] == neterminal:
                                copy = gramatica_mea[j][gramatica_mea[j].find(">") + 1:]

                        gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][-1], copy) # actualizez in urma inlocuirii respective


 
        # verific daca lista nu a ramas cu o valoare vida
        if gramatica_mea[len(gramatica_mea)-1] == "":
            gramatica_mea.remove(gramatica_mea[len(gramatica_mea)-1])

        # transform inapoi la tipul string
        string = ""
        for i in range(len(gramatica_mea)):
            string = string + gramatica_mea[i] + "\n"
            CNF = CNF.replace(CNF[0:], string)

        pas3 = CNF

        # concatenez ce am obtinut pana acum si afisez pe ecran PASUL 3:
        print("PAS 3: \n\n" + pas3 + "\n")



        #----------------------------------------------------------------------------------------------------------

        # PAS 4: elimin productiile in care sunt 3 sau mai multe litere impreuna cu netrminale sub urmatoarele forme:
        alfabet = "a b c d e f g h i j k l m n o p q r s t u v w x y z"
        lit_mici = alfabet.split(" ")
        lit_mari = alfabet.upper().split(" ")
        neterminalcount = 1
        sub = ""

        for k in range(len(gramatica_mea)):
            for i in range(len(gramatica_mea[k])-2):
                
                # a) sunt de forma "XXX" sau "xxx"
                if (gramatica_mea[k][i] in lit_mari and gramatica_mea[k][i+1] in lit_mari and gramatica_mea[k][i+2] in lit_mari) \
                        or (gramatica_mea[k][i] in lit_mici and gramatica_mea[k][i+1] in lit_mici and gramatica_mea[k][i+2] in lit_mici):

                    if gramatica_mea[k][i+1:i+3] != sub:

                        nou = lit_mari[0] + str(neterminalcount)
                        neterminalcount += 1
                        sub = gramatica_mea[k][i+1:i+3]
                        gramatica_mea.append(nou + "->" + gramatica_mea[k][i+1:i+3])  # formez o noua productie

                    gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i+1:i+3], nou) # actualizez in urma inlocuirii cu noua productie formata


                # b) sunt de forma "xXX" sau "Xxx"
                if (gramatica_mea[k][i] in lit_mici and gramatica_mea[k][i + 1] in lit_mari and gramatica_mea[k][i + 2] in lit_mari) \
                        or (gramatica_mea[k][i] in lit_mari and gramatica_mea[k][i + 1] in lit_mici and gramatica_mea[k][i + 2] in lit_mici):

                    if gramatica_mea[k][i+1:i + 3] != sub:

                        nou = lit_mari[1] + str(neterminalcount)
                        neterminalcount += 1
                        sub = gramatica_mea[k][i + 1:i + 3]
                        gramatica_mea.append(nou + "->" + gramatica_mea[k][i + 1:i + 3])   # formez o noua productie

                    gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i + 1:i + 3], nou) # actualizez in urma inlocuirii cu noua productie formata


                # c) sunt de forma "xxX" sau "XXx"
                if gramatica_mea[k][i] in lit_mici and gramatica_mea[k][i + 1] in lit_mici and gramatica_mea[k][i + 2] in lit_mari:

                    if gramatica_mea[k][i:i + 2] != sub:

                        nou = lit_mari[2] + str(neterminalcount)
                        neterminalcount += 1
                        sub = gramatica_mea[k][i:i + 2]
                        gramatica_mea.append(nou + "->" + gramatica_mea[k][i:i + 2])   # formez o noua productie

                    gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i:i + 2], nou) # actualizez in urma inlocuirii cu noua productie formata


                # d) sunt de forma "xXx" sau "XxX"
                if (gramatica_mea[k][i] in lit_mici and gramatica_mea[k][i + 1] in lit_mari and gramatica_mea[k][i + 2] in lit_mici) \
                        or (gramatica_mea[k][i] in lit_mari and gramatica_mea[k][i + 1] in lit_mici and gramatica_mea[k][i + 2] in lit_mari):

                    if gramatica_mea[k][i+1:i + 3] != sub:

                        nou = lit_mari[3] + str(neterminalcount)
                        neterminalcount += 1
                        sub = gramatica_mea[k][i+1:i + 3]
                        gramatica_mea.append(nou + "->" + gramatica_mea[k][i+1:i + 3])   # formez o noua productie

                    gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i+1:i + 3], nou) # actualizez in urma inlocuirii cu noua productie formata


       # transform inapoi la tipul string
        string = ""
        for i in range(len(gramatica_mea)):
            string = string + gramatica_mea[i] + "\n"
            CNF = CNF.replace(CNF[0:], string)

        pas4 = CNF

        # concatenez ce am obtinut pana acum si afisez pe ecran PASUL 4:
        print("PAS 4: \n\n" + pas4 + "\n")



        #----------------------------------------------------------------------------------------------------------

        # PAS 5: elimin productiile in care neterminalele sunt in combinatie cu litere sub uramatoarele forme:
        index = 0
        for k in range(len(gramatica_mea)):
            for i in range(len(gramatica_mea[k])-1):

                # a) sunt de forma "xX"
                if gramatica_mea[k][i] in lit_mici and gramatica_mea[k][i+1] in lit_mari:

                    if gramatica_mea[k][i] != index:   # verific daca avem deja formata productia pentru a ne ocupa de acest lucru:

                        nou = lit_mari[4] + str(neterminalcount)
                        neterminalcount += 1
                        index = gramatica_mea[k][i]
                        gramatica_mea.append(nou + "->" + gramatica_mea[k][i])   # creez o noua productie

 
                    # inlocuiesc literele mici cu numele noii productii formate
                    gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i], nou)


                # b) sunt de forma "Xx"
                elif gramatica_mea[k][i] in lit_mari and gramatica_mea[k][i+1] in lit_mici:

                    if gramatica_mea[k][i+1] != index:   # verific daca avem deja formata productia pentru a ne ocupa de acest lucru:

                        nou = lit_mari[5] + str(neterminalcount)
                        neterminalcount += 1
                        index = gramatica_mea[k][i+1]
                        gramatica_mea.append(nou + "->" + index)   # creez o noua productie

                    # inlocuiesc literele mici cu numele noii productii formate
                    gramatica_mea[k] = gramatica_mea[k].replace(gramatica_mea[k][i+1], nou)



        # transform inapoi la tipul string
        string = ""
        for i in range(len(gramatica_mea)):
            string = string + gramatica_mea[i] + "\n"
            CNF = CNF.replace(CNF[0:], string)

        pas5 = CNF

        # concatenez ce am obtinut pana acum si afisez pe ecran PASUL 5:
        print("PAS 5: \n\n" + pas5 + "\n")

        #----------------------------------------------------------------------------------------------------------

        # S-a transformat cu succes gramatica independenta de conext CFG intr-o gramatica in forma normala Chomsky CNF.
        print("\n------- S-a transformat cu succes CFG in CNF! -------")


# Programul main():

my_CFG = convert_to_String()
print("Gramatica CFG introdusa:")
print(my_CFG)
print('\n')

convert_to_CNF(my_CFG)