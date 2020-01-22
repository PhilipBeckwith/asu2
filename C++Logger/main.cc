#include <iostream>
#include <string>
#include "Logger.h"

using namespace std;

Logger log;

void showReccurision(int x){
  log.enterFunction("showReccurision");
  log.argument("x", x);
  if(x%3==0){
    log.info("\%3");
  }

  x--;
  log.variable("x", x);
  if(0<x){
    showReccurision(x);
  }

  log.exitFunction("Going up!");
}

int main(){
  cout<<"Starting program\n";
  showReccurision(6);
  cout<<"Ending Program\n";
}
