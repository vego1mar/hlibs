#ifndef OBJECT_COUNTER_HPP
#define OBJECT_COUNTER_HPP


namespace templates {

    template<typename T>
    struct ObjectCounter {
    public:
        static unsigned int created;
        static unsigned int alive;

    public:
        ObjectCounter() {
            increment();
        }

        ObjectCounter(const ObjectCounter&) {
            increment();
        }

        ObjectCounter& operator=(const ObjectCounter&) = delete;

        ObjectCounter(ObjectCounter&&) noexcept = delete;

        ObjectCounter& operator=(ObjectCounter&&) noexcept = delete;

    protected:
        virtual ~ObjectCounter() noexcept {
            --alive;
        }

        inline void increment() {
            ++created;
            ++alive;
        }

    };

    template<typename T> unsigned int ObjectCounter<T>::alive(0);
    template<typename T> unsigned int ObjectCounter<T>::created(0);

}

#endif //OBJECT_COUNTER_HPP
