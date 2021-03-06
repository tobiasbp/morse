// The morse code will be transmitted on this pin
#define MORSE_PIN 13

// Length of events in morse code in milliseconds
// Base unit is dot
// https://da.wikipedia.org/wiki/Morsealfabet
// Dash is 3 * dot
// Pause between dash and dots is length of dot
// Pause between characters is length dash
// Pause between words is 7 * dot

const uint16_t time_dot = 100;
const uint16_t time_dash = 3 * time_dot;
const uint16_t time_dash_dot_delimiter = time_dot;
const uint16_t time_char_delimiter = time_dash;
const uint16_t time_word_delimiter = 7 * time_dot;



// 10 characters, 5 dash/dots long
// 1 = dot, 2 = dash, 0 = none
uint8_t morse_code[][5] = {
  {1,2,0,0,0}, // A
  {2,1,1,1,0}, // B
  {1,2,1,2,0}, // C
  {2,1,1,0,0}, // D
  {1,0,0,0,0}, // E
  {1,1,2,1,0}, // F
  {2,2,1,0,0}, // G
  {1,1,1,1,0}, // H
  {1,1,0,0,0}, // I
  {2,1,1,1,0}, // J
  {2,1,2,0,0}, // K
  {1,2,1,1,0}, // L
  {2,1,0,0,0}, // N
  {2,2,0,0,0}, // M
  {2,2,2,0,0}, // O
  {1,2,2,1,0}, // P
  {2,2,1,2,0}, // Q
  {1,2,1,0,0}, // R
  {1,1,1,0,0}, // S
  {2,0,0,0,0}, // T
  {1,1,2,0,0}, // U
  {1,1,1,2,0}, // V
  {1,2,2,0,0}, // W
  {2,1,1,2,0}, // X
  {2,1,2,2,0}, // Y
  {2,2,1,1,0}, // Z
};

// The message to send
uint8_t message[] = {'S','O','S'};


void setup() {
  // put your setup code here, to run once:
  pinMode(MORSE_LED, OUTPUT);
  
  // Enable serial communication
  Serial.begin(9600);
}

void loop() {
  // Loop through the characters in the message array
  // Note: sizeof returns number of bytes, thus it works
  // here because message uses type uint8_t (Size is 1 byte)
  for (uint8_t i = 0; i < sizeof(message); i++) {
    morse(message[i]);
  }
  // Pause between words
  // Char delimiter time allready sent in morse function, so subtract it from word pause.
  delay(time_word_delimiter - time_char_delimiter);
}


// Takes an ASCII code. Sends as morse code
// Only sends the characters A-Z & a-z
// FIXME: We need to accept a 'space' (ASCII 32)
void morse(int8_t char_to_send) {
  
  // Convert lower case to upper case
  if (char_to_send >= 'a' and char_to_send <= 'z') {
    Serial.print("Convert to upper case: ");
    Serial.print(char_to_send);
    char_to_send -= 32;
    Serial.print(" -> ");
    Serial.println(char_to_send);
  }

  // Send if upper case character
  if (!(char_to_send >= 'A' and char_to_send <= 'Z')) {
    Serial.print("Ignoring invalid character: ");
    Serial.println(char_to_send);
  } else {

    Serial.print("Sending ASCII char: ");
    Serial.println(char_to_send);
    
    // Convert from ASCII to list index
    // A is index 0
    char_to_send = char_to_send - 65;
    
    // Loop trough data for character
    for (uint8_t i = 0; i < 5; i++) {
      if (morse_code[char_to_send][i] == 0) {
        break;
      } else {
        if (morse_code[char_to_send][i] == 1) {
          flash(time_dot);
        } else {
          flash(time_dash);
        }
        // Pause between dashes/dots 
        delay(time_dash_dot_delimiter);   
      }
    } // End data loop
    
    // Pause between characters. dash/dot pause already sent, so subtract one
    delay(time_char_delimiter - time_dash_dot_delimiter);
  }
}

// Turn on morse pin for a period of flash_time milli seconds
void flash(int flash_time) {
  digitalWrite(MORSE_PIN, HIGH);
  delay(flash_time);
  digitalWrite(MORSE_PIN, LOW);
}

