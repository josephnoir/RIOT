#ifndef __CPPMIX__
#define __CPPMIX__

#include <cstdio>

class cppMix {
    public:
        /**
         * @brief constructor 
         */
        cppMix();

        /**
         * @brief destructor 
         */
        ~cppMix();

        /**
         * @brief public function
         */
        void sayHello(void);
    private:
        /**
         * @brief private function
         */
        void greet(void);
};

#endif //__CPPMIX__
