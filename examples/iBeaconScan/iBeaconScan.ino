//Target iBeacon address is 0CF3EE093DA4 in this sketch. See below. **Change this number below to match the address of your own beacon**

unsigned long timestamp; //needed for timing how fast scan results are received

int rssi; // integer of the rssi value

void setup() 
{
	Serial.begin(9600);  // Begin the Serial Monitor connection at 9600bps
	Serial1.begin(9600);  // Begin the BlueDuino connection at 9600bps
	Serial1.setTimeout(900);  // length of time for BlueDuino serial to wait for BlueDuino.find to complete. You can play around with this number to see what gives the best results, i.e. fastest scans.
	Serial1.write("AT+SCAN2"); // ask BlueDuino to scan for bluetooth LE 4.0 fob
	timestamp = millis(); //sets the timestamp equal to the current millis value
}

void loop() 
{ 

	if (Serial1.available()) // If there is some data received from the BlueDuino, then do what follows in brackets
	{
		if (Serial1.find("**0CF3EE093DA4**")) // Scan the incoming message from BlueDuino to see if it contains the target string; if it does, then parse the integer following it and print the rssi value
		{
			int rssi = Serial1.parseInt();
			Serial.print("The RSSI of the target bluetooth LE 4.0 fob is: "); 
			Serial.println(rssi);
			Serial.print("Milliseconds elapsed since last string: ");
			Serial.println(millis() - timestamp); // amount of milliseconds elapsed since that line of info received
			timestamp = millis(); // reset the countdown timer
		}
		/*
		   else 
		   {
		// Timeout occurred waiting for .find()
		Serial.println("Timeout occurred"); 
		}
		 */
	}

	if (Serial.available()) // this allows you to type commands into the serial monitor while all this is running, for example, you can stop the scanning by typing AT+SCAN0
	{
		Serial1.write(Serial.read());
	}
}
