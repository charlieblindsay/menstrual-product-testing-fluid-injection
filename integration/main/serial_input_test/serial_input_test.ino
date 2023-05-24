void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

    Serial.println('Enter 1 for light flow and 2 for heavy flow:');
    while(Serial.available() > 0){
      char user_input = Serial.read();
      Serial.println(user_input);

    }
}
