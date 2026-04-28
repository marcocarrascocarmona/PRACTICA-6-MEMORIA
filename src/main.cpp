#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    if (buffer[i] < 0x10) Serial.print("0");
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
}

void printAscii(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    if (buffer[i] >= 32 && buffer[i] <= 126) {
      Serial.print((char)buffer[i]);
    } else {
      Serial.print(".");
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  SPI.begin(18, 19, 23, SS_PIN); // SCK, MISO, MOSI, SS
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF; // clave por defecto
  }

  Serial.println();
  Serial.println("RC522 listo.");
  Serial.println("Acerca una tarjeta MIFARE Classic 1K...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.println();
  Serial.println("=== TARJETA DETECTADA ===");

  Serial.print("UID: ");
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.print("Tipo: ");
  Serial.println(rfid.PICC_GetTypeName(piccType));

  Serial.println();
  Serial.println("Leyendo memoria con clave FFFFFFFFFFFF...");
  Serial.println();

  // MIFARE Classic 1K = 64 bloques, 16 bytes por bloque
  for (byte block = 0; block < 64; block++) {

    // Cada sector tiene 4 bloques. El último es trailer: 3,7,11...
    bool isTrailerBlock = ((block + 1) % 4 == 0);

    MFRC522::StatusCode status;

    status = rfid.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_A,
      block,
      &key,
      &(rfid.uid)
    );

    if (status != MFRC522::STATUS_OK) {
      Serial.print("Bloque ");
      if (block < 10) Serial.print("0");
      Serial.print(block);
      Serial.print(" | ERROR auth: ");
      Serial.println(rfid.GetStatusCodeName(status));
      continue;
    }

    byte buffer[18];
    byte size = sizeof(buffer);

    status = rfid.MIFARE_Read(block, buffer, &size);

    Serial.print("Bloque ");
    if (block < 10) Serial.print("0");
    Serial.print(block);

    if (isTrailerBlock) {
      Serial.print(" [TRAILER]");
    } else {
      Serial.print("          ");
    }

    Serial.print(" | HEX: ");
    if (status == MFRC522::STATUS_OK) {
      printHex(buffer, 16);
      Serial.print(" | ASCII: ");
      printAscii(buffer, 16);
      Serial.println();
    } else {
      Serial.print("ERROR read: ");
      Serial.println(rfid.GetStatusCodeName(status));
    }
  }

  Serial.println();
  Serial.println("Lectura terminada.");
  Serial.println("=========================");

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(3000);
}