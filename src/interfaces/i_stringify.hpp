#ifndef LIBS_I_STRINGIFY_HPP
#define LIBS_I_STRINGIFY_HPP

namespace interfaces {

    struct IStringify {
        IStringify() = default;

        virtual ~IStringify() noexcept = default;

        [[nodiscard]] virtual std::string toString() const noexcept = 0;
    };

}

#endif //LIBS_I_STRINGIFY_HPP
