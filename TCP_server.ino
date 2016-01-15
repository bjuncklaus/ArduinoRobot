/**
 * @example TCPServer.ino
 * @brief The TCPServer demo of library WeeESP8266.
 * @author Wu Pengfei<pengfei.wu@itead.cc>
 * @date 2015.02
 *
 * @par Copyright:
 * Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ESP8266.h"
#include "SoftwareSerial.h"
#include <Servo.h>

//Celular
//#define SSID        "AndroidAP"
//#define PASSWORD    "aunh8022"
//#define HOST_NAME   "192.168.43.1"

//AP
#define SSID        "Juncklaus"
#define PASSWORD    "defenestrar"
#define HOST_NAME   "192.168.25.107"

#define HOST_PORT   (80)

//azul -> 3 e verde -> 2 = (2, 3)
SoftwareSerial minhaSerial(2, 3);

ESP8266 wifi(minhaSerial);

Servo servoY;
Servo servoX;

int pinEsquerdaFrente = 11;
int pinEsquerdaTras = 10;

int pinDireitaFrente = 5;
int pinDireitaTras = 6;

int posY = 90;
int posX = 90;
int microY = 1500;
int microX = 1500;

String carro_anterior = "";
String garra_anterior = "";
void setup()
{

  Serial.begin(9600);
  Serial.print("setup begin\r\n");

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());


  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.enableMUX()) {
    Serial.print("multiple ok\r\n");
  } else {
    Serial.print("multiple err\r\n");
  }

  if (wifi.startTCPServer(8090)) {
    Serial.print("start tcp server ok\r\n");
  } else {
    Serial.print("start tcp server err\r\n");
  }

  if (wifi.setTCPServerTimeout(5)) {
    Serial.print("set tcp server timout 10 seconds\r\n");
  } else {
    Serial.print("set tcp server timout err\r\n");
  }


  delay(150);
  //servoY.attach(9);
  servoY.attach(9, 1000, 2000);
  delay(150);
  //servoX.attach(8);
  servoX.attach(8, 1000, 2000);
  delay(15);

  pinMode(pinDireitaFrente, OUTPUT);
  delay(15);
  pinMode(pinEsquerdaFrente, OUTPUT);
  delay(15);

  Serial.print("setup end\r\n");
}

void loop()
{
  uint8_t buffer[128] = {0};
  uint8_t mux_id;
  uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 50);
  if (len > 0) {
    String garra = "";
    String carro = "";

    garra.concat((char)buffer[0]);
    garra.concat((char)buffer[1]);

    carro.concat((char)buffer[2]);
    carro.concat((char)buffer[3]);

    if (carro_anterior != carro || garra_anterior != garra) {
      garra_anterior = garra;
      carro_anterior = carro;

      delay(40);
    }

    Serial.println(garra);
    Serial.println(carro);
  }

  executaAcao(garra_anterior, carro_anterior);
  delay(40);
}

void executaAcao(String garra, String carro) {
  if (garra == "GC") {
    garraCima();
    return;
  }

  if (garra == "GB") {
    garraBaixo();
    return;
  }

  if (garra == "GA") {
    garraAbre();
    return;
  }

  if (garra == "GF") {
    garraFecha();
    return;
  }

  if (carro == "CF") {
    frente(50);
    return;
  }

  if (carro == "CT") {
    tras(50);
    return;
  }

  if (carro == "CE") {
    esquerda(150);
    return;
  }

  if (carro == "CD") {
    direita(150);
    return;
  }
}

void garraCima() {
  Serial.println("Garra Cima");
  microY--;
  servoY.writeMicroseconds(microY);
  Serial.print("Pos Y: ");
  Serial.println(posY);
}

void garraBaixo() {
  Serial.println("Garra Baixo");
  microY++;
  servoY.writeMicroseconds(microY);
  Serial.print("Pos Y: ");
  Serial.println(posY);
}

void garraAbre() {
  Serial.println("Garra Abre");
  microX--;
  servoX.writeMicroseconds(microX);
  Serial.print("Pos X: ");
  Serial.println(posX);
}

void garraFecha() {
  Serial.println("Garra Fecha");
  microX++;
  servoX.writeMicroseconds(microX);
  Serial.print("Pos X: ");
  Serial.println(posX);
}

void frente(int speed) {
  Serial.println("Carro Frente");
  digitalWrite(pinDireitaFrente, HIGH);
  delay(15);
  digitalWrite(pinEsquerdaFrente, HIGH);
  delay(15);

  analogWrite(pinDireitaFrente, speed);
  analogWrite(pinEsquerdaFrente, speed);
  delay(150);
  analogWrite(pinDireitaFrente, 0);
  analogWrite(pinEsquerdaFrente, 0);
}

void tras(int speed) {
  Serial.println("Carro Tras");
  digitalWrite(pinDireitaTras, HIGH);
  delay(15);
  digitalWrite(pinEsquerdaTras, HIGH);
  delay(15);

  analogWrite(pinDireitaTras, speed);
  delay(15);
  analogWrite(pinEsquerdaTras, speed);
  delay(150);
  analogWrite(pinDireitaTras, 0);
  analogWrite(pinEsquerdaTras, 0);
}

void esquerda(int speed) {
  Serial.println("Carro Esquerda");
  digitalWrite(pinDireitaFrente, HIGH);
  delay(15);
  digitalWrite(pinEsquerdaTras, HIGH);
  delay(15);

  analogWrite(pinDireitaFrente, speed);
  analogWrite(pinEsquerdaTras, speed);
  delay(150);
  analogWrite(pinDireitaFrente, 0);
  analogWrite(pinEsquerdaTras, 0);
}

void direita(int speed) {
  Serial.println("Carro Direita");
  digitalWrite(pinEsquerdaFrente, HIGH);
  delay(15);
  digitalWrite(pinDireitaTras, HIGH);
  delay(15);

  analogWrite(pinEsquerdaFrente, speed);
  analogWrite(pinDireitaTras, speed);
  delay(150);
  analogWrite(pinEsquerdaFrente, 0);
  analogWrite(pinDireitaTras, 0);
}


