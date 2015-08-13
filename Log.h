#include <iostream>

namespace tip {
    
    class Log {
    public:
        enum class Level {DUMP, MESSAGE, DEBUG, CRITICAL, NONE};
        
        ~Log();
        
        static Log& get_instance();
        
        void set_level(Level l);
        void set_output(std::ostream* os);
        template<class T>
        void print(Level l, const T& message) {
            if(static_cast<int>(l) >= static_cast<int>(this->level))
                *out << message;
        }
        
        template<typename T, typename... Targs>
        void print(Level l, const T& message, Targs... args) {
            print(l, message);
            print(l, args...);
        }
        
    private:
        Log();
        Log(const Log&) = delete;
        Log operator=(const Log&) = delete;
        
        static Log* log;
        
        std::ostream* out = &(std::cerr);
        Level level = Level::NONE;
    };
    
    template<typename... Targs>
    void DUMP(Targs... message) {
        Log::get_instance().print(Log::Level::DUMP, message..., '\n');
    }
    template<typename... Targs>
    void MESSAGE(Targs... message) {
        Log::get_instance().print(Log::Level::MESSAGE, message..., '\n');
    }
    template<typename... Targs>
    void DEBUG(Targs... message) {
        Log::get_instance().print(Log::Level::DEBUG, message..., '\n');
    }
    template<typename... Targs>
    void CRITICAL(Targs... message) {
        Log::get_instance().print(Log::Level::CRITICAL, message..., '\n');
    }
}
