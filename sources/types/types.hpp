#ifndef LIBS_TYPES_HPP
#define LIBS_TYPES_HPP

#include <concepts>
#include <type_traits>
#include <string>
#include <cinttypes>
#include <unordered_map>
#include <array>


namespace hlibs::types {

    template<typename T>
    struct ObjectCounter {
      public:
        static unsigned int created;
        static unsigned int alive;

        ObjectCounter()
        {
            increment();
        }

        ObjectCounter(const ObjectCounter&)
        {
            increment();
        }

        ObjectCounter& operator=(const ObjectCounter&) = delete;

        ObjectCounter(ObjectCounter&&) noexcept = delete;

        ObjectCounter& operator=(ObjectCounter&&) noexcept = delete;

      protected:
        virtual ~ObjectCounter() noexcept
        {
            --alive;
        }

        inline void increment() const
        {
            ++created;
            ++alive;
        }

    };

    template<typename T> unsigned int ObjectCounter<T>::alive(0);
    template<typename T> unsigned int ObjectCounter<T>::created(0);

}

#endif //LIBS_TYPES_HPP
