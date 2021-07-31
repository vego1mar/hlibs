#ifndef LIBS_INTERFACES_HPP
#define LIBS_INTERFACES_HPP

namespace libs::types::interfaces {

    struct IStringify {
      public:
        [[nodiscard]] virtual std::string toString() const noexcept = 0;

      protected:
        IStringify() = default;
        virtual ~IStringify() noexcept = default;
    };

}

#endif //LIBS_INTERFACES_HPP
