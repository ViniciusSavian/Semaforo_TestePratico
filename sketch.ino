#include <WiFi.h>
#include <HTTPClient.h>

int led_verde = 41; //Led Verde conectado na porta 41
int led_vermelho = 40; //Led Vermelho conectado na porta 40
int led_amarelo = 9; //Led Amarelo conectado na porta 9

int botao = 18;  // Botao conectado na porta 18
int botao_status = 0;  //Variavel para a leitura do botao

int ldr = 4;  // LDR conectado na porta 4
int limite=600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(botao, INPUT);

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

void loop() {

  // Verifica estado do botão
  botao_status = digitalRead(botao);
  if (botao_status == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  int ldrstatus=analogRead(ldr);

  if(ldrstatus<=limite){
    Serial.print("Está escuro então acende o LED");
    Serial.println(ldrstatus);
    digitalWrite(led_amarelo, HIGH);
    delay(1000);// Mantém aceso por 1 segundos
    digitalWrite(led_amarelo, LOW);
    delay(1000);

  }else{
    Serial.print("Está claro então desliga o LED");
    Serial.println(ldrstatus);
    digitalWrite(led_verde, HIGH);
    delay(3000);// Mantém aceso por 3 segundos
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, HIGH);
    delay(2000);// Mantém aceso por 2 segundos
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_vermelho, HIGH);
    if (botao_status == HIGH){
      Serial.println("Botão pressionado!");
      delay(1000);// Fecha em 1 segundo
    }else{delay(5000);}// Mantém aceso por 5 segundos
    digitalWrite(led_vermelho, LOW);
  }
}