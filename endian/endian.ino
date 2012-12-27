void setup()
{
  int x = 0;

  Serial.begin(9600);
  Serial.println("hello");

  Serial.println(x);
  x = 1 << 8;
  Serial.println(x);
  x = (0x03 << 8) + 0xe8;
  Serial.println(x);
  if (x == 1000) {
    Serial.println("yea");
  } else {
    Serial.println("oy");
  }
}

void loop()
{
}

