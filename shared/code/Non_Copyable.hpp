
// Este código es de dominio público.
// angel.rodriguez@esne.edu
// 2013

#ifndef ARGB_NON_COPYABLE_HEADER
#define ARGB_NON_COPYABLE_HEADER

    namespace argb
    {

        class Non_Copyable
        {
        protected:

            Non_Copyable() { }
            virtual ~Non_Copyable() { }

        private:

            Non_Copyable(const Non_Copyable & );
            Non_Copyable & operator = (const Non_Copyable & );

        };

    }

#endif
