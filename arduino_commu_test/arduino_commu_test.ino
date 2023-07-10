const int LED = 5;
const int SWL = 18; //Right Request
const int SWR = 19; //fire Request

typedef struct Recvcom {    // size 9
    int x; // two char , IMU x
    int y; // two char , IMU y
    uint8_t start;  // 마지막 데이터 확인용
};

typedef struct Sendcom {
    unsigned int x; // two char , IMU x
    unsigned int y; // two char , IMU y
    uint8_t swL; // Right Request
    uint8_t swR; // fire Request
    uint8_t start; // 마지막 데이터 확인용
};

unsigned char Sendbuf[11] = {NULL,};
unsigned char Recvbuf[9] = {NULL,};
Sendcom sendcom;
Recvcom recvcom;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode (SWL, INPUT);
  pinMode (SWR, INPUT);
  Serial.begin(115200);
  digitalWrite(LED, LOW);
  delay(1000);
  sendcom.start = 30;
  sendcom.x = 0;
  sendcom.y = 0;
  sendcom.swL = 0;
  sendcom.swR = 0;
}

void loop() {
  // Recving example
  Serial.readBytes(Recvbuf, 9);
  if (Recvbuf[8] == 10) {
    memcpy(&recvcom, Recvbuf, sizeof(Recvbuf));
    if (recvcom.x  > 50 ) {
      digitalWrite(LED, HIGH);
    }
    else {
      digitalWrite(LED, LOW);
    }

    if (recvcom.y  > 50 ) {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  //Sending example
  if (digitalRead(SWL)) sendcom.swL = 1;
  else sendcom.swL = 0;

  if (digitalRead(SWR)) sendcom.swR = 1;
  else sendcom.swR = 0;

  sendcom.x = random(0, 9);
  sendcom.y = random(0, 9);

  memcpy(Sendbuf, &sendcom, sizeof(Sendcom));
  
  Serial.write(Sendbuf, sizeof(Sendbuf));
  delay(10);
}


