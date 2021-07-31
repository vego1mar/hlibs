#ifndef I_STRINGIFY_HPP
#define I_STRINGIFY_HPP

namespace libs::types::interfaces {

    struct IStringify {
      public:
        [[nodiscard]] virtual std::string toString() const noexcept = 0;

      protected:
        IStringify() = default;
        virtual ~IStringify() noexcept = default;
    };

}

#endif //I_STRINGIFY_HPP
