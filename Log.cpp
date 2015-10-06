#include "Log.h"

namespace tip {

    Log& Log::get_instance() {
        if(log == nullptr) {
            log = new Log();
        }
        return *log;
    }

    void Log::set_level(Level l) {
        this->level = l;
    }

    void Log::set_output(std::ostream* os) {
        out = os;
    }


    Log::Log() {}

    Log::~Log() {
        delete this->log;
    }

    Log* Log::log {nullptr};

}
