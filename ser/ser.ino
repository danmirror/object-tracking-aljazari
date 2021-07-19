int num=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

   if (Serial.available()) {
    while (Serial.available()) {
      
      char data = (char)Serial.read() ;
      num = data- '0';
      
    }
    Serial.println(num);
   }
   if(num == 1){
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);
    delay(500);
   }
   if(num == 2){
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
   }

}
