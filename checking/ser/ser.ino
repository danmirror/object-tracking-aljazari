
String container;

int found;
float x,y;
String y_str,x_str;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

   if (Serial.available() > 0) {
    while (Serial.available()) {
      
       char data = (char)Serial.read() ;
       container +=data;
    }
    int indexing=0;
    
     //===========parsing data=============
    for(int i=0; i<container.length();i++){
      if(container[i]==','){
        indexing = indexing +1;
      }
      else{
        if(indexing==0){
          found = container[i]- '0';
        }
        if(indexing==1){
           x_str += container[i];
        }
        if(indexing==2){
           y_str += container[i];
        }
      }
     }
     x = x_str.toFloat();
     y = y_str.toFloat();
   }
   
   delay(500);
   Serial.print("d => ");//
   Serial.print(found);
   Serial.print("\t x => ");
   Serial.print(x);
   Serial.print("\t y => ");
   Serial.println(y);
   //reset
    container = "";
    y_str = "";
    x_str = "";
  
   if(found == 1){
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);
    delay(500);
   }
   if(found == 2){
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
   }

}
