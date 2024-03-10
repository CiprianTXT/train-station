#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"


/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
    TrainStation *station = malloc(sizeof(TrainStation));

    station->platforms_no = platforms_no;

    station->platforms = malloc(platforms_no * sizeof(Train));
    for (int i = 0; i < platforms_no; i++) {
        station->platforms[i] = NULL;
    }
    
    return station;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {

    // Daca gara nu exista sau este invalida => n-am ce sa fac
    if (station == NULL) {
        return;
    }

    // Eliberez fiecare tren de pe peron
    for (int i = 0; i < station->platforms_no; i++) {
        if (station->platforms[i] != NULL) {
            if (station->platforms[i]->train_cars != NULL) {
                TrainCar *current_car = station->platforms[i]->train_cars;
                TrainCar *next_car = current_car->next;
                while (current_car->next != NULL) {
                    free(current_car);
                    current_car = next_car;
                    next_car = next_car->next;
                }
            }
            free(station->platforms[i]);
        }
    }

    // Eliberez campul propriu-zis si toata structura
    free(station->platforms);
    free(station);

}

/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    // Daca gara nu exista sau este invalida => n-am ce sa fac
    if (station == NULL) {
        return;
    }

    for (int i = 0; i < station->platforms_no; i++) {
        fprintf(f, "%d: ", i);

        if (station->platforms[i] != NULL) {
            TrainCar *iterator = station->platforms[i]->train_cars;
            fprintf(f, "(%d)", station->platforms[i]->locomotive_power);
            while (iterator != NULL) {
                fprintf(f, "-|%d|", iterator->weight);
                iterator = iterator->next;
            }
        }

        fprintf(f, "\n");
    }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    // Gara nu exista => n-am unde sa adaug o locomotiva
    if (station == NULL) {
        return;
    }

    // Locomotiva nu se poate adauga pe un peron inexistent
    if (platform < 0 || station->platforms_no <= platform) {
        return;
    }

    // Pe peronul 'platform' exista deja o locomotiva
    if (station->platforms[platform] != NULL) {
        return;
    }

    // Peronul este liber
    Train *train = malloc(sizeof(Train));

    train->locomotive_power = locomotive_power;
    train->train_cars = NULL;

    station->platforms[platform] = train;
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    // Gara nu exista => nu are de unde sa plece trenul
    if (station == NULL) {
        return;
    }
    
    // Trenul nu poate pleca de pe un peron inexistent
    if (platform < 0 || station->platforms_no <= platform) {
        return;
    }

    // Daca peronul este gol => nu are ce tren sa plece
    if (station->platforms[platform] == NULL) {
        return;
    }

    // Peronul este ocupat
    if (station->platforms[platform]->train_cars != NULL) {
        TrainCar *current_car = station->platforms[platform]->train_cars;
        TrainCar *next_car = current_car->next;
        while (current_car->next != NULL) {
            free(current_car);
            current_car = next_car;
            next_car = next_car->next;
        }
    }
    
    station->platforms[platform] = NULL;
}


/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    // Gara nu exista => you got the point
    if (station == NULL) {
        return;
    }

    // Vagonul nu poate fi adaugat pe un peron inexistent
    if (platform < 0 || station->platforms_no <= platform) {
        return;
    }

    // Daca peronul este gol => nu am la ce locomotiva sa adaug
    if (station->platforms[platform] == NULL) {
        return;
    }

    // Am unde adauga
    TrainCar *car = malloc(sizeof(TrainCar));
    car->weight = weight;
    car->next = NULL;

    if (station->platforms[platform]->train_cars == NULL) {
        station->platforms[platform]->train_cars = car;
        return;
    }

    TrainCar *iterator = station->platforms[platform]->train_cars;
    while (iterator->next != NULL) {
        iterator = iterator->next;
    }
    iterator->next = car;
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    // Gara nu exista => ( ͡° ͜ʖ ͡°)
    if (station == NULL) {
        return;
    }

    // Vagonul nu poate fi scos de pe un peron inexistent
    if (platform < 0 || station->platforms_no <= platform) {
        return;
    }

    // Daca peronul este gol => nu am de unde sa scot
    if (station->platforms[platform] == NULL) {
        return;
    }

    // Daca trenul nu are vagoane, nu am ce sa scot
    if (station->platforms[platform]->train_cars == NULL) {
        return;
    }

    // Parcurg tot trenul, iar match-urile le scot
    TrainCar *iterator = station->platforms[platform]->train_cars;
    while (iterator != NULL && iterator->weight == weight) {
        iterator = iterator->next;
    }
    station->platforms[platform]->train_cars = iterator;
    if (iterator == NULL)
        return;

    while (iterator->next != NULL) {
        if (iterator->next->weight == weight) {
            iterator->next = iterator->next->next;
        } else {
            iterator = iterator->next;
        }
    }
}


/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
                        
    // ATENTIE! URMATOAREA SECVENTA DE COD VA POATE AFECTA EMOTIONAL! ¯\_(ツ)_/¯

    // Gara nu exista bla bla bla
    if (station == NULL) {
        return;
    }

    // Verificam daca platform_a si platform_b sunt valide pentru station
    if (platform_a < 0 || station->platforms_no < platform_a) {
        return;
    }
    if (platform_b < 0 || station->platforms_no < platform_b) {
        return;
    }

    //Verificam daca pos_a si pos_b sunt valide
    if (pos_a <= 0 || pos_b <= 0) {
        return;
    }

    // Verificam daca cars_no este valid
    if (cars_no <= 0) {
        return;
    }

    // Verificam daca platform_a si platform_b sunt goale
    if (station->platforms[platform_a] == NULL) {
        return;
    }
    if (station->platforms[platform_b] == NULL) {
        return;
    }

    // Verificam daca avem ce vagoane avem sa mutam de pe platform_a
    if (station->platforms[platform_a]->train_cars == NULL) {
        return;
    }

    // Obtinem secventa de vagoane
    int i = 1;
    TrainCar *seq_first_car = station->platforms[platform_a]->train_cars;
    TrainCar *to_delete = station->platforms[platform_a]->train_cars;

    while (seq_first_car != NULL && i != pos_a) {
        if (i == pos_a - 1) {
            to_delete = seq_first_car; 
        }
        seq_first_car = seq_first_car->next;
        i++;
    }

    // Verificam daca am ajuns la pos_a
    if (i != pos_a) {
        return;
    }

    TrainCar *seq_last_car = seq_first_car;
    while (seq_last_car != NULL && --cars_no != 0) {
        seq_last_car = seq_last_car->next;
    }

    // Verificam daca lungimea secventei se regaseste in tren
    if (cars_no != 0) {
        return;
    }

    // Crazy insert stuff
    
    // Corner case: inserez la inceput
    if (pos_b == 1) {
        if (pos_a != 1) {
            to_delete->next = seq_last_car->next;
        } else {
            station->platforms[platform_a]->train_cars = seq_last_car->next;
        }

        seq_last_car->next = station->platforms[platform_b]->train_cars;
        station->platforms[platform_b]->train_cars = seq_first_car;
        return;
    }

    i = 1;
    TrainCar *to_insert = station->platforms[platform_b]->train_cars;
    while (to_insert != NULL && to_insert->next != NULL && i != pos_b - 1) {
        to_insert = to_insert->next;
        i++;
    }

    // Verificam daca pos_b este valid pentru trenul de pe platform_b
    if (i != pos_b - 1) {
        return;
    }

    // Corner case: inserez la final
    if (to_insert->next == NULL) {
        if (pos_a != 1) {
            to_delete->next = seq_last_car->next;
        } else {
            station->platforms[platform_a]->train_cars = seq_last_car->next;
        }
        seq_last_car->next = NULL;

        to_insert->next = seq_first_car;
        return;
    }

    if (pos_a != 1) {
        to_delete->next = seq_last_car->next;
    } else {
        station->platforms[platform_a]->train_cars = seq_last_car->next;
    }

    seq_last_car->next = to_insert->next;
    to_insert->next = seq_first_car;

    // EMOTIONAL DAMAGE ENDS HERE! ~(˘▾˘~)
}


/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    int max_weight_diff = 0, express_platform = 0;

    for (int i = 0; i < station->platforms_no; i++) {
        int weight_diff = 0;

        if (station->platforms[i] != NULL) {
            TrainCar *iterator = station->platforms[i]->train_cars;
            while (iterator != NULL) {
                weight_diff += iterator->weight;
                iterator = iterator->next;
            }

            weight_diff = station->platforms[i]->locomotive_power - weight_diff;

            if (weight_diff > max_weight_diff) {
                max_weight_diff = weight_diff;
                express_platform = i;
            }
        }
    }
    return express_platform;
}

/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {
    for (int i = 0; i < station->platforms_no; i++) {
        int weight_diff = 0;

        if (station->platforms[i] != NULL) {
            TrainCar *iterator = station->platforms[i]->train_cars;
            while (iterator != NULL) {
                weight_diff += iterator->weight;
                iterator = iterator->next;
            }

            weight_diff = station->platforms[i]->locomotive_power - weight_diff;

            if (weight_diff < 0) {
                return i;
            }
        }
    }

    return -1;
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    int min_weight_diff = 0, optimal_platform = 0;

    for (int i = 0; i < station->platforms_no; i++) {
        int weight_diff = 0;

        if (station->platforms[i] != NULL) {
            TrainCar *iterator = station->platforms[i]->train_cars;
            while (iterator != NULL) {
                weight_diff += iterator->weight;
                iterator = iterator->next;
            }

            weight_diff = station->platforms[i]->locomotive_power - weight_diff;

            if (i == 0) {
                min_weight_diff = weight_diff;
                optimal_platform = i;
            }

            if (weight_diff < min_weight_diff && i != 0) {
                min_weight_diff = weight_diff;
                optimal_platform = i;
            }
        }
    }

    return optimal_platform;
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    if (station == NULL) {
        return -1;
    }

    int max_heavy_seq_cars = 0, heavy_seq_cars_platform = -1;
    int current_cars_no = 0;
    
    for (int i = 0; i < station->platforms_no; i++) {
        if (station->platforms[i] != NULL && station->platforms[i]->train_cars != NULL) {

            int weight_seq_cars = 0;
            current_cars_no = 0;

            TrainCar *iterator1 = station->platforms[i]->train_cars;
            TrainCar *iterator2 = station->platforms[i]->train_cars;

            while (iterator1 != NULL) {
                current_cars_no++;
                weight_seq_cars += iterator1->weight;

                if (current_cars_no >= cars_no) {
                    if (weight_seq_cars > max_heavy_seq_cars) {
                        max_heavy_seq_cars = weight_seq_cars;
                        heavy_seq_cars_platform = i;
                        *start_car = iterator2;
                    }

                    weight_seq_cars -= iterator2->weight;
                    iterator2 = iterator2->next;
                }

                iterator1 = iterator1->next;
            }
        }
    }

    if (heavy_seq_cars_platform == -1) {
        *start_car = NULL;
    }
    return heavy_seq_cars_platform;
}


/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform) {
    if (station == NULL) {
        return;
    }

    if (station->platforms[platform]->train_cars == NULL) {
        return;
    }

    if (station->platforms[platform]->train_cars->next == NULL) {
        return;
    }

    // Verificam daca vagoanele sunt deja ordonate
    TrainCar *iterator_i = station->platforms[platform]->train_cars;
    while (iterator_i->next != NULL) {
        if (iterator_i->weight < iterator_i->next->weight) {
            break;
        } else {
            iterator_i = iterator_i->next;
        }
        break;
    }
    if (iterator_i->next == NULL) {
        return;
    }

    iterator_i = station->platforms[platform]->train_cars;
    int first_walkthrough = 0;

    while (iterator_i->next != NULL) {
        TrainCar *iterator_j = iterator_i;
        TrainCar *prev_car = iterator_i;
        TrainCar *min_weight_car = iterator_j->next;

        while (iterator_j->next != NULL) {
            if (iterator_j->next->weight < min_weight_car->weight) {
                prev_car = iterator_j;
                min_weight_car = iterator_j->next;
            }
            iterator_j = iterator_j->next;
        }

        prev_car->next = prev_car->next->next;
        
        min_weight_car->next = station->platforms[platform]->train_cars;
        station->platforms[platform]->train_cars = min_weight_car;

        // Verificam daca este prima parcurgere a vagoanelor
        if (first_walkthrough == 0 && iterator_i->next != NULL) {
            first_walkthrough = 1;
            iterator_i = station->platforms[platform]->train_cars;
        }
    }
}


/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
    if (station == NULL) {
        return;
    }

    if (find_overload_train(station) == -1) {
        return;
    }

    int overload_platform = find_overload_train(station);
    TrainCar *iterator = station->platforms[overload_platform]->train_cars;
    int weight_to_remove = 0;

    while (iterator != NULL) {
        weight_to_remove += iterator->weight;
        iterator = iterator->next;
    }

    weight_to_remove -= station->platforms[overload_platform]->locomotive_power;

    iterator = station->platforms[overload_platform]->train_cars;

    TrainCar *prev_car = iterator;
    TrainCar *to_delete = iterator;
    TrainCar *min_weight_car = iterator;

    while (iterator != NULL) {
        if (iterator->weight >= weight_to_remove && iterator->weight < min_weight_car->weight) {
             min_weight_car = iterator;
             to_delete = prev_car;
        } else {
            if (iterator->weight >= weight_to_remove && min_weight_car->weight < weight_to_remove) {
                min_weight_car = iterator;
                to_delete = prev_car;
            }
        }
        
        if (iterator != station->platforms[overload_platform]->train_cars) {
            prev_car = prev_car->next;
        }
        iterator = iterator->next;
    }

    if (min_weight_car == station->platforms[overload_platform]->train_cars) {
        station->platforms[overload_platform]->train_cars = station->platforms[overload_platform]->train_cars->next;
    
    } else {
        to_delete->next = to_delete->next->next;
    }
}
