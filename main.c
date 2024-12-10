#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PASSWORD "hotel el3ayechi"
#define FILENAME_MAX "avis_clients.txt"

typedef struct Room {
    int roomNumber;
    char type[10];
    bool isBooked;
    float pricePerNight;
    struct Room *next;
} Room;

typedef struct Reservation {
    int reservationNumber;
    char clientName[50];
    int roomNumber;
    char startDate[11];
    int periode;
    float totalPrice;
    struct Reservation *next;
} Reservation;
typedef struct ComplaintNode {
    char complaint[200];
    struct ComplaintNode *next;
} ComplaintNode;

Room *roomsHead = NULL;
Reservation *reservationHead = NULL;
ComplaintNode *complaintsFront = NULL, *complaintsRear = NULL;
float N=0;

void loadRooms();
void clientSpace();
void managerSpace();
void checkOut();
void makeReservation();
void makeComplaint();
void viewRevenues();
void viewComplaints();
void searchReservation();
void loadAndDisplayReviews();
void enqueueComplaint(const char *complaint);
char *dequeueComplaint();
void displayAllReservations();
int main() {
    int choice;

    loadRooms();

    while (1) {
        printf("\nBienvenue dans le système Hôtel-Smart!\n");
        printf("Veuillez choisir votre espace:\n");
        printf("1. Espace Client\n");
        printf("2. Espace Gérant\n");
        printf("3. Quitter\n");
        printf("Votre choix: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                clientSpace();
                break;
            case 2: {
                char password[50];
                printf("Veuillez entrer le mot de passe: ");
                fgets(password, 50, stdin);
                password[strcspn(password, "\n")] = 0;

                if (strcmp(password, PASSWORD) == 0) {
                    managerSpace();
                } else {
                    printf("Mot de passe incorrect.\n");
                }
                break;
            }
            case 3:
                printf("Merci d'avoir utilisé Hôtel-Smart. Au revoir!\n");
                exit(0);
            default:
                printf("Choix invalide.\n");
        }
    }

    return 0;
}

void loadRooms() {
    Room *newRoom, *current, *prev = NULL;

    for (int i = 100; i < 200; i++) {
        newRoom = (Room *)malloc(sizeof(Room));
        *newRoom = (Room){i, "single", false, 50.0, NULL};

        if (prev == NULL) {
            roomsHead = newRoom;
        } else {
            prev->next = newRoom;
        }
        prev = newRoom;
    }


    for (int i = 200; i < 300; i++) {
        newRoom = (Room *)malloc(sizeof(Room));
        *newRoom = (Room){i, "double", false, 90.0, NULL};

        prev->next = newRoom;
        prev = newRoom;
    }
}

void enqueueComplaint(const char *complaint) {
    ComplaintNode *newNode = (ComplaintNode *)malloc(sizeof(ComplaintNode));
    strcpy(newNode->complaint, complaint);
    newNode->next = NULL;

    if (complaintsRear == NULL) {
        complaintsFront = complaintsRear = newNode;
    } else {
        complaintsRear->next = newNode;
        complaintsRear = newNode;
    }
}

char *dequeueComplaint() {
    if (complaintsFront == NULL) {
        return NULL;
    }

    ComplaintNode *temp = complaintsFront;
    complaintsFront = complaintsFront->next;
    if (complaintsFront == NULL) {
        complaintsRear = NULL;
    }

    char *complaint = (char *)malloc(strlen(temp->complaint) + 1);
    strcpy(complaint, temp->complaint);
    free(temp);

    return complaint;
}

void clientSpace() {
    int choice;
    while (1) {
        printf("\nBienvenue dans l'espace client!\n");
        printf("1. Faire une réservation\n");
        printf("2. Check-out\n");
        printf("3. Faire une réclamation\n");
        printf("4. Retour au menu principal\n");
        printf("Votre choix: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                makeReservation();
                break;
            case 2:
                checkOut();
                break;
            case 3:
                makeComplaint();
                break;
            case 4:
                return;
            default:
                printf("Choix invalide.\n");
        }
    }
}

void managerSpace() {
    int choice;

    while (1) {
        printf("\nBienvenue dans l'espace gérant!\n");
        printf("1. Afficher le chiffres d'affaire\n");
        printf("2. Afficher les réclamations des clients\n");
        printf("3. Rechercher une réservation\n");
        printf("4. Afficher toutes les réservations\n");
        printf("5. Afficher les avis des clients\n");
        printf("6. Retour au menu principal\n");
        printf("Votre choix: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                viewRevenues();
                break;
            case 2:
                viewComplaints();
                break;
            case 3:
                searchReservation();
                break;
            case 4:
                displayAllReservations();
                break;
            case 5:
                loadAndDisplayReviews();
                break;;
            case 6:
                return;
            default:
                printf("Choix invalide.\n");
        }
    }
}

void checkOut() {
    int roomNumber;
    char review[200];
    int reservationFound = 0;

    printf("Veuillez entrer votre numéro de chambre: ");
    scanf("%d", &roomNumber);
    getchar();

    printf("Veuillez donner votre avis sur le séjour: ");
    fgets(review, sizeof(review), stdin);
    review[strcspn(review, "\n")] = '\0';

    FILE *file = fopen("avis_clients.txt", "a");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier avis_clients.txt.\n");
        return;
    }
    fprintf(file, "Chambre %d: %s\n", roomNumber, review);
    fclose(file);

    printf("Merci pour votre avis. Il a été enregistré.\n");

    Room *currentRoom = roomsHead;
    while (currentRoom != NULL) {
        if (currentRoom->roomNumber == roomNumber) {
            if (!currentRoom->isBooked) {
                printf("La chambre %d n'était pas réservée.\n", roomNumber);
                return;
            }
            currentRoom->isBooked = false;
            printf("Chambre %d libérée avec succès!\n", roomNumber);
            reservationFound = 1;
            break;
        }
        currentRoom = currentRoom->next;
    }

    if (!reservationFound) {
        printf("Numéro de chambre non valide ou non réservé.\n");
        return;
    }

    Reservation *currentReservation = reservationHead;
    Reservation *previousReservation = NULL;

    while (currentReservation != NULL) {
        if (currentReservation->roomNumber == roomNumber) {
            if (previousReservation == NULL) {
                reservationHead = currentReservation->next;
            } else {
                previousReservation->next = currentReservation->next;
            }
            free(currentReservation);
            printf("Réservation associée supprimée.\n");
            return;
        }
        previousReservation = currentReservation;
        currentReservation = currentReservation->next;
    }

    printf("Aucune réservation trouvée pour la chambre %d.\n", roomNumber);
}

void makeReservation() {
    char clientName[50];
    char type[10];
    char startDate[11];
    int per;

    printf("Entrez votre nom: ");
    fgets(clientName, 50, stdin);
    clientName[strcspn(clientName, "\n")] = '\0';

    printf("Type de chambre (single/double): ");
    scanf("%s", type);
    printf("Date de début (YYYY-MM-DD): ");
    scanf("%s", startDate);
    printf("Période (en nuits): ");
    scanf("%d", &per);

    Room *current = roomsHead;
    while (current != NULL) {
        if (strcmp(current->type, type) == 0 && !current->isBooked) {
            printf("Chambre disponible: %d à %.2f par nuit.\n", current->roomNumber, current->pricePerNight);
            printf("Confirmer la réservation? (1: Oui, 0: Non): ");
            int confirm;
            scanf("%d", &confirm);

            if (confirm == 1) {
                float totalPrice = current->pricePerNight * per;
                N += totalPrice;
                current->isBooked = true;
                Reservation *newReservation = (Reservation *)malloc(sizeof(Reservation));
                newReservation->reservationNumber = rand();
                strcpy(newReservation->clientName, clientName);
                strcpy(newReservation->startDate, startDate);
                newReservation->periode = per;
                newReservation->roomNumber = current->roomNumber;
                newReservation->totalPrice = per * current->pricePerNight;
                newReservation->next = NULL;

                if (reservationHead == NULL) {
                    reservationHead = newReservation;
                } else {
                    Reservation *cur = reservationHead;
                    while (cur->next != NULL) {
                        cur = cur->next;
                    }
                    cur->next = newReservation;
                }

                printf("Réservation confirmée! Numéro de chambre: %d.\n", current->roomNumber);
                return;
            } else {
                printf("Réservation annulée.\n");
                return;
            }
        }
        current = current->next;
    }
    printf("Désolé, aucune chambre disponible pour cette période.\n");
}

void makeComplaint() {
    char complaint[200];
    int roomNumber;

    printf("Veuillez saisir votre numéro de chambre: ");
    scanf("%d", &roomNumber);
    getchar();

    printf("Veuillez saisir votre réclamation: ");
    fgets(complaint, 200, stdin);

    char fullComplaint[256];
    snprintf(fullComplaint, sizeof(fullComplaint), "Chambre %d: %s", roomNumber, complaint);

    enqueueComplaint(fullComplaint);

    printf("Votre réclamation a été enregistrée. Merci!\n");
}

void searchReservation() {
    char searchName[50];
    printf("Entrez le nom du client à rechercher: ");
    fgets(searchName, 50, stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    Reservation *current = reservationHead;
    bool found = false;

    printf("Résultats de la recherche pour le client '%s':\n", searchName);

    while (current != NULL) {
        if (strcmp(current->clientName, searchName) == 0) {
            printf("----------------------------------\n");
            printf("Numéro de réservation: %d\n", current->reservationNumber);
            printf("Nom du client: %s\n", current->clientName);
            printf("Numéro de chambre: %d\n", current->roomNumber);
            printf("Date de début: %s\n", current->startDate);
            printf("Durée: %d nuits\n", current->periode);
            printf("Prix total: %.2f\n", current->totalPrice);
            printf("----------------------------------\n");
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        printf("Aucune réservation trouvée pour le client '%s'.\n", searchName);
    }
}

void displayAllReservations() {
    Reservation *current = reservationHead;
    if (current == NULL) {
        printf("Aucune réservation n'a été effectuée.\n");
        return;
    }

    printf("Liste des réservations:\n");
    while (current != NULL) {
        printf("Numéro de réservation: %d\n", current->reservationNumber);
        printf("Nom du client: %s\n", current->clientName);
        printf("Numéro de chambre: %d\n", current->roomNumber);
        printf("Date de début: %s\n", current->startDate);
        printf("Durée: %d nuits\n", current->periode);
        printf("Prix total: %.2f\n", current->totalPrice);
        printf("----------------------------------\n");
        current = current->next;
    }
}

void viewRevenues() {
    printf("Chiffre d'affaires total: %.2f\n", N);
}

void viewComplaints() {
    printf("\nRéclamations des clients:\n");
    if (complaintsFront == NULL) {
        printf("- Aucune réclamation enregistrée.\n");
    } else {
        ComplaintNode *currentComplaint = complaintsFront;
        ComplaintNode *prevComplaint = NULL;

        while (currentComplaint != NULL) {
            printf("\n- %s\n", currentComplaint->complaint);

            int userChoice;
            while (1) {
                printf("\nChoisissez une option:\n");
                printf("1. Supprimer cette réclamation\n");
                printf("2. Conserver cette réclamation\n");
                printf("3. Retour au menu principal\n");
                printf("Votre choix: ");
                scanf("%d", &userChoice);
                getchar();

                if (userChoice == 1) {
                    printf("Réclamation supprimée.\n");
                    if (prevComplaint == NULL) {
                        complaintsFront = currentComplaint->next;
                    } else {
                        prevComplaint->next = currentComplaint->next;
                    }

                    ComplaintNode *temp = currentComplaint;
                    currentComplaint = currentComplaint->next;

                    if (complaintsRear == temp) {
                        complaintsRear = prevComplaint;
                    }

                    free(temp);
                    break;
                } else if (userChoice == 2) {
                    printf("Réclamation conservée.\n");
                    prevComplaint = currentComplaint;
                    currentComplaint = currentComplaint->next;
                    break;
                } else if (userChoice == 3) {
                    printf("Retour au menu principal.\n");
                    return;
                } else {
                    printf("Choix invalide. Veuillez essayer à nouveau.\n");
                }
            }
        }
    }
}

void loadAndDisplayReviews() {
    FILE *file = fopen("avis_clients.txt", "r");
    if (file == NULL) {
        printf("Aucun avis trouvé ou impossible d'ouvrir le fichier.\n");
        return;
    }

    char reviews[100][200];
    int count = 0;
    while (fgets(reviews[count], sizeof(reviews[count]), file)) {
        reviews[count][strcspn(reviews[count], "\n")] = '\0';
        count++;
    }
    fclose(file);

    while (1) {
        printf("\nLes avis des clients sont :\n");
        for (int i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, reviews[i]);
        }

        printf("\nOptions :\n");
        printf("1. Supprimer un avis\n");
        printf("2. Retour au menu principal\n");
        printf("Votre choix : ");
        int choice;
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            printf("Entrez le numéro de l'avis à supprimer : ");
            int deleteIndex;
            scanf("%d", &deleteIndex);
            getchar();

            if (deleteIndex < 1 || deleteIndex > count) {
                printf("Numéro d'avis invalide.\n");
            } else {
                FILE *file = fopen("avis_clients.txt", "w");
                if (file == NULL) {
                    printf("Erreur lors de l'ouverture du fichier pour supprimer l'avis.\n");
                    return;
                }

                for (int i = 0; i < count; i++) {
                    if (i != deleteIndex - 1) {
                        fprintf(file, "%s\n", reviews[i]);
                    }
                }

                fclose(file);
                printf("Avis supprimé avec succès.\n");
                return;
            }
        } else if (choice == 2) {
            printf("Retour au menu principal.\n");
            return;
        } else {
            printf("Choix invalide. Veuillez essayer à nouveau.\n");
        }
    }
}



