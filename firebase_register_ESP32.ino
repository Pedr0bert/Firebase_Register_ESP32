#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>

//== CREDENCIAIS //
#define WIFI_SSID "SSID" // SSID da rede
#define WIFI_PASSWORD "SENHA" // Senha da rede
#define FIREBASE_HOST "URL_REALTIME_DATABASE_FIREBASE" // URL do Realtime Database
#define FIREBASE_AUTH "CHAVE_API" // CHAVE DE API (API KEY)

//======== RFID =======//
#define SS_PIN 5
#define RST_PIN 22 
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ==== LCD 16x2 (sem I2C) ====
#define LCD_RS 13
#define LCD_E 12
#define LCD_D4 14
#define LCD_D5 27
#define LCD_D6 26
#define LCD_D7 25
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

String currentUser = "";
String currentUserNome = "";

// FUNCAO PARA DEFINIR AS LEITURAS DO CARTAO //
void setLeitura (String PATH, int leitura = 0) {
  int adicionar = Firebase.getInt(PATH);
  adicionar++;
  Firebase.setInt(PATH, adicionar);
  Serial.println("Adicionado");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando...");

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando LCD...");
  delay(1000);
  lcd.clear();
  lcd.print("Aproxime CARTAO");

  // RFID
  Serial.println("Inicializando SPI para RFID...");
  SPI.begin(18, 19, 23, 5); 
  mfrc522.PCD_Init();
  Serial.println("RFID iniciado com sucesso!");
  Serial.println("Aproxime o cartão/tag do leitor...");

  // Wi-Fi
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.println("Connected to wifi\n"); 

  // Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Verifica a presença de um cartão
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      String uid = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i], HEX);
      }
      uid.toUpperCase();
      Serial.print("UID lido: ");
      Serial.println(uid);

      String nomePath = "/cartoes/" + uid + "/nome";
      if (Firebase.getString(nomePath)) { 
        currentUser = uid;
        currentUserNome = Firebase.getString(nomePath); 
        //sacos = 0; // Reseta o valor de sacos

        String timePath = "/cartoes/" + currentUser + "/timestamp";
        Firebase.setInt(timePath, millis()); 

        String leituraPath = "/cartoes/" + currentUser + "/leituras";
        setLeitura(leituraPath); 

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bem-vindo:");
        lcd.setCursor(0, 1);
        lcd.print(currentUserNome.substring(0, 16));

        delay(1500);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Aproxime CARTAO");

      } else {
        Serial.println("Colaborador NAO encontrado!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cartao invalido");
        delay(1500);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Aproxime CARTAO");
      }

      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
  }

}
