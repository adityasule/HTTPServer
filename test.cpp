#include <iostream>
#include <stdexcept>

class another_runtime_error: public std::runtime_error
{
    public:
        /**
         * The following two constructors are extentions to the std::runtime_error constructors
         */
        another_runtime_error(char *what_arg, int &which_arg): runtime_error(what_arg)
        {
            which_error = which_arg;
        }

        another_runtime_error(std::string &what_arg, int &which_arg): runtime_error(what_arg)
        {
            which_error = which_arg;
        }
        
        /**
         * Function to get the error number stored in the thrown exception
         */
        int which() { return which_error; }
    private:
        using std::runtime_error::runtime_error;
        int which_error;
};

int main(int argc, char const *argv[])
{
    another_runtime_error("Hello", 3);
    int a;
    return 0;
}

