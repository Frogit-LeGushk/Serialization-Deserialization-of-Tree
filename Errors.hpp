#pragma once
#include <string>
#include <inttypes.h>


namespace NSErr {

    /** all codes of errors in class Error **/
    enum class ErrEnum : uint16_t {
        OVERFLOW = 1,
        EMPTY,
        RANGE_OUT
    };

    /** ejected object in try catch syntax-construction **/
    class Error {
    public:
        explicit Error(const std::string & message, const ErrEnum code)
            : _message(message), _code(code)
        {}
        explicit Error(const char * message, const ErrEnum code)
            : _message(std::string(message)), _code(code)
        {}

        std::string message() const { return _message; }
        ErrEnum code() const { return _code; }

        Error() = delete;
        Error(const Error & err) = delete;
        Error(Error && err) = default;
        Error & operator=(const Error & err) = delete;
        Error & operator=(Error && err) = delete;
        ~Error() = default;
    private:
        std::string _message;
        ErrEnum _code;
    };
}
