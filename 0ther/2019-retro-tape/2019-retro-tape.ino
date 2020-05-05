// http://cmucam.org/projects/cmucam5/wiki/Hooking_up_Pixy_to_a_Microcontroller_(like_an_Arduino)
//
//pixy.blocks[i].signature; //The signature number of the detected object (1-7)
//pixy.blocks[i].x; //The x location of the center of the detected object (0 to 319)
//pixy.blocks[i].y; //The y location of the center of the detected object (0 to 199)
//pixy.blocks[i].width; //The width of the detected object (1 to 320)
//pixy.blocks[i].height; //The height of the detected object (1 to 200)
//pixy.blocks[i].print(); //A member function that prints the detected object information to the serial port

String outtxt = "";
char LF = 10;
String SN = "";
String C = ",";
String blank = SN + "1,000,000" + LF;
String Status = "1";    //status 1= object not found,  2=found
int leng = 0;
unsigned long mils = 0;
unsigned long old = 0;
uint16_t blocks;

#include <SPI.h>
#include <Pixy.h>


// This is the main Pixy object
Pixy pixy;

void setup()
{
  Serial.begin(115200); //(19200);
  pixy.init();
}

void loop()
{
  blocks = pixy.getBlocks();
  // Serial.println(blocks);
  //pixy.blocks[0].print(); //The x location of the center of the detected object (0 to 319)
 // delay(200);

  if (blocks >= 2) {
    //Serial.println(millis());
    old = millis();
    Status = "2";
    float widthratio = pixy.blocks[0].width / pixy.blocks[1].width;
    float heightratio = pixy.blocks[0].height / pixy.blocks[1].height;
    float averagex = (pixy.blocks[0].x + pixy.blocks[1].x) / 2;
    float averagey = (pixy.blocks[0].y + pixy.blocks[1].y) / 2;
    
    int distx = abs(pixy.blocks[0].x - pixy.blocks[1].x);
    int tmp1 = pixy.blocks[0].width + pixy.blocks[1].width;
    int tmp2 = tmp1/2;
    int tmp3 = distx-tmp2;
    int insidex = abs(tmp3);
    
    int disty = abs(pixy.blocks[0].y - pixy.blocks[1].y);
    int tmp4 = pixy.blocks[0].height + pixy.blocks[1].height;
    int tmp5 = tmp4/2;
    int tmp6 = disty-tmp5;
    int insidey = abs(tmp6);

   // unsigned int insidex = abs(distx - ((pixy.blocks[0].width + pixy.blocks[1].width ) / 2));
   // Serial.println(pixy.blocks[0].width);
    //Serial.println(pixy.blocks[1].width);
    //Serial.println(insidex);

    outtxt = Status + C + T(averagex) + C + T(distx)+ LF; // were using insidex, but went back to distx so we could offset
   // Serial.println(outtxt);
  }
  
  else {
    if ((millis() - old) > 45)  outtxt = blank;
  }
  if (Serial.read() == 82) {
      Serial.print(outtxt);
    }
}

String T(int val) {
  String SN = "";
  String Tmp = SN + "00" + val;
  int len = Tmp.length();
  String Final = Tmp.substring(len - 3, len);
  return Final;

}
