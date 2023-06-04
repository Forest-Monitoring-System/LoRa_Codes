int x;
int y;
void oled(String msg,int x,int y){
    display.clearDisplay();  
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(x,y);
    display.print(msg);
    delay(2000);  
    display.display(); 
     
}


