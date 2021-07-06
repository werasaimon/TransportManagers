#ifndef HELP_FUNC_HPP
#define HELP_FUNC_HPP

unsigned constexpr const_hash(char const *input)
{
    return *input ?
        static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) :
        5381;
}




#endif // HELP_FUNC_HPP
