#ifndef DISPLAY_CLASS
#define DISPLAY_CLASS

#include "BaseClass.h"
#include "MethodDescriptor.h"

class Display;

namespace nanpy {
    class DisplayClass: public ObjectsManager<Display> {
        public:
            void elaborate( nanpy::MethodDescriptor* m );
            const char* get_firmware_id();
    };
}

#endif
