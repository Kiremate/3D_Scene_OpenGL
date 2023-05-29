
// Código bajo licencia Boost Software License, version 1.0
// Ver www.boost.org/LICENSE_1_0.txt
// angel.rodriguez@esne.edu
// 2020.12

#ifndef ARGB_COLOR_BUFFER_HEADER
#define ARGB_COLOR_BUFFER_HEADER

    #include <algorithm>
    #include <cassert>
    #include <vector>
    #include "Color.hpp"

    namespace argb
    {

        template< class COLOR >
        class Color_Buffer
        {
        public:

            using Color_Format = COLOR;
            using Color        = Color_Format;
            using Iterator     = Color *;

        private:

            using Buffer = std::vector< Color_Format >;

        public:

            static constexpr unsigned bits_per_color ()
            {
                return Color_Format::bits;
            }

        private:

            unsigned width;
            unsigned height;
            unsigned size;

            Buffer   buffer;

            Iterator start;
            Iterator ending;

        public:

            Color_Buffer(unsigned width, unsigned height)
            :
                width ( width          ),
                height( height         ),
                size  ( width * height ),
                buffer( size           ),
                start ( buffer.data () ),
                ending( start + size   )
            {
            }

        public:

                  Color_Format * colors ()       { return buffer.data (); }
            const Color_Format * colors () const { return buffer.data (); }

                  Iterator       begin  ()       { return start; }
            const Iterator       begin  () const { return start; }

                  Iterator       end    ()       { return ending; }
            const Iterator       end    () const { return ending; }

            unsigned get_width () const
            {
                return width;
            }

            unsigned get_height () const
            {
                return height;
            }

            unsigned get_size () const
            {
                return size;
            }

        public:

            void clear (const Color & color)
            {
                std::fill_n (buffer.data (), size, color);
            }

            void set_color (unsigned x, unsigned y, Color & color)
            {
                assert(x < width && y < height);

                buffer[y * width + x] = color;
            }

            void set_color (unsigned offset, Color & color)
            {
                assert(offset < buffer.size ());

                buffer[offset] = color;
            }

            explicit operator Color_Format * ()
            {
                return colors ();
            }

            explicit operator const Color_Format * () const
            {
                return colors ();
            }

        };

    }

#endif
