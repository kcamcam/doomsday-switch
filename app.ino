TCPClient client;
byte server[] = { 192, 168, 1, 2 }; // Local light server
unsigned int len; // length of val
int inPin = D0;  // button  // led
//int outPin = D5;
int statusLight = 0;
/* Button and led logics */
int buttonState=0;

void setup() {
    Serial.begin(9600);
    delay(500);
    pinMode(inPin, INPUT);    // button pin setup
    //pinMode(outPin, OUTPUT);
    //digitalWrite(outPin, LOW);
    client.connect(server, 80);
    if (client.connected()){ //initiate connection
       Serial.println("connected");
    }else{
        Serial.println("failed");
    }
    
}

void loop(){
    // read the state of the pushbutton value:

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
    buttonState = digitalRead(inPin);
    if (buttonState == HIGH && statusLight==0) {
        //turn on
        statusLight=1;
        //digitalWrite(outPin, statusLight);
        sendCommand("true");
    } else if (buttonState==LOW && statusLight==1){
        //turn off
        statusLight=0;
        //digitalWrite(outPin, statusLight);
        sendCommand("false");
    }
    delay(200);
}

void sendCommand(String state){
    if (client.connect(server, 80)) {
        if(client.connected()){
            unsigned int len = state.length(); //get length
            client.println("PUT /api/{{api key here}}/lights/1/state HTTP/1.1");
            client.println("Connection: keep-alive"); //
            client.println("Host: 192.168.1.2"); //same as server
            client.println("Content-Type: text/plain;charset=UTF-8"); //
            client.print("Content-Length: "); //param
            client.println(10+len); //brightness string + val length
            client.println();  // blank line before body
            client.print("{\"on\": ");
            client.print(state); //value of potentiometer
            client.println("}");
            Serial.print("sent");  // command executed
            delay(100); // slight delay IMPORTANt
        }
        client.stop();
        Serial.println("stopping");
    }
}
