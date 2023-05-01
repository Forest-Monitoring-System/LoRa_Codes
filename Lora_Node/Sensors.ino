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
