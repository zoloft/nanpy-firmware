#ifndef DISPLAY_H
#define DISPLAY_H

#include "cfg_all.h"
#include "consts.h"
#include <Arduino.h>

namespace nanpy {
  class Display {
    public:

      Display();
      Display(const Display&);
      Display& operator=(const Display&);
      ~Display();

      int getSeconds();
      void setSeconds(int secs);

      int getMinutes();
      void setMinutes(int mins);

      int getHours();
      void setHours(int hrs);

      void setup();
      void loop();
  };
}

#endif
