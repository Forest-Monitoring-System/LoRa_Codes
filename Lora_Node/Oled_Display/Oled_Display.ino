char msg[50];
int x;
int y;
void oled(msg,x,y){
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(x,y);
    display.print("Flame Detected ");
    delay(2000);  
    display.display(); 
}


