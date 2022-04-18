#include <Servo.h>
#define marcha0 3
#define marcha1 4
#define marcha2 5
#define marcha3 6
#define marcha4 7
#define marcha5 8
#define marcha6 9
#define acPin A2
#define frPin A1
#define emPin A0
#define SERVO 10 // Porta Digital 10 PWM
#define buzzerPin 11
Servo servo;

class CaixaDeMarchas
{
    int marcha = 0;
  public:
    int getMarcha() {
      return this->marcha;
    }

    void setMarcha(int marcha) {
      this->marcha = marcha;
    }

};
CaixaDeMarchas caixaDeMarchas;


class Pedais
{
    int acelerador = 0;
    int freio = 0;
    int embreagem = 0;
    int embreagemHistorico;

  public:
    int getAcelerador() {
      return this->acelerador;
    }

    void setAcelerador(int acelerador) {
      this->acelerador = acelerador;
    }

    int getFreio() {
      return this->freio;
    }

    void setFreio(int freio) {
      this->freio = freio;
    }

    int getEmbreagem() {
      return this->embreagem;
    }

    void setEmbreagem(int embreagem) {
      this->embreagem = embreagem;
    }
    void setEmbreagemHistorico(int tempo) {
      this->embreagemHistorico = tempo;
    }
    int getEmbreagemHistorico() {
      return this->embreagemHistorico;
    }
    int getCalcAceleracao() {
      return (this->getAcelerador() - this->getFreio());
    }
};
Pedais pedais;


class Motor
{
    bool ligado = true;
    int velocidade = 0;
    bool check = true;
    int tempoMarcha0;

  public:
    bool getLigado() {
      return ligado;
    }

    void isLigado(bool booleano) {
      if (booleano) {
        this->ligado = true;
        return;
      }
      if (this->velocidade > 0) {
        while (this->velocidade != 0) {
          this->velocidade--;
          delay(200);
        }
      } else {
        while (this->velocidade < 0) {
          this->velocidade++;
          delay(200);
        }
      }
      if (pedais.getEmbreagem() < 500) {
        this->ligado = false;
      }
      Serial.println("morri");
    }

    int getVelocidade() {
      return this->velocidade;
    }
    void setVelocidade() {
      int delayAceleracao = 0;
      if (pedais.getCalcAceleracao() >= 0) {
        delayAceleracao = 1000 - (pedais.getCalcAceleracao()) / 5 - this->getVelocidade();
      } else {
        delayAceleracao = 1000 - (this->getVelocidade() - pedais.getCalcAceleracao() / 5);
      }
      //Serial.println(delayAceleracao);
      switch (caixaDeMarchas.getMarcha()) {
        case 0:
          if (this->getVelocidade() < 0) {
            this->velocidade++;
            delay(500);
          } else if (getVelocidade() > 0) {
            this->velocidade--;
            delay(500);
          }
          break;
        case 1:
          if (this->getVelocidade() >= 0 && this->getVelocidade() <= 9) {
            if ((this->getVelocidade() - (1023 - pedais.getEmbreagem()) / 113.666666666) < -1) {
              this->isLigado(false);
            }
            if (this->getVelocidade() < (9 - (pedais.getEmbreagem() / 113.666666666))) {
              this->velocidade++;
              delay(delayAceleracao);
            }
          }
          if (this->getVelocidade() >= 9 && this->getVelocidade() <= 18) {
            if (this->getVelocidade() > 9 + (pedais.getCalcAceleracao() / 113.666666666)) {
              this ->velocidade--;
              delay(delayAceleracao);
            } else if (this->getVelocidade() < 9 + (pedais.getCalcAceleracao() / 113.666666666)) {
              this ->velocidade++;
              delay(delayAceleracao);
            }
          }
          if (pedais.getEmbreagem() < 500) {
            if (this->getVelocidade() > 18) {
              this->velocidade--;
              delay(50 + pedais.getEmbreagem());
            }
          }
          break;
        case 2:
          if (pedais.getEmbreagem() < 50) {
            if (this->getVelocidade() < 9) {
              this->isLigado(false);
            } else if (this->getVelocidade() > 27) {
              this->velocidade--;
              delay(50);
            }
            if (this->getVelocidade() >= 9 && this->getVelocidade() < 27) {
              int velocidadeDesejada = (pedais.getCalcAceleracao() / 56.83) + 9;
              if (velocidadeDesejada > this->getVelocidade()) {
                this->velocidade++;
                delay(delayAceleracao);
              } else if (velocidadeDesejada < this->getVelocidade()) {
                this->velocidade--;
                delay(delayAceleracao);
              }
            }
          }
          break;
        case 3:
          if (pedais.getEmbreagem() < 50) {
            if (this->getVelocidade() < 14) {
              this->isLigado(false);
            } else if (this->getVelocidade() > 45) {
              this->velocidade--;
              delay(50);
            }
            if (this->getVelocidade() >= 14 && this->getVelocidade() <= 45) {
              int velocidadeDesejada = (pedais.getCalcAceleracao() / 56.83) + 27;
              if (velocidadeDesejada > this->getVelocidade()) {
                this->velocidade++;
                this->getVelocidade() < 27 ?delay(600):delay(delayAceleracao);
              } else if (velocidadeDesejada < this->getVelocidade()) {
                this->velocidade--;
                delay(delayAceleracao);
              }
            }
          }
          break;
        case 4:
          if (pedais.getEmbreagem() < 50) {
            if (this->getVelocidade() < 27) {
              this->isLigado(false);
            } else if (this->getVelocidade() > 63) {
              this->velocidade--;
              delay(50);
            }
            if (this->getVelocidade() >= 27 && this->getVelocidade() < 63) {
              int velocidadeDesejada = (pedais.getCalcAceleracao() / 56.83) + 45;
              if (velocidadeDesejada > this->getVelocidade()) {
                this->velocidade++;
                this->getVelocidade() < 45?delay(600):delay(delayAceleracao);
              } else if (velocidadeDesejada < this->getVelocidade()) {
                this->velocidade--;
                delay(delayAceleracao);
              }
            }
          }
          break;
        case 5:
          if (pedais.getEmbreagem() < 50) {
            if (this->getVelocidade() < 35) {
              this->isLigado(false);
            }
            if (this->getVelocidade() >= 35 && this->getVelocidade() <= 80) {
              int velocidadeDesejada = (pedais.getCalcAceleracao() / 37.88) + 63;
              if (velocidadeDesejada > this->getVelocidade()) {
                this->velocidade++;
                this->getVelocidade() < 63? delay(600): delay(delayAceleracao);
              } else if (velocidadeDesejada < this->getVelocidade()) {
                this->velocidade--;
                delay(delayAceleracao);
              }
            }
          }
          break;
        case 6:
          //Serial.println("Marcha6");
          break;
      }
    }
};
Motor motor;


void setup()
{
  servo.attach(SERVO);
  Serial.begin(9600);
  pinMode(marcha0, INPUT);
  pinMode(marcha1, INPUT);
  pinMode(marcha2, INPUT);
  pinMode(marcha3, INPUT);
  pinMode(marcha4, INPUT);
  pinMode(marcha5, INPUT);
  pinMode(marcha6, INPUT);
  pinMode(acPin, INPUT);
  pinMode(frPin, INPUT);
  pinMode(emPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}
const int buzzer = 11;
void loop()
{
  if (motor.getLigado()) {
    //monitorando as trocas de marchas:
    if (digitalRead(marcha0) && pedais.getEmbreagem() > 1000) {
      caixaDeMarchas.setMarcha(0);
    }
    if (digitalRead(marcha1) && pedais.getEmbreagem() > 1000) {
      caixaDeMarchas.setMarcha(1);
    }
    if (digitalRead(marcha2) && pedais.getEmbreagem() > 900) {
      caixaDeMarchas.setMarcha(2);
    }
    if (digitalRead(marcha3) && pedais.getEmbreagem() > 800) {
      caixaDeMarchas.setMarcha(3);
    }
    if (digitalRead(marcha4) && pedais.getEmbreagem() > 700) {
      caixaDeMarchas.setMarcha(4);
    }
    if (digitalRead(marcha5) && pedais.getEmbreagem() > 600) {
      caixaDeMarchas.setMarcha(5);
    }
    if (digitalRead(marcha6) && pedais.getEmbreagem() > 1000) {
      caixaDeMarchas.setMarcha(6);
    }

    //monitorando os pedais:
    pedais.setAcelerador(analogRead(A2));
    pedais.setFreio(analogRead(A1));
    if (!(digitalRead(marcha0) && digitalRead(marcha1) && digitalRead(marcha2) && digitalRead(marcha3) && digitalRead(marcha4) && digitalRead(marcha5) && digitalRead(marcha6))) {
      pedais.setEmbreagem(analogRead(A0));
    }
    motor.setVelocidade();
    servo.write(80 - motor.getVelocidade());

    Serial.print("velocidade = ");
    Serial.println(motor.getVelocidade());
    Serial.print("marcha = ");
    Serial.print(caixaDeMarchas.getMarcha());
    Serial.println("\n");
    Serial.println(motor.getLigado());
    //delay(2000);*/

    //tone(buzzer, motor.getVelocidade(), 50);

  }
}
