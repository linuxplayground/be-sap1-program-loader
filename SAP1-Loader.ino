
const char DATA[] = {38, 40, 42, 44, 46, 48, 50, 52};  // Data Pins (OUTPUT)
const char ADDR[] =  {22, 24, 26, 28};                 // Address Pins (OUTPUT)

#define WRITE 32                                       // Write Button (OUTPUT)
#define SEL 53                                         // Program Select Line (INPUT) 


/*
 * This assembly is compiled into machine code by: https://github.com/threadException/EaterEmulator
 *  
 
start:
    LDI 1
    STA n1
    LDI 0
loop:
    STA n2
    OUT
    LDA n1
    ADD n2
    STA n1
    OUT
    LDA n2
    ADD n1
    JC end
    JMP loop
end:
    JMP start
n1:
    0
n2:
    1%
 */
const char fib[] = {
  0b01010001,
  0b01001110,
  0b01010000,
  0b01001111,
  0b11100000,
  0b00011110,
  0b00101111,
  0b01001110,
  0b11100000,
  0b00011111,
  0b00101110,
  0b01111101,
  0b01100011,
  0b01100000,
  0b00000000,
  0b00000001
};

/*
 * This assembly is compiled into machine code by: https://github.com/threadException/EaterEmulator
 * 

count_up:
  OUT
  ADD counter
  JC count_dn
  JMP count_up
count_dn:
  SUB counter
  OUT
  JZ count_up
  JMP count_dn
counter:
  1
 */
const char updown[] = {
  0b11100000,
  0b00101000,
  0b01110100,
  0b01100000,
  0b00111000,
  0b11100000,
  0b10000000,
  0b01100100,
  0b00000001  
};

/*
 * Set the address lines
 * @param int Address - a number from 0 to 15.
 */
void setAddress(int address) {
  for (int i = 0; i < 4; i += 1) {
    digitalWrite(ADDR[i], address & 1);
    address = address >> 1;
  }
}

/*
 * Set the data lines
 * @param data - A binary value
 */
void setData(int data) {
  Serial.println(data);
  for (int i = 0; i <= 7; i += 1) {
    digitalWrite(DATA[i], data & 1);
    data = data >> 1; 
  }
}

/*
 * Pulse the write to ram line. This line is normally held high by the switch
 * send it low for 10 milliseconds.
 */
void strobe() {
  digitalWrite(WRITE, LOW);
  delay(10);
  digitalWrite(WRITE, HIGH);
  delay(10);  // I just chucked in this delay to slow things down a little.  I'm old and want to see the flashing lights.
}

void setup() {
  digitalWrite(WRITE, HIGH);
  pinMode(WRITE, OUTPUT);

  pinMode(SEL, INPUT); // This is how we choose between fibonacci or counter
  
  for (int i = 0; i < 8; i += 1) {
    digitalWrite(DATA[i], LOW);
    pinMode(DATA[i], OUTPUT);
  }
  for (int i = 0; i < 4; i += 1) {
    digitalWrite(ADDR[i], LOW);
    pinMode(ADDR[i], OUTPUT);
  }

  /*
   * Now look here! this bit is nasty as all hell.  I know I can do something far more elegant,
   * I could probably even write an assembler in this sketch and have it be interfaced through
   * the serial port and build some kind of monitor.  But that's already been done.  I just want
   * to load in code without using dips.  That's what this does.
   * 
   * It's not pretty - but it works.... most of the time.
   */
  if (digitalRead(SEL)) {
    for (int i=0; i < sizeof(fib)/sizeof(fib[0]); i+=1) {
      setAddress(i);
      setData(fib[i]);
      strobe();
    }
  } else {
    for (int i=0; i < sizeof(updown)/sizeof(updown[0]); i+=1) {
      setAddress(i);
      setData(updown[i]);
      strobe();
    }    
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
