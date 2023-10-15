#include <stdbool.h>
#include <string.h>
#include <Wire.h>
/*
 * relayfish: A simple relay+arduino way to toggle the bias-t power to force
 * retuning of my LDG RT-100 remote tuner. You could use it for other things
 * too
 */

#define	MAX_RELAYS	24
bool pin_states[MAX_RELAYS];	// state of each relay
char buf[512];
size_t	buf_p_parse;		// parse pointer
size_t	buf_p_read;		// read pointer

void setup() {
    // Setup the serial port
    Serial.begin(9600);		// open sercons0 at 8n1@9600

    // Print a banner
    Serial.print("*************\n");
    Serial.print("* relayfish *\n");
    Serial.print("*************\n");

    // Initialize the io pin as an output, pulled down
    Serial.print("* io init\n");

    // Clear out our buffer
    memset(buf, 0, sizeof(buf));

    Serial.print("READY!\n");
}

bool get_val(int relay) {
   if (relay < 0 || relay > MAX_RELAYS) {
      Serial.print("-ERR: Relay id (");
      Serial.print(relay, DEC);
      Serial.print(" out of range (0-");
      Serial.print(MAX_RELAYS, DEC);
      Serial.print("\n");
      return false;
   }
   return pin_states[relay];
}

bool set_val(int relay, bool val) {
   if (relay < 0 || relay > MAX_RELAYS) {
      Serial.print("-ERR: Relay id (");
      Serial.print(relay, DEC);
      Serial.print(" out of range (0-");
      Serial.print(MAX_RELAYS, DEC);
      Serial.print("\n");
      return false;
   }

   pin_states[relay] = val;
   return pin_states[relay];
}

void send_help() {
    Serial.print("*************\n");
    Serial.print("* relayfish *\n");
    Serial.print("*************\n");
    Serial.print("\n");
    Serial.print("g\t\tGet      [Gg]\n");
    Serial.print("h\t\tHelp     [Hh]\n");
    Serial.print("n\t\tNo (off) [Nn0]\n");
    Serial.print("y\t\tYes (on) [Yy1]\n");
    Serial.print("x\t\tToggle   [TtXx]\n");
}

void loop() {
    char cmd = ' ';
    int relay = -1;
    int rv = -1;
    bool val;
    int tval = -1;

    // Read the serial data, if available and add to end of buffer
    if (Serial.available() > 0) {
    }

    // Parse into cmd and relay
    //
    char *cp = NULL, *vp = NULL;
    cp = buf;
    // if we got a command tpr, look for a value (relay selection)
    if (cp != 0) {
       vp = strchr(cp, ' ');
    }

    if (vp != NULL) {
       relay = atoi(vp + 1);
       vp = strchr(vp, ' ');
       if (vp != NULL) {
          tval = atoi(vp + 1);
          if (tval == 1) {
             val = true;
          } else if (tval == 0) {
             val = false;
          } else if (tval == -1) {
             Serial.print("-ERR: Unable to parse relay #\n");
          }
       }
    } else {
       Serial.print("-ERR: No relay # given\n");
       return;
    }

    // Switch on the command read
    switch(cmd) {
        // 'G' or 'g' for get state
        case 'G': case 'g':
           Serial.print("OK ");
           Serial.print(relay, DEC);
           Serial.print(" ");
           Serial.print(get_val(relay), DEC);
           Serial.print("\n");
           break;
        // 'H' or 'h' for help
        case 'H': case 'h':
           send_help();
           break;
        // 'N' to set to off and return new state
        case 'N': case 'n': case '0':
           rv = set_val(relay, false);
           break;
        // 'Y' to set to on and return new state
        case 'Y': case 'y': case '1':
           rv = set_val(relay, true);
           break;

        // 'X' to toggle and return new state
        case 'X': case 'x': case 'T': case 't':
           rv = set_val(relay, !get_val(relay));
           break;

        // Invalid command, send help
        default:
           Serial.print("Invalid command ");
           Serial.print(cmd);
           Serial.print(" ignored!\n\n");
           send_help();
           break;
    }
    
    // Was there an error?
    if (rv != -1) {
       Serial.print("-ERR: rv: ");
       Serial.print(rv, DEC);
       Serial.print("\n");
    } else {
       Serial.print("OK ");
       Serial.print(relay, DEC);
       Serial.print(" ");
       Serial.print(rv, DEC);
       Serial.print("\n");
    }
}
