#define trigger_x 2
#define echo_x 3
#define trigger_y 4
#define echo_y 5
#define trigger_z 6
#define echo_z 7

const int bufferLen = 5;  // Moving average buffer size

double temp_dur;
float distance_x, distance_y, distance_z, temp_dist;

float buffer_x[bufferLen];
float buffer_y[bufferLen];
float buffer_z[bufferLen];

float total_x = 0;
float total_y = 0;
float total_z = 0;

int index_x = 0;
int index_y = 0;
int index_z = 0;

void setup() {

  pinMode(trigger_x, OUTPUT);
  pinMode(echo_x, INPUT);
  pinMode(trigger_y, OUTPUT);
  pinMode(echo_y, INPUT);
  pinMode(trigger_z, OUTPUT);
  pinMode(echo_z, INPUT);
  Serial.begin(9600);

  // Initialize buffers
  for (int i = 0; i < bufferLen; i++) {
    buffer_x[i] = getDistance(trigger_x, echo_x);
    buffer_y[i] = getDistance(trigger_y, echo_y);
    buffer_z[i] = getDistance(trigger_z, echo_z);

    total_x += buffer_x[i];
    total_y += buffer_y[i];
    total_z += buffer_z[i];
  }
}

void loop() {
  // Sensor x
  temp_dist = getDistance(trigger_x, echo_x);
  total_x = total_x - buffer_x[index_x] + temp_dist;
  buffer_x[index_x] = temp_dist;
  index_x = (index_x + 1) % bufferLen;
  distance_x = total_x / bufferLen;

  // Sensor y
  temp_dist = getDistance(trigger_y, echo_y);
  total_y = total_y - buffer_y[index_y] + temp_dist;
  buffer_y[index_y] = temp_dist;
  index_y = (index_y + 1) % bufferLen;
  distance_y = total_y / bufferLen;

  // Sensor z
  temp_dist = getDistance(trigger_z, echo_z);
  total_z = total_z - buffer_z[index_z] + temp_dist;
  buffer_z[index_z] = temp_dist;
  index_z = (index_z + 1) % bufferLen;
  distance_z = total_z / bufferLen;

  // Print distances to Serial Monitor
  Serial.print("x ");
  Serial.println(distance_x);

  Serial.print("y ");
  Serial.println(distance_y);

  Serial.print("z ");
  Serial.println(distance_z);

  // Debug print: Print the buffers for sensors x, y, z
  // debugPrintBuffer("Buffer x: ", buffer_x);
  // debugPrintBuffer("Buffer y: ", buffer_y);
  // debugPrintBuffer("Buffer z: ", buffer_z);

  delay(100);
}

int getDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  temp_dur = pulseIn(echoPin, HIGH);
  temp_dist = temp_dur * 0.034 / 2;
  return temp_dist;
}

void debugPrintBuffer(const char* label, int* buffer) {
  Serial.print(label);
  for (int i = 0; i < bufferLen; i++) {
    Serial.print(buffer[i]);
    Serial.print(" ");
  }
  Serial.println();
}
