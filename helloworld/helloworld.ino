void setup() {
	//Create Serial Obj
	Serial.begin(9600); //115200, 38400, etc
}

void loop() {
	//have the arduino wait for input
	while(Serial.available() == 0);
	  
	//Read the input
	char val = char(Serial.read());
	  
	//Echo the input
	Serial.println(val);
}
