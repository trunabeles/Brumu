#include <SoftwareSerial.h>
#include <Servo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// CONEXOES PARA  BLUETOOTH
int bluetoothTx = 6;
int bluetoothRx = 7;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

// Pinos do motor e do driver
int motor1Pin1 = 3;
int motor1Pin2 = 2;
int motor2Pin1 = 5;
int motor2Pin2 = 4;
int enableA = 9;  // PWM para o motor A
int enableB = 10; // PWM para o motor B

// Velocidade e Distancia a ser percorrida
const int velocidade = 200; // Velocidade PWM (0-255) - forca

// Parâmetros específicos do carrinho (calibrar conforme necessário)
const float tempoPara10cm = 370; // Tempo em ms para percorrer 10 cm (precisa ser calibrado)
const float tempoPara90Graus = 740;  // Tempo em ms para virar 90 graus (precisa ser calibrado)

void FrenteComRampa(int velocidadeInicial, int velocidadeFinal, int duracao) {
  for (int t = 0; t <= duracao; t += 10) {
    int velocidade = map(t, 0, duracao, velocidadeInicial, velocidadeFinal);
    analogWrite(enableA, velocidade);
    analogWrite(enableB, velocidade);
    delay(10);
  }
}

void Frente() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  FrenteComRampa(100, 200, 370); // Acelera suavemente para PWM 100 em 1 segundo
  delay(tempoPara10cm);         // Mantém movimento constante
  FrenteComRampa(200, 0, 370); // Desacelera suavemente em 1 segundo
}

void Direita(){
    // Configura os motores para girar à direita (um motor para frente, outro para trás)
    digitalWrite(motor1Pin1, LOW); // Motor esquerdo para frente
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, HIGH);  // Motor direito para trás
    digitalWrite(motor2Pin2, LOW);

    // Ajusta a velocidade dos motores
    analogWrite(enableA, velocidade);
    analogWrite(enableB, velocidade);

    // Girar por 90 graus (tempo calculado)
    delay(tempoPara90Graus);

    // Para os motores após girar 90 graus
    analogWrite(enableA, 0);
    analogWrite(enableB, 0);
    delay(2000);
}

void Esquerda(){
    // Configura os motores para girar à esquerda (um motor para frente, outro para trás)
    digitalWrite(motor1Pin1, HIGH); // Motor esquerdo para frente
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);  // Motor direito para trás
    digitalWrite(motor2Pin2, HIGH);

    // Ajusta a velocidade dos motores
    analogWrite(enableA, velocidade);
    analogWrite(enableB, velocidade);

    // Girar por 90 graus (tempo calculado)
    delay(tempoPara90Graus);

    // Para os motores após girar 90 graus
    analogWrite(enableA, 0);
    analogWrite(enableB, 0);
    delay(2000);
}

void setup (){
  bluetooth.begin(9600);
  Serial.begin(9600);
  pinMode( motor1Pin1, OUTPUT );
  pinMode( motor2Pin1, OUTPUT );
  pinMode( motor1Pin2, OUTPUT );
  pinMode( motor2Pin2, OUTPUT );
}


void loop() {
  // Verifica se há dados disponíveis do módulo Bluetooth
  if (bluetooth.available()) {
    String receivedText = "";  // Cria uma string para armazenar o texto recebido
    
    // Lê todos os caracteres disponíveis
    while (bluetooth.available()) {
      char c = bluetooth.read();  // Lê um caractere do Bluetooth
      receivedText += c;          // Adiciona o caractere à string
      delay(10);                  // Aguarda um pouco para garantir a leitura correta
    }
    // Separar o texto em letras e imprimir cada uma delas
    Serial.println("Letras separadas:");
    for (int i = 0; i < receivedText.length(); i++) {
      if(receivedText[i] != ' '){
        if(receivedText[i] == 'P'){
          digitalWrite( motor1Pin1, LOW);
          analogWrite( motor1Pin2, LOW);

          digitalWrite( motor2Pin1, LOW),
          analogWrite( motor2Pin2, LOW);
        }
        else if (receivedText[i] == 'F') {
          Frente();
        }
        else if (receivedText[i] == 'D') {
          Direita();
        }
        else if (receivedText[i] == 'E') {
          Esquerda();
        }
      } 
    }
  }
}
