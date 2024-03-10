open_train_station:
    Aceasta functie aloca memorie pentru structura de date TrainStation.

close_train_station:
    Aceasta functie dezaloca memoria pentru toata gara. Mai intai sunt sterse
vagoanele, apoi locomotivele si in final toata structura.

show_existing_trains:
    Functia aceasta afiseaza intr-un fisier f o gara data ca parametru. Afisarea
se face prin parcurgerea pe rand a valorii fiecarui tren cu sau fara vagoane de pe
fiecare peron.

arrive_train:
    Aceasta functie aloca memorie pentru o locomotiva, ii atribuie puterea data ca
parametru si il pune pe peronul dorit.

leave_train:
    Functia dezaloca pe rand vagoanele trenului, iar apoi dezaloca locomotiva,
facand loc pentru o alta.

add_train_car:
    Se aloca memorie pentru un vagon, i se atribuie greutatea data ca parametru, iar
apoi este legat de ultimul vagon sau de locomotiva, dupa caz.

remove_train_cars:
    Cu ajutorul unui iterator parcurg toate vagoanele trenului de pe peronul platform.
Existand posibilitatea ca primul vagon sa trebuiasca sters, am implementat o secventa
care scoate primul vagon si care verifica daca mai exista vagoane de sters. Apoi se
parcurg restul de vagoane spre a fi sterse.

move_train_cars:
    Functia parcurge toate vagoanele de pe platform_a memorand pozitia de inceput a
secventei, respectiv pozitia de final a secventei. Mai apoi secventa este legata de
trenul de pe platform_b pe pozitia data ca parametru, urmand a fi stearsa de pe
trenul sursa.

find_express_train:
    Este parcursa toata gara, pentru a calcula diferenta maxima de greutate a fiecarui
tren. Mai apoi este returnat peronul pe care se afla trenul cu diferenta de greutate
maxima.

find_overload_train:
    Este parcursa toata gara, pentru a calcula diferenta de greutate a fiecarui tren.
Daca la un moment dat valoarea calculata este negativa, se returneaza peronul cu
trenul supraincarcat, altfel se va returna la finalul functiei -1.

find_optimal_train:
    Aceasta functie parcurge toata gara, pentru a calcula diferenta minima de greutate
a fiecarui tren. Peronul pe care se afla trenul respectiv este returnat.

find_heaviest_sequence_train:
    Functia calculeaza greutatea fiecarei secvente, cu ajutorul a 2 iteratoare. Primul
iterator parcurge toate vagoanele, iar al doilea memoreaza pozitia vagonului de
dinaintea secventei care ajuta la calculul greutatii secventei.

order_train:
    Functia sorteaza descrescator vagoanele trenului de pe peronul platform, prin
parcurgerea repetata a vagoanelor. La fiecare parcurgere se cauta vagonul de greutate
minima, ca mai apoi sa fie mutat in fata primului vagon.

fix_overload_train:
    Aceasta functie calculeaza mai intai greutatea de care e nevoie pentru a echilibra
trenul. Folosind aceasta valoare se cauta vagonul care are greutatea minima, dar care
are greutatea mai mare sau egala cu greutatea de scos.