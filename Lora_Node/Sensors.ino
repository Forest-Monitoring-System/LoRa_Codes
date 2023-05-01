//Smoke Detection Code.....
#define SMOKE_THRESHOLD 120
#define SMOKE_PIN A0

bool isSmokeDetected() {
  bool smokeDetected = false;
  int smokeVal = analogRead(SMOKE_PIN);
  if (smokeVal > SMOKE_THRESHOLD) {
    smokeDetected = true;
  }
  return smokeDetected;
}
////////////////////////////////////////

//Flame Sensor Code.....
#define FLAME_THRESHOLD 900
int flameCount() {
  int count = 0;
  if (analogRead(A1) > FLAME_THRESHOLD) count++;
  if (analogRead(A2) > FLAME_THRESHOLD) count++;
  if (analogRead(A3) > FLAME_THRESHOLD) count++;
  if (analogRead(A4) > FLAME_THRESHOLD) count++;
  if (analogRead(A5) > FLAME_THRESHOLD) count++;
  return count;
}
//////////////////////////////////////////////
