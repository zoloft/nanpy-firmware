#include "cfg.h"

#if USE_Display

#include <Arduino.h>
#include "Display.h"
#include "DisplayClass.h"
#include <stdlib.h>

const char* nanpy::DisplayClass::get_firmware_id() {
  return "Display";
}

void nanpy::DisplayClass::elaborate( MethodDescriptor* m ) {
  ObjectsManager<Display>::elaborate(m);

  if (strcmp(m->getName(), "new") == 0) {
    Display* disp = new Display();
    v.insert(disp);
    m->returns(v.getLastIndex());
  }

  if (strcmp(m->getName(), "setSeconds") == 0) {
    Display* disp = v[m->getObjectId()];
    disp->setSeconds(m->getInt(0));
    m->returns(0);
  }

  if (strcmp(m->getName(), "getSeconds") == 0) {
    Display* disp = v[m->getObjectId()];
    m->returns(disp->getSeconds());
  }

  if (strcmp(m->getName(), "setMinutes") == 0) {
    Display* disp = v[m->getObjectId()];
    disp->setMinutes(m->getInt(0));
    m->returns(0);
  }

  if (strcmp(m->getName(), "getMinutes") == 0) {
    Display* disp = v[m->getObjectId()];
    m->returns(disp->getMinutes());
  }

  if (strcmp(m->getName(), "setHours") == 0) {
    Display* disp = v[m->getObjectId()];
    disp->setHours(m->getInt(0));
    m->returns(0);
  }

  if (strcmp(m->getName(), "getHours") == 0) {
    Display* disp = v[m->getObjectId()];
    m->returns(disp->getHours());
  }

  if (strcmp(m->getName(), "setup") == 0) {
    Display* disp = v[m->getObjectId()];
    disp->setup();
    m->returns(0);
  }

};

#endif
