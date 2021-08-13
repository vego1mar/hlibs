#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../../sources/types/interfaces.hpp"


TEST_CASE("IStringify", "[libs][interfaces]")
{
    using hlibs::types::interfaces::IStringify;


    class Implementer0 : public IStringify {
      private:
        uint8_t value;

      public:
        explicit Implementer0(const uint8_t& v) : value(v)
        {
        }

        [[nodiscard]] std::string toString() const noexcept override
        {
            return "Implementer0(" + std::to_string(value) + ')';
        }

        [[nodiscard]] inline const uint8_t& get() const noexcept
        {
            return value;
        }

        inline void set(const uint8_t& v) noexcept
        {
            value = v;
        }
    };

    SECTION("copy constructor (impl0) -> OK", "[expected_usage]") {
        Implementer0 obj{1};
        Implementer0 copy(obj);
        CHECK(copy.get() == obj.get());
        REQUIRE_THAT(copy.toString(), Catch::Matchers::Equals(obj.toString()));
    }

    SECTION("copy assignment operator (impl0) -> OK", "[expected_usage]") {
        Implementer0 obj{2};
        Implementer0 copy{0};
        copy = obj;
        CHECK(copy.get() == obj.get());
        REQUIRE_THAT(copy.toString(), Catch::Matchers::Equals(obj.toString()));
    }

    SECTION("move constructor (impl0) -> OK", "[expected_usage]") {
        Implementer0 obj{3};
        const auto objMsg = obj.toString();
        Implementer0 move(std::move(obj));
        CHECK_THAT(objMsg, Catch::Matchers::Equals(move.toString()));
    }

    SECTION("move assignment operator (impl0) -> OK", "[expected_usage]") {
        Implementer0 obj{4};
        const auto objMsg = obj.toString();
        Implementer0 move{41};
        move = std::move(obj);
        CHECK_THAT(move.toString(), Catch::Matchers::Equals(objMsg));
    }


    struct Implementer1 : public IStringify {
      public:
        std::vector<std::string> messages;

        Implementer1()
        {
            messages.emplace_back("DefaultConstructor");
        }

        [[maybe_unused]] Implementer1(const Implementer1& rhs) : messages(rhs.messages)
        {
            messages.emplace_back("CopyConstructor");
        }

        [[maybe_unused]] Implementer1(Implementer1&& rhs) noexcept : messages(std::move(rhs.messages))
        {
            messages.emplace_back("MoveConstructor");
        }

        Implementer1& operator=(const Implementer1& rhs)
        {
            messages.emplace_back("CopyAssignmentOperator");
            return *this;
        }

        Implementer1& operator=(Implementer1&& rhs) noexcept
        {
            messages.emplace_back("MoveAssignmentOperator");
            return *this;
        }

        ~Implementer1() noexcept override = default;

        [[nodiscard]] std::string toString() const noexcept override
        {
            return "Implementer1(" + std::to_string(messages.size()) + ')';
        }

        static std::string ToString(const std::vector<std::string>& messages)
        {
            std::string result{};

            for (const auto& msg : messages) {
                result.append(msg + ',');
            }

            return result;
        }
    };

    SECTION("copy constructor (impl1) -> OK", "[ascertain_behaviour]") {
        Implementer1 obj{};
        Implementer1 copy(obj);
        CHECK(obj.messages.size() == 1);
        CHECK(copy.messages.size() == 2);
        CHECK_THAT(obj.toString(), !Catch::Matchers::Equals(copy.toString()));
        REQUIRE_THAT(Implementer1::ToString(obj.messages), Catch::Matchers::Equals("DefaultConstructor,"));
        REQUIRE_THAT(Implementer1::ToString(copy.messages), Catch::Matchers::Equals("DefaultConstructor,CopyConstructor,"));
    }

    SECTION("copy assignment operator (impl1) -> OK", "[ascertain_behaviour]") {
        Implementer1 obj1{};
        Implementer1 obj2{};
        obj2 = obj1;
        CHECK(obj1.messages.size() == 1);
        CHECK(obj2.messages.size() == 2);
        CHECK_THAT(obj1.toString(), !Catch::Matchers::Equals(obj2.toString()));
        REQUIRE_THAT(Implementer1::ToString(obj1.messages), Catch::Matchers::Equals("DefaultConstructor,"));
        REQUIRE_THAT(Implementer1::ToString(obj2.messages), Catch::Matchers::Equals("DefaultConstructor,CopyAssignmentOperator,"));
    }

    SECTION("move constructor (impl1) -> OK", "[ascertain_behaviour]") {
        Implementer1 moved{};
        const auto movedStr = moved.toString();
        const auto movedMessagesSize = moved.messages.size();
        const auto movedMessages = Implementer1::ToString(moved.messages);
        Implementer1 obj(std::move(moved));
        CHECK(movedMessagesSize == 1);
        CHECK(obj.messages.size() == 2);
        CHECK_THAT(movedStr, !Catch::Matchers::Equals(obj.toString()));
        REQUIRE_THAT(movedMessages, Catch::Matchers::Equals("DefaultConstructor,"));
        REQUIRE_THAT(Implementer1::ToString(obj.messages), Catch::Matchers::Equals("DefaultConstructor,MoveConstructor,"));
    }

    SECTION("move assignment operator (impl1) -> OK", "[ascertain_behaviour]") {
        Implementer1 moved{};
        const auto movedStr = moved.toString();
        const auto movedMsgSize = moved.messages.size();
        const auto movedMsg = Implementer1::ToString(moved.messages);
        Implementer1 obj{};
        obj = std::move(moved);
        CHECK(movedMsgSize == 1);
        CHECK(obj.messages.size() == 2);
        CHECK_THAT(movedStr, !Catch::Matchers::Equals(obj.toString()));
        REQUIRE_THAT(movedMsg, Catch::Matchers::Equals("DefaultConstructor,"));
        REQUIRE_THAT(Implementer1::ToString(obj.messages), Catch::Matchers::Equals("DefaultConstructor,MoveAssignmentOperator,"));
    }

}