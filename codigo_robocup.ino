// PROGRAMA ADAPTADO PARA TESTE VIA SERIAL MONITOR (Arduino IDE)
// Comandos: w = frente, s = ré, a = esquerda, d = direita, q = parar
// Abra o Serial Monitor em 9600 baud e envie os caracteres.

#include <Servo.h>

// Portas usados para o controle do Driver de Motor
char DIR_Latch  = 12;
char DIR_SER    = 8;
char DIR_CLK    = 4;

char PWM_M1     = 11;
char PWM_M2     = 3;
char PWM_M3     = 5;
char PWM_M4     = 6;

// Porta usada para o Sensor
char sensor     = 22;

// Variáveis de Estado
char estado     = 0;
char estado_mov = 0;

// Variáveis de Temporização
unsigned long tempo_arma = 0;
unsigned long tempo_mov  = 0;

int valor_b = 0;

// Variável de recebimento de dados serial
int data_by_serial = 0;

// LEDs RGB
int LED_R_1 = 46;
int LED_G_1 = 45;
int LED_B_1 = 44;

Servo myservo_1;
Servo myservo_2;

// Função para acionamento individual de motor
void aciona_motor_b(int M, int S, int M_P) {
  char i = 0;
  int valor_b_temp = 0;

  if (M == 1) {
    analogWrite(PWM_M1, M_P);
    valor_b = valor_b & 0b11110011;
    if (S == 1)      { valor_b = valor_b | 0b00000100; }
    else if (S == 2) { valor_b = valor_b | 0b00001000; }
  }

  if (M == 2) {
    analogWrite(PWM_M2, M_P);
    valor_b = valor_b & 0b11101101;
    if (S == 1)      { valor_b = valor_b | 0b00000010; }
    else if (S == 2) { valor_b = valor_b | 0b00010000; }
  }

  if (M == 3) {
    analogWrite(PWM_M3, M_P);
    valor_b = valor_b & 0b10111110;
    if (S == 1)      { valor_b = valor_b | 0b00000001; }
    else if (S == 2) { valor_b = valor_b | 0b01000000; }
  }

  if (M == 4) {
    analogWrite(PWM_M4, M_P);
    valor_b = valor_b & 0b01011111;
    if (S == 1)      { valor_b = valor_b | 0b00100000; }
    else if (S == 2) { valor_b = valor_b | 0b10000000; }
  }

  valor_b_temp = valor_b;
  for (i = 0; i <= 7; i++) {
    analogWrite(DIR_SER, LOW);
    if ((valor_b_temp & 0b10000000) == 128) {
      digitalWrite(DIR_SER, HIGH);
    }
    delay(1);
    digitalWrite(DIR_CLK, HIGH);
    delay(1);
    digitalWrite(DIR_CLK, LOW);
    valor_b_temp = valor_b_temp * 2;
  }

  digitalWrite(DIR_Latch, HIGH);
  delay(1);
  digitalWrite(DIR_Latch, LOW);
}

// Função para tratamento de dado recebido
void Trata_serial(void) {
  estado_mov = 1;

  Serial.print("Comando recebido: ");
  Serial.println((char)data_by_serial);

  if (data_by_serial == 's') {
    Serial.println(">> Movimento: (Frente)");
    aciona_motor_b(1, 1, 225);
    aciona_motor_b(2, 2, 225);
  }

  if (data_by_serial == 'w') {
    Serial.println(">> Movimento: FRENTE (Tras)");
    aciona_motor_b(1, 2, 225);
    aciona_motor_b(2, 1, 225);
  }

  if (data_by_serial == 'a') {
    Serial.println(">> Movimento: ESQUERDA");
    aciona_motor_b(2, 2, 225);
    aciona_motor_b(2, 1, 225);
  }

  if (data_by_serial == 'd') {
    Serial.println(">> Movimento: DIREITA");
    aciona_motor_b(1, 1, 225);
    aciona_motor_b(1, 2, 225);
  }

  if (data_by_serial == 'q') {
    Serial.println(">> Movimento: PARAR");
    aciona_motor_b(1, 0, 0);
    aciona_motor_b(2, 0, 0);
  }

  tempo_mov = millis();
}

// Setup
void setup() {
  Serial.begin(9600);
  Serial.println("========================================");
  Serial.println(" ROBO SI - Teste via Serial Monitor");
  Serial.println("========================================");
  Serial.println(" Comandos disponiveis:");
  Serial.println("   w = Frente");
  Serial.println("   s = Re");
  Serial.println("   a = Esquerda");
  Serial.println("   d = Direita");
  Serial.println("   q = Parar");
  Serial.println("   t = Acionar arma (simula sensor)");
  Serial.println("========================================");
  Serial.println(" Aguardando comandos...");
  Serial.println();

  myservo_1.attach(9);   // << AJUSTE O PINO DO SERVO 1 AQUI
  myservo_2.attach(10);  // << AJUSTE O PINO DO SERVO 2 AQUI
  myservo_1.write(180);
  myservo_2.write(0);

  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);

  pinMode(PWM_M1, OUTPUT);
  pinMode(PWM_M2, OUTPUT);
  pinMode(PWM_M3, OUTPUT);
  pinMode(PWM_M4, OUTPUT);

  analogWrite(PWM_M1, 0);
  analogWrite(PWM_M2, 0);
  analogWrite(PWM_M3, 0);
  analogWrite(PWM_M4, 0);

  pinMode(DIR_SER, OUTPUT);
  pinMode(DIR_CLK, OUTPUT);
  pinMode(DIR_Latch, OUTPUT);

  digitalWrite(DIR_SER, LOW);
  digitalWrite(DIR_CLK, LOW);
  digitalWrite(DIR_Latch, LOW);

  pinMode(sensor, INPUT_PULLUP);

  // Teste LED RGB
  Serial.println("[LED] Vermelho...");
  analogWrite(LED_R_1, 255);
  analogWrite(LED_G_1, 0);
  analogWrite(LED_B_1, 0);
  delay(1500);

  Serial.println("[LED] Verde...");
  analogWrite(LED_R_1, 0);
  analogWrite(LED_G_1, 255);
  analogWrite(LED_B_1, 0);
  delay(1500);

  Serial.println("[LED] Azul...");
  analogWrite(LED_R_1, 0);
  analogWrite(LED_G_1, 0);
  analogWrite(LED_B_1, 255);
  delay(1500);

  Serial.println("[LED] Verde (pronto)...");
  analogWrite(LED_R_1, 0);
  analogWrite(LED_G_1, 255);
  analogWrite(LED_B_1, 0);
  delay(1500);

  Serial.println();
  Serial.println(">> Sistema PRONTO! Envie um comando.");
  Serial.println();
}

// Loop principal
void loop() {

  // ===== LEITURA VIA SERIAL MONITOR (substitui Serial3 / Bluetooth) =====
  if (Serial.available()) {
    data_by_serial = Serial.read();

    // Comando 't' simula o acionamento do sensor da arma
    if (data_by_serial == 't') {
      Serial.println(">> SIMULANDO acionamento do sensor da arma!");
      if (estado == 0) {
        estado = 1;
        tempo_arma = millis();
        myservo_1.write(0);
        myservo_2.write(180);
        Serial.println("[ARMA] Acionando...");
        analogWrite(LED_R_1, 255);
        analogWrite(LED_G_1, 0);
        analogWrite(LED_B_1, 0);
      } else {
        Serial.print("[ARMA] Ocupada, estado atual = ");
        Serial.println((int)estado);
      }
    }

    // Comandos de movimento
    if ((data_by_serial == 'a') || (data_by_serial == 'd') ||
        (data_by_serial == 'w') || (data_by_serial == 's') ||
        (data_by_serial == 'q')) {
      Trata_serial();
    }
  }

  // Auto-parada após 400ms sem novo comando
  if ((estado_mov == 1) && ((millis() - tempo_mov) >= 400)) {
    aciona_motor_b(1, 0, 0);
    aciona_motor_b(2, 0, 0);
    estado_mov = 0;
    Serial.println("[AUTO] Motores parados (timeout 400ms)");
  }

  // ===== MÁQUINA DE ESTADOS DA ARMA (via sensor OU comando 't') =====

  // Também verifica o sensor físico normalmente
  if ((digitalRead(sensor) == 0) && (estado == 0)) {
    estado = 1;
    tempo_arma = millis();
    myservo_1.write(0);
    myservo_2.write(180);
    Serial.println("[ARMA] Sensor acionado! Acionando...");
    analogWrite(LED_R_1, 255);
    analogWrite(LED_G_1, 0);
    analogWrite(LED_B_1, 0);
  }

  // Finaliza acionamento da arma (350ms)
  if ((estado == 1) && ((millis() - tempo_arma) >= 350)) {
    estado = 2;
    tempo_arma = millis();
    Serial.println("[ARMA] Acionamento completo. Aguardando recolhimento (12.5s)...");
  }

  // Inicia recolhimento da arma (12500ms)
  if ((estado == 2) && ((millis() - tempo_arma) >= 12500)) {
    estado = 3;
    tempo_arma = millis();
    myservo_1.write(180);
    myservo_2.write(0);
    Serial.println("[ARMA] Recolhendo...");
  }

  // Finaliza recolhimento da arma (400ms)
  if ((estado == 3) && ((millis() - tempo_arma) >= 400)) {
    estado = 0;
    analogWrite(LED_R_1, 255);
    analogWrite(LED_G_1, 255);
    analogWrite(LED_B_1, 255);
    Serial.println("[ARMA] Recolhida! Pronta novamente.");
    Serial.println();
  }
}