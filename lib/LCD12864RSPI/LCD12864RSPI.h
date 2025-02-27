/*

 // # Update the library and sketch to compatible with IDE V1.0 and earlier
 
 // # Description:
 // # The sketch for using LCD12864 in serial mode(3-wire communication)

 // # Version: 0.2
 
 */

 #ifndef LCD12864RSPI_h
 #define LCD12864RSPI_h
 #include <avr/pgmspace.h>
 #include <inttypes.h>
 
 
 class LCD12864RSPI {
 typedef unsigned char uchar;
 
 
 public:
 
 LCD12864RSPI(int RS, int RW, int E);
 
 void Init(void);
 void delayns(void);
 
 void WriteByte(int dat);
 void WriteCommand(int CMD);
 void WriteData(int CMD);
 
 
 void Clear(void);
 void DisplayString(int X,int Y,uchar *ptr,int dat);
 void DisplaySig(int M,int N,int sig);
 void DisplayFullScreen(uchar *ptr1, uchar *ptr2, uchar *ptr3, uchar *ptr4);
 void DrawFullScreen(uchar *p);
 void img1(uchar img[]);
 void img2(uchar img[]);

 int delaytime;
 int DEFAULTTIME;

 private:
 int RS_Pin; 	  // RS
 int RW_Pin;      // R/W
 int E_Pin;       // E
 
 };   
 #endif