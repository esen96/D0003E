#ifndef LCD_H_
#define LCD_H_

void initLCD(void);
void writeChar(char ch, int pos);
void printAt(long num, int pos);
void switchIndicator(int pulseGenerator);


#endif /* LCD_H_ */