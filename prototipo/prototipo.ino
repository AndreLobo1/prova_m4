#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

// Pinagem dos leds
#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

// Botão
const int pinoBotao = 18;  // Pino do botão
int estadoBotao = 0;  // Variável para leitura do status do botão (pressionado ou não)

// LDR
const int pinoLDR = 4;  // Pino do LDR
int limiteLDR = 600; // Limite do LDR

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);  // Indicando como saida
  pinMode(led_verde,OUTPUT);    // Indicando como saida
  pinMode(led_vermelho,OUTPUT); // Indicando como saida

  // Inicialização das entradas
  pinMode(pinoBotao, INPUT);    // Indicando como entrada

  digitalWrite(led_amarelo, LOW);  // Apagado na inicialização
  digitalWrite(led_verde, LOW);    // Apagado na inicialização
  digitalWrite(led_vermelho, LOW); // Apagado na inicialização

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  // Conexão com a internet
  const String SSID = "Wokwi-GUEST";
  const String PASSWORD = "";
  WiFiClient espClient;
  PubSubClient client(espClient);

  // Verifica estado do botão
  estadoBotao = digitalRead(pinoBotao);
  if (estadoBotao == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int codigoHttpErro = http.GET(); // Código do Resultado da Requisição HTTP

    if (codigoHttpErro>0) {
      Serial.print("Resposta HTTP: ");
      Serial.println(codigoHttpErro);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Código do erro: ");
      Serial.println(codigoHttpErro);
      }
      http.end();
    }

  else {
    Serial.println("WiFi desconectado");
  }
}

void loop() {
  int ldrstatus=analogRead(pinoLDR);

  if(ldrstatus>=limiteLDR){ // Lógica para ativar modo noturno se estiver escuro
    Serial.print("Modo escuro ativado. led amarelo piscando!");
    delay(1000); // Lógica para piscar a cada 1 segundo
    digitalWrite(led_amarelo, HIGH);  // Acende o LED amarelo
    delay(1000); // Lógica para piscar a cada 1 segundo
    digitalWrite(led_amarelo, LOW);  // Apaga o LED amarelo

  }else{ // Lógica para ativar fluxo padrão se estiver claro
    Serial.print("Está claro. Fluxo padrão rodando!");
    digitalWrite(led_verde, HIGH);  // Acende o LED verde
    delay(3000); // Lógica para alternar o led do semaforo
    digitalWrite(led_verde, LOW);  // Apaga o LED verde
    digitalWrite(led_amarelo, HIGH);  // Acende o LED amarelo
    delay(2000); // Lógica para alternar o led do semaforo
    digitalWrite(led_amarelo, LOW);  // Apaga o LED amarelo
    digitalWrite(led_vermelho, HIGH);  // Acende o LED vermelho
    delay(5000); // Lógica para alternar o led do semaforo
    digitalWrite(led_vermelho, LOW);  // Apaga o LED vermelho
  }
}
