#ifndef I_STRINGIFY_HPP
#define I_STRINGIFY_HPP

namespace interfaces {

    struct IStringify {
        IStringify() = default;

        virtual ~IStringify() noexcept = default;

        [[nodiscard]] virtual std::string toString() const noexcept = 0;
    };

}

#endif //I_STRINGIFY_HPP
