#include <xc.h>
#include "UART_Protocol.h"
unsigned char UartCalculateChecksum(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
//Fonction prenant entree la trame et sa longueur pour calculer le checksum
 unsigned char checksum = 0; 
 checksum ^= (msgFunction & 0xFF); 
 checksum ^= ((msgFunction >> 8) & 0xFF );
 checksum ^= msgPayloadLength ;
 for (int i = 0; i < msgPayloadLength; i++){
     checksum ^= msgPayload{1};
 }
 return checksum ;
     
 
}
void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
//Fonction d?encodage et d?envoi d?un message
  unsigned char frame[128];  
  int index = 0; frame[index++] = (msgFunction >> 8) & 0xFF; 
  frame[index++] = msgFunction & 0xFF; 
  frame[index++] = msgPayloadLength; 
  for (int i = 0; i < msgPayloadLength; i++) { 
      frame[index++] = msgPayload[i]; 
  }
frame[index++] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload); 
for (int i = 0; i < index; i++) {
    int UartSendByte(frame[i]); 
 }
}
int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

void UartDecodeMessage(unsigned char c)
{
//Fonction prenant en entree un octet et servant a reconstituer les trames
static unsigned char buffer[128]; 
static int bufferIndex = 0; 
static int expectedLength = 0; 
buffer[bufferIndex++] = c; 
if (bufferIndex == 3) { 
    expectedLength = buffer[2] + 4;  
} 
if (bufferIndex == expectedLength) { 
    int msgFunction = (buffer[0] << 8) | buffer[1]; 
    int msgPayloadLength = buffer[2]; 
    unsigned char* msgPayload = &buffer[3]; 
    unsigned char receivedChecksum = buffer[expectedLength - 1]; 
}
if (UartCalculateChecksum(msgFun.ction, msgPayloadLength, msgPayload) == (receivedChecksum)) { 

UartProcessDecodedMessage(msgFunction, msgPayloadLength, msgPayload);  
bufferIndex = 0; 
}}

void UartProcessDecodedMessage(int function,int payloadLength, unsigned char* payload)
{
//Fonction appelee apres le decodage pour executer l?action
//correspondant au message recu
switch (function) { case 0x0080: // Transmission de texte 
    

printf("Texte reçu : %s\n", payload); break; 
case 0x0020: // Réglage LED 
    printf("Réglage LED : numéro %d, état %s\n", payload[0], payload[1] ? "allumée" : "éteinte"); break; 
    case 0x0030: // Distance télémètre IR
        printf("Distances IR : Gauche = %d, Droite = %d\n", payload[0], payload[1]); break; 
        case 0x0040: // Consigne de vitesse 
            printf("Vitesse : Gauche = %d%%, Droite = %d%%\n", payload[0], payload[1]); break; 
            default: printf("Commande inconnue : 0x%04X\n", function); break; } 
}

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/
        