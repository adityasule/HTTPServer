#include <iostream>
#include <errno.h>
#include <stdexcept>
#include <mutex>

#ifndef _ERROR_H_
#define _ERROR_H_

namespace Core
{
    /**
     * This is the standard error thrown throughout the program
     * The functionality of std::runtime_error is extended to also store an errornumber
     * This errornumber can be the number reported by the global variable errorno.
     */
    class socket_runtime_error: public std::runtime_error
    {
        public:
            /**
             * The following two constructors are extentions to the std::runtime_error constructors
             */
            socket_runtime_error(char *what_arg, int &which_arg): runtime_error(what_arg)
            {
                which_error = which_arg;
            }

            socket_runtime_error(std::string &what_arg, int &which_arg): runtime_error(what_arg)
            {
                which_error = which_arg;
            }
            
            /**
             * Function to get the error number stored in the thrown exception
             */
            int which() { return which_error; }
        private:
            int which_error;
    };

    /**
     * For thread safe error logging, use this
     * TODO: Extend class functanility to accept format strings itself like fprintf()
     */
    class ErrorLog
    {
        public:
            /**
             * Stop compiler from creating move and copy constructors and assignment operator
             */
            ErrorLog(ErrorLog &other) = delete;
            ErrorLog &operator=(ErrorLog &other) = delete;

            /**
             * Function to acquire only instance of class
             */
            static ErrorLog &get();

            /**
             * Functions to log errors
             */
            void log(char *errmsg);
            void log(std::string &errmsg);
        private:
            static std::mutex *mu;


    };
}

#endif
