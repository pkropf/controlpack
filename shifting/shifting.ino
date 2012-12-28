void setup()
{
  Serial.begin(9600);
  Serial.println("hello");
}


void loop()
{
  uint16_t x = 0x0110;
  uint8_t u = x >> 8;
  uint8_t l = (x << 8) >> 8;

  Serial.println(x);
  Serial.println(u);
  Serial.println(l);
  Serial.println("");
  delay(2000);
}

