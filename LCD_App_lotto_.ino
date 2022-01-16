#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define pinSW1 D2
#define pinSW2 D3
#define YES 1
#define NO 0

LiquidCrystal_I2C lcd(0x27, 16, 2); 

int LOTTO[6] = {0};
int num_cnt = 0;
int dis_cnt = 0;
bool swState[4] = {false};
// swState[0] : Current State of SW1
// swState[1] : Last    State of SW1
// swState[2] : Current State of SW2
// swState[3] : Last    State of SW2

void setup() {
  Wire.begin();
  pinMode(pinSW1,INPUT);
  pinMode(pinSW2,INPUT);

  lcd.init();
  lcd.backlight();
  ClearDisplay(0);
}

void loop() {
  swState[0] = digitalRead(pinSW1);
  swState[2] = digitalRead(pinSW2);
  delay(10);

  if(swState[0] && !swState[1])
  {
    if(num_cnt < 6)
    {
      bool CHECK = true;
      char tmpStr[3];

      if (num_cnt > 0)
      {
        while(CHECK)
        {
          int tmpCnt = 0;
          
          LOTTO[num_cnt] = random(1,46);
          
          for (int i=0 ; i < num_cnt ; i++)
            if (LOTTO[num_cnt] == LOTTO[i]) tmpCnt++;

          if (tmpCnt == 0) CHECK = false;
        }
        
        if(num_cnt < 3) lcd.setCursor(7 + 3*num_cnt,0);
        else lcd.setCursor(7 + 3*(num_cnt-3),1);
        lcd.print(itoa(LOTTO[num_cnt],tmpStr,10));
      }
      else 
      {
        LOTTO[num_cnt] = random(1,46);

        lcd.setCursor(7 + 3*num_cnt,0);
        lcd.print(itoa(LOTTO[num_cnt],tmpStr,10));
      }
      if(num_cnt==5) dis_cnt = 1;
    }
    num_cnt++;
  }
   
  if(swState[2] && !swState[3])
  {
    if(dis_cnt == 1)
    {
      char tmpStr[3];
      Insertion_Sort(LOTTO,6);
      ClearDisplay(1);    

      for(int i=0 ; i<6 ; i++) 
      {
        if(i < 3) lcd.setCursor(7 + 3*i,0);
        else lcd.setCursor(7 + 3*(i-3),1);
        lcd.print(itoa(LOTTO[i],tmpStr,10)); 
      }      
      dis_cnt = 2;
    }
    else if(dis_cnt == 2)
    {
      num_cnt = 0;
      dis_cnt = 0;
      for (int i=0 ; i<6 ; i++) LOTTO [i]= 0;
      ClearDisplay(0);
    }
  }
  swState[1] = swState[0];
  swState[3] = swState[2];
}
     
void Insertion_Sort(int arr[], int n){
  int i, j;
  int key;

  for (i=1 ; i<n ; i++) {
    key=arr[i];
    for (j=i-1 ; j>=0 ; j--){
      if(arr[j]>key) arr[j+1] = arr[j];
      else break;
    }
    arr[j+1] = key;   
  }
}

void ClearDisplay(int SORT)
{
  if( SORT == NO)
  {
    lcd.setCursor(0,0);
    lcd.print("*LOTTO:  |  |  |");
    lcd.setCursor(0,1);
    lcd.print("      :  |  |  |");
  }
  else if(SORT == YES)
  {
    lcd.setCursor(0,0);
    lcd.print("*LOTTO:  |  |  |");
    lcd.setCursor(0,1);
    lcd.print("(sort):  |  |  |");
  }
}
