#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "CB_RX1.h"
#include "CB_TX1.h"

#define CBRX1_BUFFER_SIZE 128
int cbRx1Head;
int cbRx1Tail;
unsigned char cbRx1Buffer[CBRX1_BUFFER_SIZE];

// Fonction pour ajouter une donnée dans le tampon circulaire de réception
void CB_RX1_Add(unsigned char value) {
    cbRx1Buffer[cbRx1Head] = value;
    cbRx1Head = (cbRx1Head + 1) % CBRX1_BUFFER_SIZE;  // Incrémentation circulaire de head
}

// Fonction pour récupérer une donnée du tampon circulaire de réception
unsigned char CB_RX1_Get(void) {
    unsigned char value = cbRx1Buffer[cbRx1Tail];
    cbRx1Tail = (cbRx1Tail + 1) % CBRX1_BUFFER_SIZE;  // Incrémentation circulaire de tail
    return value;
}

// Fonction pour vérifier si des données sont disponibles dans le tampon de réception
unsigned char CB_RX1_IsDataAvailable(void) {
    return cbRx1Head != cbRx1Tail;  // Vérifie si le tampon de réception contient des données
}

// Interruption UART : ajout des données reçues dans le tampon
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;  // Effacer le flag d'interruption RX
    if (U1STAbits.FERR == 1) {
        U1STAbits.FERR = 0;  // Réinitialiser l'erreur de parité
    }
    if (U1STAbits.OERR == 1) {
        U1STAbits.OERR = 0;  // Réinitialiser l'erreur de débordement
    }
    while (U1STAbits.URXDA == 1) {
        CB_RX1_Add(U1RXREG);  // Ajouter la donnée reçue dans le tampon circulaire
    }
}

// Fonction pour obtenir la taille des données présentes dans le tampon
int CB_RX1_GetDataSize(void) {
    if (cbRx1Head >= cbRx1Tail) {
        return cbRx1Head - cbRx1Tail;  // Taille des données si head est après tail
    } else {
        return CBRX1_BUFFER_SIZE - (cbRx1Tail - cbRx1Head);  // Taille des données si head a tourné
    }
}

// Fonction pour obtenir la taille restante dans le tampon
int CB_RX1_GetRemainingSize(void) {
    return CBRX1_BUFFER_SIZE - 1 - CB_RX1_GetDataSize();  // Taille restante dans le tampon, -1 pour ne pas écraser la donnée
}
