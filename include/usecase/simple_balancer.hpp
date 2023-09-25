#pragma once

#include <vector>
#include <functional>

namespace hlb
{
    template<typename T>
    class SimpleBalancer;

    template<typename Out, typename... Ins>
    class SimpleBalancer<Out(Ins...)> {
    public:
        struct Executor {
            using type = std::function<Out(Ins...)>;
            using id_t = int;

            type executor;
            id_t id{ 0 };
        };

        using type = std::vector<Executor>;

        template<typename Func>
        static auto make_executor(typename Executor::id_t id, Func&& executor) {
            return Executor{ std::forward<Func>(executor), id };
        }

        type executors;
        typename type::iterator next_executor;

        Out operator()(Ins... inputs) {
            if (executors.empty())
                throw std::runtime_error("no executor!");
            auto result{ _run(std::forward<decltype(inputs)>(inputs)...) };
            _next();
            return result;
        }

        template<typename Func>
        void add_executor(typename Executor::id_t id, Func&& executor) {
            executors.emplace_back(make_executor(id, std::forward<Func>(executor)));
            next_executor = executors.begin();
        }

    private:
        Out _run(Ins... inputs) {
            return next_executor->executor(std::forward<Ins>(inputs)...);
        }

        void _next() {
            next_executor = std::next(next_executor);
            if (next_executor == executors.end())
                next_executor = executors.begin();
        }
    };
} // namespace hlb
