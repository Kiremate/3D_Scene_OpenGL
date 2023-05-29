
// Copyright © 2014+ ÁRgB
// Código bajo licencia Boost Software License, version 1.0
// Ver www.boost.org/LICENSE_1_0.txt
// angel.rodriguez@esne.edu
// 2014.04

#ifndef ARGB_SHADER_HEADER
#define ARGB_SHADER_HEADER

    #include <string>
    #include <glad/glad.h>
    #include <Non_Copyable.hpp>

    namespace argb
    {

        class Shader : Non_Copyable
        {
        public:

            class Source_Code
            {
            public:

                static Source_Code from_string (const std::string & source_code_string)
                {
                    return Source_Code(source_code_string);
                }

                static Source_Code from_file (const std::string & file_path);

            private:

                std::string string;

            private:

                Source_Code()
                {
                }

                Source_Code(const std::string & source_code_string)
                :
                    string(source_code_string)
                {
                }

            public:

                size_t size         () const { return  string.size  (); }
                bool   is_empty     () const { return  string.empty (); }
                bool   is_not_empty () const { return !string.empty (); }

            public:

                operator const std::string & () const
                {
                    return Source_Code::string;
                }

                operator const char * () const
                {
                    return Source_Code::string.c_str ();
                }

            };

        private:

            GLuint      shader_object_id;
            std::string log_string;

        protected:

            Shader(const Source_Code & source_code, GLenum shader_type);

            virtual ~Shader();

        public:

            bool is_compiled () const
            {
                return shader_object_id != 0;
            }

            bool compilation_failed () const
            {
                return shader_object_id == 0;
            }

            const std::string & log () const
            {
                return log_string;
            }

        public:

            operator GLuint () const
            {
                return shader_object_id;
            }

        };

    }

#endif
