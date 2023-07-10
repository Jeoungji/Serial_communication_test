const int LED = 5;
const int SWL = 18; //Right Request
const int SWR = 19; //fire Request

typedef struct Recvcom {    // size 48
    unsigned char right;
};

typedef struct Sendcom {
    unsigned int x; // two char , IMU x
    unsigned int y; // two char , IMU y
    unsigned char swL; // Right Request
    unsigned char swR; // fire Request
};

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode (SWL, INPUT);
  pinMode (SWR, INPUT);
  Serial.begin(115200);
  digitalWrite(LED, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}


unsigned char Sendbuf[6] = {NULL,};
unsigned char Recvbuf = NULL;
int Right = 0;
Sendcom sendcom{0,0,0,0};
Recvcom recvcom{0} ;

void loop() {
  if (Right) { 
    digitalWrite(LED, HIGH);
    if (digitalRead(SWR)) { // 발사
      sendcom.swR = 1;
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      sendcom.swR = 0;
      digitalWrite(LED_BUILTIN, LOW);
    }

    if (digitalRead(SWL)) {       // 권한이 있는 상태에서 스위치를 누르면 조작제어 추
      digitalWrite(LED, LOW);
      sendcom.swL = 0;
      sendcom.swR = 0;
      Right = 0;
      delay(1000);
    } 
  } else {
    if (digitalRead(SWL))
      sendcom.swL = 1;
    else 
      sendcom.swL = 0;
    digitalWrite(LED, LOW);
    sendcom.swR = 0;
  }

  sendcom.x = random(0, 360);
  sendcom.y = random(0, 360);

  for (int i = 0; i < 6; i++)
        Sendbuf[i] = NULL;
     
  Sendbuf[5] = sendcom.x / 254;
  Sendbuf[4] = sendcom.x % 254;
  Sendbuf[3] = sendcom.y / 254;
  Sendbuf[2] = sendcom.y % 254;
  if (sendcom.swL == 1 ||sendcom.swL == 0)
    Sendbuf[1] = sendcom.swL;
  if (sendcom.swR == 1 ||sendcom.swR == 0)
    Sendbuf[0] = sendcom.swR;
  String a = "      ";
  for (int i = 0; i < 6; i++) {
        Sendbuf[i]++; // add bias
        a.setCharAt(i,Sendbuf[i]);
  }
  Serial.print(a);
  delay(1000);
}

void serialEvent() {
  if (Serial.available()) {
    recvcom.right = (char)Serial.read();
    recvcom.right--; // delete bias
    Right = recvcom.right;
  }
}
