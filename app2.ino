
//Variable declaration:
String red = "{\"on\":true,\"bri\": 254,\"hue\": 65527,\"sat\": 253,\"effect\": \"none\",\"xy\": [0.6736,0.3221]}";
String white = "{\"on\": true,\"bri\": 254,\"hue\": 14910,\"sat\": 144,\"effect\": \"none\",\"xy\": [0.4596,0.4105]}";
int onoff = D3;   //ON/OFF button on frot of box
int button = D4;  //Main switch on top of box
int ledGreen = D5;//GREEN LED on box
int ledRed = D6;  //Red LED on box
int ledBlue = D7; // Blue LED on particle
int light = 0;
int val = 0;

//TCP protocol
TCPClient client;
byte server[] = { 192, 168, 2, 1 }; // Local light server
unsigned int len; // length of val

void setup() {
    pinMode(button, INPUT);
    pinMode(onoff, INPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledRed, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    
    client.connect(server, 80);
    if (client.connected()){ //initiate connection
       Serial.println("connected");
    }else{
        Serial.println("failed");
    }
    /* GREEN ON AT STARTUP */
    digitalWrite(ledGreen,HIGH);
    
}

void loop() {
    val = digitalRead(button);//read the state of the switch
    // ON/OFF switch puts system to sleep
    if(digitalRead(onoff)==LOW){
        System.sleep(SLEEP_MODE_DEEP);
        digitalWrite(ledBlue, LOW);//BLUE LED OFF if particle asleep
    }else if(digitalRead(onoff)==HIGH){
        digitalWrite(ledBlue, HIGH);//BLUE LED ON if particle awake
    }
    if(val==HIGH&&light==1){//IF SWITCH IS OPEN
        digitalWrite(ledGreen,HIGH);//GREEN LED ON
        digitalWrite(ledRed,LOW);//RED LED OFF
        /* TURN SIRENS OFF*/
        sendCommand("false",#light);
        delay(100);
        sendCommand("false",#light);
        delay(100);
        sendCommand("false",#light);
        delay(100);
        
        /* TURN LIGHTS BACK ON*/
        turnOn(#light,white);
        turnOn(#light,white);
        delay(1000);
        light=0;
    } else if (val==LOW&&light==0) {//IF SWITCH IS CLOSED
        digitalWrite(ledGreen,LOW);//GREN LED OFF
        digitalWrite(ledRed,HIGH);//RED LED ON
        /* TURN LIGHTS OFF*/
        sendCommand("false",#light);
        sendCommand("false",#light);
        delay(150);
        sendCommand("false",#light);
        sendCommand("false",#light);
        delay(150);
        delay(1000);
        /* TURN ON SIREN*/
        for (int i=0;i<10;i++){
            turnOn(#light,red);
            delay(150);
            sendCommand("false",#light);
            delay(150);
            turnOn(#light,red);//couch far red
            delay(150);
            sendCommand("false",#light);
            delay(150);
            turnOn(#light,red);
            delay(150);
            sendCommand("false",#light);
            delay(150);
        }
        light=1;
    }
    delay(100);
}
//sendCommand boolen, light#
void sendCommand(String state,int lightNumber){
    if (client.connect(server, 80)) {
        if(client.connected()){
            unsigned int len = state.length(); //get length
            client.print("PUT /api/APIkeyGOEShere/lights/");
            client.print(lightNumber);
            client.print("/state HTTP/1.1");
            client.println("Connection: keep-alive"); //
            client.println("Host: 192.168.2.1"); //same as server
            client.println("Content-Type: text/plain;charset=UTF-8"); //
            client.print("Content-Length: "); //param
            client.println(10+len); //brightness string + val length
            client.println();  // blank line before body
            client.print("{\"on\": ");
            client.print(state); //value of potentiometer
            client.println("}");
            Serial.print("sent");  // command executed
            delay(100); // slight delay IMPORTANT
        }
        client.stop();
    }
}
//TURN on light#, color
void turnOn(int lightNumber, String color){
    if (client.connect(server, 80)) {
        if(client.connected()){
            unsigned int len = color.length(); //get length
            client.print("PUT /api/APIkeyGOEShere/lights/");
            client.print(lightNumber);
            client.print("/state HTTP/1.1");
            client.println("Connection: keep-alive"); //
            client.println("Host: 192.168.2.18"); //same as server
            client.println("Content-Type: text/plain;charset=UTF-8"); //
            client.print("Content-Length: "); //param
            client.println(10+len); //brightness string + val length
            client.println();  // blank line before body
            client.println(color);
            Serial.print("sent");  // command executed
            delay(100); // slight delay IMPORTANT
        }
        client.stop();
    }
}
