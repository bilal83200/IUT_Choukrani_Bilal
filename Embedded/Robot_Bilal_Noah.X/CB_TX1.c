#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "CB_TX1.h"

#define CBTX1_BUFFER_SIZE 128 // Taille du tampon circulaire

// Déclarations globales
int cbTx1Head = 0; // Index de la tête (lecture)
int cbTx1Tail = 0; // Index de la queue (écriture)
unsigned char cbTx1Buffer[CBTX1_BUFFER_SIZE]; // Tampon circulaire
unsigned char isTransmitting = 0; // État de transmission (0 = inactif, 1 = actif)

// Fonction pour envoyer un message complet
void SendMessage(unsigned char* message, int length) {
    unsigned char i = 0;

    if (CB_TX1_GetRemainingSize() > length) {
        for (i = 0; i < length; i++) {
            CB_TX1_Add(message[i]);
        }

        if (!CB_TX1_IsTranmitting()) {
            SendOne();
        }
    }
}

void CB_TX1_Add(unsigned char value) {
    int next = (cbTx1Tail + 1) % CBTX1_BUFFER_SIZE;

    // Vérifie si le tampon n'est pas plein
    if (next != cbTx1Head) {
        cbTx1Buffer[cbTx1Tail] = value; // Ajoute la valeur
        cbTx1Tail = next; // Met à jour l'index de la queue
    } else {
        // Si le tampon est plein, aucune action n'est prise
        // Vous pourriez ajouter une gestion d'erreur ici
    }
}

// Fonction pour récupérer un octet depuis le tampon circulaire
unsigned char CB_TX1_Get(void) {
    if (cbTx1Head != cbTx1Tail) { // Vérifie si le tampon contient des données
        unsigned char value = cbTx1Buffer[cbTx1Head]; // Lit l'octet
        cbTx1Head = (cbTx1Head + 1) % CBTX1_BUFFER_SIZE; // Avance la tête
        return value; // Retourne l'octet lu
    } else {
        return 0; // Tampon vide, retourne une valeur par défaut
    }
}

// Gestionnaire d'interruption UART (transmission terminée)
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; // Réinitialise le drapeau d'interruption UART

    // Vérifie si des caractères restent à transmettre
    if (cbTx1Tail != cbTx1Head) {
        SendOne(); // Transmet le caractère suivant
    } else {
        isTransmitting = 0; // Marque la fin de la transmission
    }
}

// Fonction pour envoyer un seul caractère depuis le tampon circulaire
void SendOne() {
    isTransmitting = 1; // Marque la transmission comme active
    unsigned char value = CB_TX1_Get(); // Récupère un caractère depuis le tampon
    U1TXREG = value; // Écrit le caractère dans le registre de transmission UART
}

// Fonction pour vérifier si une transmission est en cours
unsigned char CB_TX1_IsTranmitting(void) {
    return isTransmitting; // Retourne l'état de la transmission
}

// Fonction pour calculer la taille des données présentes dans le tampon
int CB_TX1_GetDataSize(void) {
    return (cbTx1Tail - cbTx1Head + CBTX1_BUFFER_SIZE) % CBTX1_BUFFER_SIZE; // Taille des données
}

// Fonction pour calculer l'espace restant disponible dans le tampon
int CB_TX1_GetRemainingSize(void) {
    return CBTX1_BUFFER_SIZE - CB_TX1_GetDataSize() - 1; // Taille restante
}


