# Firebase_Register_ESP32

## 🛠️ Sistema de Leitura de Cartões RFID com ESP32 e Firebase

Este projeto implementa um sistema de identificação de usuários por meio de cartões RFID usando um **ESP32**, com exibição em **LCD 16x2** e integração com o **Firebase Realtime Database**. O objetivo é registrar a leitura de cartões (RFID) associados a colaboradores e armazenar essas informações remotamente.

## 📌 Funcionalidades

- Leitura de cartões RFID usando MFRC522
- Exibição do nome do colaborador no display LCD
- Integração com o Firebase para registrar:
  - UID do cartão
  - Nome do usuário
  - Timestamp da leitura
  - Quantidade total de leituras
- Conexão com rede Wi-Fi para comunicação em tempo real
---


## ⚙️ Tecnologias Utilizadas

| Tecnologia                     | Descrição |
|--------------------------------|---------------------------------------------------------------|
| **ESP32**                      | Microcontrolador principal do projeto                         |
| **MFRC522**                    | Leitor RFID para leitura de cartões/tag                       |
| **Firebase Realtime Database** | Banco de dados em tempo real usado para armazenar as leituras |
| **Wi-Fi**                      | Comunicação do ESP32 com o Firebase                           |
| **LCD 16x2 (sem I2C)**         | Exibição de mensagens ao usuário                              |
| **Arduino IDE**                | Desenvolvimento e upload do código                            |
---

## 📋 Estrutura do Banco de Dados no Firebase

```json
/cartoes
  └── UID_CARTAO
      ├── nome: "Nome do Colaborador"
      ├── leituras: 1
      └── timestamp: 123456789
