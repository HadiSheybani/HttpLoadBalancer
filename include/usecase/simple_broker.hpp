#pragma once

#include <map>
#include <functional>

namespace hlb
{
    template<typename Key, typename Handler>
    class SimpleBroker;

    template<typename Key, typename Out, typename... Ins>
    class SimpleBroker<Key, Out(Ins...)> {
    public:
        using key_t = Key;

        struct Executor {
            using type = std::function<Out(Ins...)>;
            using id_t = int;

            type executor;
            id_t id{ 0 };
        };

        using type = std::map<key_t, Executor>;

        template<typename Func>
        static auto make_executor(typename Executor::id_t id, Func&& executor) {
            return Executor{ std::forward<Func>(executor), id };
        }

        type executors;

        Out operator()(const key_t& key, Ins... inputs) {
            if (executors.empty())
                throw std::runtime_error("no executor!");
            auto iter{ executors.find(key) };
            if (iter == executors.end())
                throw std::runtime_error("no executor!");
            return iter->second.executor(std::forward<decltype(inputs)>(inputs)...);
        }

        template<typename Func>
        void add_executor(const key_t& key, typename Executor::id_t id, Func&& executor) {
            executors.insert_or_assign(key, make_executor(id, std::forward<Func>(executor)));
        }
    };
}