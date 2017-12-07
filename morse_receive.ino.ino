// Length of events in morse code in milliseconds
// Base unit is dot
// https://da.wikipedia.org/wiki/Morsealfabet
// Dash is 3 * dot
// Pause between dash and dots is length of dot
// Pause between characters is length dash
// Pause between words is 7 * dot

const uint8_t time_dot = 5;
const uint8_t time_dash = 3 * time_dot;
const uint8_t time_dash_dot_delimiter = time_dot;
const uint8_t time_char_delimiter = time_dash;
const uint8_t time_word_delimiter = 7 * time_dot;

// Signals are allowed to this much lower of 
const uint8_t difference_allowed = 2;

#define RADIO_PIN 3
#define LED_PIN 6

// The length of a period where signal was high or low
uint32_t period_length;

// Timestamp for state change
uint32_t timestamp_change;

// The state of the pin connected to the receiver
uint8_t radio_pin_state;

// The signals I need to decode
enum signal_type { dot, dash, char_delimiter, word_delimiter, invalid };

void setup() {
  // put your setup code here, to run once:

  // Read radio on this pin
  pinMode(RADIO_PIN, INPUT);
  
  // Show status on LED
  pinMode(LED_PIN, OUTPUT);
  
  // Enable serial communication
  Serial.begin(115200);

  timestamp_change = millis();
  //uint32_t period_length;

  radio_pin_state = digitalRead(RADIO_PIN);

}

void loop() {
  
  // Wait for a pin change
  while (digitalRead(RADIO_PIN) == radio_pin_state){};

  // Length of previous period
  period_length = millis() - timestamp_change;

  // The time the state changed (Now) 
  timestamp_change = millis();

  // Decode the previous period
  switch (decodeSignal(period_length, radio_pin_state)) {
    case dot:
      Serial.print(".");
      // FIXME: Add to buffer
      break;
    case dash:
      Serial.print("-");
      // FIXME: Add to buffer
      break;
    case char_delimiter:
      Serial.print(" ");
      // FIXME: Match the character
      break;
    case word_delimiter:
      Serial.println();
      break;
    default:
      // FIX ME: Do something? What? 
      break;
  }

  // Flip the state
  radio_pin_state = !radio_pin_state;
 
  // Show state on LED
  digitalWrite(LED_PIN, radio_pin_state);

}

// Decode signal
// signal should be more bits
enum signal_type decodeSignal(uint8_t length, uint8_t level) {
  if (level == 1) { // Decode high signal
    if (period_length >= (time_dot -1) && period_length <= (time_dot + 1)) {
      return dot;
    } else if (period_length >= (time_dash -1) && period_length <= (time_dash + 1)) {
      return dash;
    }
  } else { // Decode low signals
    if (period_length >= (time_char_delimiter -1) && period_length <= (time_char_delimiter + 1)) {
      return char_delimiter;
    } else if (period_length >= (time_word_delimiter -1) && period_length <= (time_word_delimiter + 1)) {
      return word_delimiter;
    }
  }
  // Did not match anything.
  return invalid;
}

