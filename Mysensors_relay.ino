/**
 * MySensors_Grove_Relay_Actuator
 *
 *******************************
 *
 *
 * REVISION HISTORY
 * Version 1.0: GUILLOTON Laurent
 * Version 1.1 - 2017-07-03 : Création du sketch initial
 * Version 1.2 - 2017-11-07 : Modification des retours d'états
 *
 * DESCRIPTION
 *
 * Sketch gérant un relais (Grove) avec un lien NRF24L01 Mysensors v2.0
 * For more information, please visit:
 * http://wiki.seeed.cc/Grove-Relay/
 *
 */


// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Enable repeater functionality for this node
#define MY_REPEATER_FEATURE

#include "Arduino.h"
#include <MySensors.h>

#define PIN_RELAY  6  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
//#define NUMBER_OF_RELAYS 1 // Total number of attached relays
#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay

/* Bloc de création des Id et message Mysensors */
// ID des capteurs sur le noeud
#define CHILD_ID_STATE 1					// Identificateur du mode d'utilisation

// Message MySensors
MyMessage stateMsg(CHILD_ID_STATE, V_STATUS);

void setup()
{
	Serial.begin(115200);
	pinMode(PIN_RELAY, OUTPUT);
	// Set relay to last known state (using eeprom storage)
	digitalWrite(PIN_RELAY, loadState(CHILD_ID_STATE));
	send(stateMsg.set(loadState(CHILD_ID_STATE)));
	Serial.print("Etat initial du relais : ");
	Serial.println(loadState(CHILD_ID_STATE));
}

void presentation()
{
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("Mysensors_Grove_Relay_Actuator", "1.2");
	present(CHILD_ID_STATE, S_BINARY);
}


void loop()
{

}

void receive(const MyMessage &message)
{
	// We only expect one type of message from controller. But we better check anyway.
	if (message.type==V_STATUS) {
		if (message.sensor==CHILD_ID_STATE)
		{
			// Change relay state
			bool state = message.getBool();
			digitalWrite(PIN_RELAY, state);
			send(stateMsg.set(state));
			// Store state in eeprom
			saveState(CHILD_ID_STATE, state);
			Serial.print("Etat du relais : ");
			Serial.println(loadState(CHILD_ID_STATE));
		}
	}
}
